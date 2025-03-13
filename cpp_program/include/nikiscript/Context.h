#pragma once

#include <vector>
#include <stdint.h>
#include <string>
#include <unordered_map>

#include "Command.h"
#include "CommandHandler.h"
#include "PrintCallback.h"
#include "ProgramVariable.h"
#include "Lexer.h"

namespace ns {
	/**
	 * @warning **DO NOT** rearrange this enum. ns::handleConsoleVariableCall uses bit logic on VARIABLE related to VARIABLE_IN_VARIABLE
	 */
	enum OriginType : uint8_t {
		COMMAND = 1, ///< if a command is calling another command
		VARIABLE = 2, ///< any variable
		VARIABLE_IN_VARIABLE = 4, ///< var x that calls var y
		VARIABLE_LOOP = 8, ///< '!'
		VARIABLE_TOGGLE = 16, ///< '+' or '-'
		FILE = 32, ///< ns::parseFile or exec command
		INTERNAL = 64, ///< raw script generated from C++ code and not from a file or variable or anything else
	};

	struct Context;

	struct Arguments {
		std::vector<std::string> arguments;
		uint8_t offset = 0;

		std::string& getString();

		template<typename T>
		T getInteger();
		template<typename T>
		T getUnsignedInteger();

		float getFloat();
		double getDouble();
		long double getLongDouble();

		/**
		 * @brief clears arguments vector and sets offset to 0
		 */
		void clear();

#ifdef NIKISCRIPT_ARGUMENTS_EXTRA
		NIKISCRIPT_ARGUMENTS_EXTRA
#endif
	};

	typedef std::unordered_map<std::string, std::string> ConsoleVariables;
	typedef std::vector<ConsoleVariables::pointer> LoopVariablesRunning;
	typedef std::vector<ConsoleVariables::pointer> ToggleVariablesRunning; ///< This is unecessary to be a pointer but I like the idea of using only 8 bytes instead of the same bytes as the var name
	typedef std::vector<Command*> ToggleCommandsRunning;

	struct Context {
		Lexer* pLexer = nullptr;

		Command* pCommand = nullptr;

		Arguments arguments;

		ConsoleVariables consoleVariables;
		ProgramVariables programVariables;

		CommandHandler commands;

		LoopVariablesRunning loopVariablesRunning;
		ToggleVariablesRunning toggleVariablesRunning;
		ToggleCommandsRunning toggleCommandsRunning;

		std::string filePath; ///< when running script from a file
		size_t lineIndex = 0, lineCount = 0;

		uint8_t origin = 0; ///< this is used so that the command knows where he's running in. See ns::OriginType
	};

	/**
	 * @brief Do not copy Context without calling this function.
	 * LoopVariablesRunning and ToggleVariablesRunning stores pointers
	 * pointed to ConsoleVariables as well as toggleCommandsRunning whose
	 * pointers are from CommandHandler. That's why this function exists:
	 * It updates all those pointers.
	 * @param source object to copy content from
	 */
	Context copyContext(const Context& source);
}

template<typename T>
T ns::Arguments::getInteger() {
	return std::stoll(arguments[offset++]);
}

template <typename T>
T ns::Arguments::getUnsignedInteger() {
	return std::stoull(arguments[offset++]);
}

uint8_t operator|(ns::OriginType l, ns::OriginType r);
uint8_t operator|(uint8_t l, ns::OriginType r);
uint8_t operator|(ns::OriginType l, uint8_t r);
uint8_t& operator|=(uint8_t& l, ns::OriginType r);
uint8_t operator&(uint8_t l, ns::OriginType r);
uint8_t operator&(ns::OriginType l, uint8_t r);
uint8_t& operator&=(uint8_t& l, ns::OriginType r);
uint8_t operator~(ns::OriginType l);