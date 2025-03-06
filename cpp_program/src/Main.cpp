#include <iostream>
#include <fstream>
#include <filesystem>

#include <nikiscript/PrintCallback.h>
#include <nikiscript/Parser.h>

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
		break;
	}

	std::cout << message;
}

int main() {
	ns::setPrintCallback(nullptr, nikiscriptPrintCallback);
	ns::maxConsoleVariableCalls = 10;

	ns::Context ctx;
	registerCommands(ctx);

	ns::Lexer lexer;
	ctx.pLexer = &lexer;

	while (true) {
		std::cout << "input\n";
		std::getline(std::cin, lexer.input);
		if (lexer.input.empty())
			continue;

		ns::parse(ctx);
		lexer.clear();

		std::cout << "\nend\n";
	}
}