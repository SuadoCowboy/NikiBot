#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

#define NIKISCRIPT_IMPLEMENTATION
#include <nikiscript/PrintCallback.h>
#include <nikiscript/Parser.h>
#include <nikiscript/NikiScript.h>

#include <Commands.h>

void nikiscriptPrintCallback(void*, ns::PrintLevel level, const char* message) {
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

int main() {
	ns::setPrintCallback(nullptr, nikiscriptPrintCallback);

	ns::Context ctx{};
	ctx.maxConsoleVariablesRecursiveDepth = 10;

	::registerCommands(ctx);
	ns::registerVariable(ctx, "cvars_calls_max", "how many variables can be called inside each other", &ctx.maxConsoleVariablesRecursiveDepth, ns::getNumber<uint16_t>, ns::setUnsigned<uint16_t>);

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