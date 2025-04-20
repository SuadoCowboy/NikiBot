#pragma once

#include "Context.h"

#ifndef NIKISCRIPT_LOOP_VARIABLE
#define NIKISCRIPT_LOOP_VARIABLE '!'
#endif

#ifndef NIKISCRIPT_TOGGLE_ON
#define NIKISCRIPT_TOGGLE_ON '+'
#endif

#ifndef NIKISCRIPT_TOGGLE_OFF
#define NIKISCRIPT_TOGGLE_OFF '-'
#endif

#ifndef NIKISCRIPT_FILE_EXTENSION
#define NIKISCRIPT_FILE_EXTENSION ".cfg"
#endif

#define PATH_SEPARATOR "/"

#ifndef NIKISCRIPT_CFG_ROOT_DIRECTORY
#define NIKISCRIPT_CFG_ROOT_DIRECTORY "cfg" PATH_SEPARATOR
#endif

namespace ns {
	/**
	 * @brief Clear anything related to the current lexer statement
	 * @note It does not go to the next EOS
	 */
	void clearStatementData(Context& ctx);

	/**
	 * @brief if variable is a toggle variable and its allowed to run or if it's a common variable.
	 * It's also mandatory to call this function so that it updates those special variables
	 *
	 * @note loop variables are only appended/erased from Context::LoopVariablesRunning
	 * @param ctx
	 * @see ns::parse
	 * @see ns::handleIdentifierToken
	 */
	bool canRunVariable(Context& ctx);

	/**
	 * @brief Called in parse function when EOS or END is reached
	 * @note Sets ctx.pCommand to nullptr
	 * @note ctx.pCommand should point to a valid Command object
	 * @param ctx
	 * @param pProgramVar if not null: sets/gets program variable
	 * @see ns::parse
	 */
	void handleCommandCall(Context& ctx, ProgramVariable*& pProgramVar);

	/**
	 * @brief Called in parse function when IDENTIFIER token is passed
	 * 
	 * @param ctx
	 * @param pProgramVar
	 * @return 0 = it called advanceUntil(EOS) 1 = should call pLexer->advance() 2 = if it's a variable and should run it
	 * @see ns::parse
	 * @see ns::canRunVariable
	 */
	uint8_t handleIdentifierToken(Context& ctx, ProgramVariable*& pProgramVar, bool printError);

	/**
	 * @brief Handles references in the string and checks if the parameter matches the argument type
	 * @note Possible argument types: s = string, i = integer, d = decimal, v = variable
	 * @see ns::parse
	 */
	void handleArgumentToken(Context& ctx, bool printError);

	/**
	 * @brief Called in parse function when an ConsoleVariable is passed as a IDENTIFIER
	 * @note I find this algorithm very interesting and fun(it creates a bunch of lexers for each variable call instead of recursive calling)
	 * @see ns::Context::maxConsoleVariablesRecursiveDepth
	 * @param ctx
	 * @param pProgramVar
	 */
	void handleConsoleVariableCall(Context& ctx, ProgramVariable*& pProgramVar, bool printError);

	/**
	 * @brief if a loop variable is active, its script is ran here
	 * @param ctx
	 */
	void updateLoopVariables(Context& ctx);

	/**
	 * @brief Parses and interpret scripts: handles commands and variables as well as their arguments
	 * @param ctx
	 */
	void parse(Context& ctx, bool printError=true);
	bool parseFile(Context& ctx, const char* filePath, bool printError);

	/**
	 * @brief should be used when the main context is already using the lexer
	 * @param ctx
	 * @param input
	 * @note the newlines from the end are removed
	 * @see ns::printAppendToString
	 * @return the print output which is not printed by the commands
	 * @warning print function is set to ns::printAppendToString
	 */
	std::string parseInsideAnotherScript(Context& ctx, const char* input); // TODO: when making NikiScript thread-safe, don't forget this function!

	std::string getCfgRootDirectory();
}

#ifdef NIKISCRIPT_IMPLEMENTATION
#undef NIKISCRIPT_IMPLEMENTATION
std::string ns::getCfgRootDirectory() {
	return std::string(NIKISCRIPT_CFG_ROOT_DIRECTORY);
}
#endif