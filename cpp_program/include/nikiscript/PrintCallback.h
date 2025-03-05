#pragma once

#include <stdint.h>
#include <string>

#include "Utils.h"

namespace ns {
	enum class PrintLevel : uint8_t {
		DEFAULT = 0, ///< any user input
		ECHO, ///< any text that came from a command that is not an error
		WARNING, ///< anything that is wrong but can continue
		ERROR, ///< anything that went wrong and can not continue
	};

	std::string printLevelToString(PrintLevel level);

	typedef void(*PrintCallback)(void* pData, PrintLevel level, const std::string& message);

	extern PrintCallback printCallback;
	extern void* pPrintCallbackData;

	template<typename... Args>
	static void printf(const PrintLevel& level, const std::string& format, Args ... args) {
		print(level, formatString(format, args...));
	}

	extern PrintCallback printCallback;
	extern void* pPrintCallbackData;

	void setPrintCallback(void* pData, PrintCallback callback);
	void print(const PrintLevel& level, const std::string& str);
	void printUnknownCommand(const std::string& command);
}