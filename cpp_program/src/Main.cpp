#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

#include <nikiscript/PrintCallback.h>
#include <nikiscript/Parser.h>
#include <nikiscript/NikiScript.h>

#include <Commands.h>

void nikiscriptPrintCallback(void*, ns::PrintLevel level, const std::string& message) {
	switch (level) {
	case ns::PrintLevel::ECHO:
		std::cout << "ECHO:\n";
		break;

	case ns::PrintLevel::WARNING:
		std::cout << "WARNING:\n";
		break;

	case ns::PrintLevel::ERROR:
		std::cout << "ERROR:\n";
		break;

	default:
		std::cout << "DEFAULT:\n";
		break;
	}

	std::cout << message;
}

std::string getUint64_t(ns::ProgramVariable *pVar) {
	return std::to_string(*static_cast<uint64_t*>(pVar->pValue));
}

void setUint64_t(ns::ProgramVariable* pVar, const std::string& str) {
	if (isOwner) *static_cast<uint64_t*>(pVar->pValue) = std::stoull(str);
}

int main() {
	ns::setPrintCallback(nullptr, nikiscriptPrintCallback);
	ns::maxConsoleVariableCalls = 10;

	ns::Context ctx;
	::registerCommands(ctx);
	ns::registerVariable(ctx, "cvars_calls_max", "how many variables can be called inside each other", &ns::maxConsoleVariableCalls, getUint64_t, setUint64_t);

	ns::Lexer lexer;
	ctx.pLexer = &lexer;

	while (true) {
		std::getline(std::cin, lexer.input);
		if (lexer.input.size() < 2)
			continue;

		if (lexer.input.back() == '1') // is owner
			isOwner = true;
		else
			isOwner = false;
		lexer.input.pop_back();

		discord.clear();
		ns::parse(ctx);
		lexer.clear();

		std::cout << "\nDISCORD\n" << discord << "\nEND\n";
	}
}