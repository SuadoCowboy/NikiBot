#include "Commands.h"

#include <filesystem>
#include <sstream>
#include <fstream>

#include <nikiscript/PrintCallback.h>
#include <nikiscript/Parser.h>
#include <nikiscript/NikiScript.h>

bool isOwner = false;
std::string discord;

static bool isUserOwner() {
	if (!isOwner) {
		ns::print(ns::PrintLevel::ERROR, "You don't have permission to use this command");
		return false;
	}

	return true;
}

static bool isValidFileName(const std::string& name) {
	for (size_t i = 0; i < name.size(); ++i) {
		if (isspace(name[i]) || name[i] == '\\' || name[i] == '/' || (i+1 < name.size() && name[i] == '.' && name[i+1] == '.')) {
			ns::printf(ns::PrintLevel::ERROR, "Invalid name {}\n", name);
			return false;
		}
	}
	return true;
}

void registerCommands(ns::Context& ctx) {
	ns::registerCommands(ctx);
	ctx.commands.remove(ctx, "remove");
	ctx.commands.add(ns::Command("exec", 1,1, exec_command, "parses a script file", {"s[fileName]", "file to parse"}));
	ctx.commands.add(ns::Command("save", 1,1, save_command, "saves console variables in a file", {"s[fileName]", "file to store data"}));
	ctx.commands.add(ns::Command("vars", 0,0, vars_command, "prints out current stored console variables and their values", {}));
	ctx.commands.add(ns::Command("pvars", 0,0, pvars_command, "prints out current stored program variables and their values", {}));
	ctx.commands.add(ns::Command("cfgs", 0,0, cfgs_command, "prints out cfgs folder content", {}));
	ctx.commands.add(ns::Command("ex", 0,1, ex_command, "shows examples or run the chosen example", {"s[path?]", "path to example"}));

	ctx.commands.add(ns::Command("dc", 1,1, dc_command, "passes a string for the bot to run", {"s[text]", "text to pass as a command"}));
	ctx.commands.add(ns::Command("rcon", 1,1, rcon_command, "run system command", {"s[text]", "text to run on environment"}));
}

void exec_command(ns::Context& ctx) {
	std::string& path = ctx.args.getString(0);

	if (!isValidFileName(path))
		return;

	path = "./cfgs/"+path;
	ns::parseFile(ctx, path.c_str(), true);
}

void save_command(ns::Context& ctx) {
	if (!std::filesystem::exists("./cfgs"))
		std::filesystem::create_directory("./cfgs");

	std::string& path = ctx.args.getString(0);
	if (!isValidFileName(path))
		return;

	path = "./cfgs/"+path;
	if (!std::filesystem::path(path).has_extension())
		path += NIKISCRIPT_FILE_EXTENSION;

	ns::Context tempCtx = ns::copyContext(ctx);
	ns::parseFile(tempCtx, path.c_str(), false);

	std::ofstream file{path};
	for (auto& variable : ctx.consoleVariables) {
		if (tempCtx.consoleVariables.count(variable.first) != 0) {
			tempCtx.consoleVariables[variable.first] = variable.second;
			break;
		}

		file << "var (" << variable.first << "," << variable.second << ")\n";
	}

	for (auto& variable : tempCtx.consoleVariables)
		file << "var (" << variable.first << "," << variable.second << ")\n";

	ns::printf(ns::PrintLevel::ECHO, "Stored variables in \"{}\"", path);
}

void vars_command(ns::Context& ctx) {
	std::stringstream vars;
	for (auto& var : ctx.consoleVariables)
		vars << var.first << " = " << var.second << '\n';

	ns::printf(ns::PrintLevel::ECHO, vars.str().c_str());
}

void pvars_command(ns::Context& ctx) {
	std::stringstream vars;
	for (auto& var : ctx.programVariables)
		vars << var.first << " = " << var.second.get(ctx, &var.second) << '\n';

	ns::printf(ns::PrintLevel::ECHO, vars.str().c_str());
}

void cfgs_command(ns::Context& ctx) {
	if (!std::filesystem::is_directory("./cfgs")) {
		ns::print(ns::PrintLevel::ECHO, "cfgs folder is empty\n");
		return;
	}

	std::stringstream out;
	for (const auto& entry : std::filesystem::directory_iterator("./cfgs")) {
		if (entry.is_directory())
			continue;

        out << entry.path() << '\n';
	}

	ns::print(ns::PrintLevel::ECHO, out.str().c_str());
}

void ex_command(ns::Context& ctx) {
	if (!std::filesystem::is_directory("./examples")) {
		ns::print(ns::PrintLevel::ECHO, "Examples folder is empty\n");
		return;
	}

	std::stringstream out;
	if (ctx.args.arguments.size() == 0) {
		for (const auto& entry : std::filesystem::directory_iterator("./examples"))
			out << entry.path().filename() << '\n';

		ns::print(ns::PrintLevel::ECHO, out.str().c_str());
		return;
	}

	std::string& path = ctx.args.getString(0);
	if (!isValidFileName(path))
		return;

	path = "./examples/"+path;

	std::ifstream file{path};
	if (!file) {
		ns::printf(ns::PrintLevel::ERROR, "Could not load file \"{}\"\n", path);
		return;
	}

	out << "SCRIPT:\n";
	while (file.good()) {
		std::string line = "";
		std::getline(file, line);
		out << line;
	}
	out << '\n';

	ns::printf(ns::PrintLevel::ECHO, out.str().c_str());
	ns::parseFile(ctx, path.c_str(), true);
}


void dc_command(ns::Context& ctx) {
	discord += ctx.args.getString(0) + '\n';
}

void rcon_command(ns::Context& ctx) {
	if (!isUserOwner())
		return;

	system(ctx.args.getString(0).c_str());
}