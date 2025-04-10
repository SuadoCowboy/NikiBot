#pragma once

#include <stdint.h>
#include <string>

#include "Utils.h"

namespace ns {
	enum PrintLevel : uint8_t {
		DEFAULT = 0, ///< any user input
		ECHO, ///< any text that came from a command that is not an error
		WARNING, ///< anything that is wrong but can continue
		ERROR, ///< anything that went wrong and can not continue
	};

	const char* levelToString(PrintLevel level);

	typedef void(*PrintCallback)(void* pData, PrintLevel level, const char* message);

	extern PrintCallback printCallback;
	extern void* pPrintCallbackData;

	template<typename... Args>
	void printf(PrintLevel level, const char* format, Args ... args) {
		print(level, formatString(format, args...).c_str());
	}

	void setPrintCallback(void* pData, PrintCallback callback);
	void print(PrintLevel level, const char* str);
	void printUnknownCommand(const char* command);
}