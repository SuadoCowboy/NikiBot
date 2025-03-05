#include <iostream>
#include <fstream>
#include <filesystem>

#include <nikiscript/PrintCallback.h>
#include <nikiscript/Context.h>
#include <nikiscript/Parser.h>
#include <nikiscript/NikiScript.h>

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

bool isValidFileName(const std::string& name) {
	for (size_t i = 0; i < name.size(); ++i) {
		if (isspace(name[i]) || name[i] == '\\' || name[i] == '/' || (i+1 < name.size() && name[i] == '.' && name[i+1] == '.')) {
			ns::printf(ns::PrintLevel::ERROR, "Invalid name {}\n", name);
			return false;
		}
	}
	return true;
}

void exec_command(ns::Context& ctx) {
	std::string& path = ctx.arguments.getString();

	if (!isValidFileName(path))
		return;

	path = "./scripts/"+path;
	ns::parseFile(ctx, path.c_str(), true);
}

void save_command(ns::Context& ctx) {
	if (!std::filesystem::is_directory("./scripts"))
		std::filesystem::create_directory("./scripts");

	std::string& path = ctx.arguments.getString();
	if (!isValidFileName(path))
		return;

	path = "./scripts/"+path;

	std::ofstream file{path, std::ios_base::app};
	for (auto& variable : ctx.consoleVariables) {
		file << "var (" << variable.first << "," << variable.second << ")\n";
	}

	ns::printf(ns::PrintLevel::ECHO, "Stored variables in \"{}\"", path);
}

void vars_command(ns::Context& ctx) {
	std::stringstream vars;
	for (auto& var : ctx.consoleVariables)
		vars << var.first << " = " << var.second << '\n';

	ns::printf(ns::PrintLevel::ECHO, vars.str());
}

void pvars_command(ns::Context& ctx) {
	std::stringstream vars;
	for (auto& var : ctx.programVariables)
		vars << var.first << " = " << var.second.get(&var.second) << '\n';

	ns::printf(ns::PrintLevel::ECHO, vars.str());
}

int main() {
	ns::setPrintCallback(nullptr, nikiscriptPrintCallback);
	ns::maxConsoleVariableCalls = 10;

	ns::Context ctx;
	ns::registerCommands(ctx);
	ctx.commands.remove("exec", ctx);
	ctx.commands.add(ns::Command("exec", 1,1, exec_command, "parses a script file", {"s[fileName]", "file to parse"}));
	ctx.commands.add(ns::Command("save", 1,1, save_command, "saves console variables in a file", {"s[fileName]", "file to store data"}));
	ctx.commands.add(ns::Command("vars", 0,0, vars_command, "prints out current stored console variables and their values", {}));
	ctx.commands.add(ns::Command("pvars", 0,0, pvars_command, "prints out current stored program variables and their values", {}));

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