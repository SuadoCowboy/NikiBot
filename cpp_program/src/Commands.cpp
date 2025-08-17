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
		ns::print(ns::ERROR, "You don't have permission to use this command");
		return false;
	}

	return true;
}

/**
 * @brief whether it's trying to go back a dir with '..' or using '/' or '\\'
 */
static bool isSafeFileName(const std::string& name) {
	for (size_t i = 0; i < name.size(); ++i) {
		if (isspace(name[i]) || name[i] == '\\' || name[i] == '/' || (i+1 < name.size() && name[i] == '.' && name[i+1] == '.')) {
			ns::printf(ns::ERROR, "Invalid path \"{}\"\n", name);
			return false;
		}
	}
	return true;
}

void registerCommands(ns::Context* pCtx) {
	ns::registerCommands(pCtx);
	pCtx->commands.remove(pCtx, "exec");

	nsRegisterCommand(pCtx, "exec", 1,1, exec_command, "parses a script file", "s[fileName]", "file to parse");
	nsRegisterCommand(pCtx, "save", 1,1, save_command, "saves console variables in a file", "s[fileName]", "file to store data");
	nsRegisterCommand(pCtx, "vars", 0,0, vars_command, "prints out current stored console variables and their values");
	nsRegisterCommand(pCtx, "pvars", 0,0, pvars_command, "prints out current stored program variables and their values");
	nsRegisterCommand(pCtx, "cfgs", 0,0, cfgs_command, "prints out cfgs folder content");
	nsRegisterCommand(pCtx, "ex", 0,1, ex_command, "shows examples or run the chosen example", "s[path?]", "path to example");
	nsRegisterCommand(pCtx, "dc", 1,1, dc_command, "passes a string for the bot to run", "s[text]", "text to pass as a command");
	nsRegisterCommand(pCtx, "rcon", 1,1, rcon_command, "run system command", "s[text]", "text to run on environment");
}

void exec_command(ns::Context* pCtx, void*) {
	std::string& path = pCtx->args.getString(0);

	if (!isSafeFileName(path))
		return;

	ns::parseFile(pCtx, path.c_str(), true);
}

void save_command(ns::Context* pCtx, void*) {
	std::filesystem::path cfgPath(pCtx->cfgDirectory);
	if (!std::filesystem::exists("./"/cfgPath))
		std::filesystem::create_directory("./"/cfgPath);

	const std::string& path = pCtx->args.getString(0);
	if (!isSafeFileName(path))
		return;

	ns::Context tempCtx = ns::deepCopyContext(pCtx);
	ns::parseFile(&tempCtx, path.c_str(), false);

	std::ofstream file{path};
	for (auto& variable : pCtx->consoleVariables) {
		if (tempCtx.consoleVariables.count(variable.first) != 0) {
			tempCtx.consoleVariables[variable.first] = variable.second;
			break;
		}

		file << "var (" << variable.first << "," << variable.second << ")\n";
	}

	for (auto& variable : tempCtx.consoleVariables)
		file << "var (" << variable.first << "," << variable.second << ")\n";

	ns::printf(ns::ECHO, "Stored variables in \"{}\"", path);
}

void vars_command(ns::Context* pCtx, void*) {
	std::stringstream vars;
	for (auto& var : pCtx->consoleVariables)
		vars << var.first << " = " << var.second << '\n';

	ns::printf(ns::ECHO, vars.str().c_str());
}

void pvars_command(ns::Context* pCtx, void*) {
	std::stringstream vars;
	for (auto& var : pCtx->programVariables)
		vars << var.first << " = " << var.second.get(pCtx, &var.second) << '\n';

	ns::printf(ns::ECHO, vars.str().c_str());
}

void cfgs_command(ns::Context* pCtx, void*) {
	if (!std::filesystem::is_directory("./cfgs")) {
		ns::print(ns::ECHO, "cfgs folder is empty\n");
		return;
	}

	std::stringstream out;
	for (const auto& entry : std::filesystem::directory_iterator("./cfgs")) {
		if (entry.is_directory())
			continue;

        out << entry.path() << '\n';
	}

	ns::print(ns::ECHO, out.str().c_str());
}

void ex_command(ns::Context* pCtx, void*) {
	std::filesystem::path cfgPath(pCtx->cfgDirectory);
	if (!std::filesystem::is_directory(cfgPath/"examples")) {
		ns::print(ns::ECHO, "Examples folder is empty\n");
		return;
	}

	std::stringstream out;
	if (pCtx->args.arguments.size() == 0) {
		for (const auto& entry : std::filesystem::directory_iterator(cfgPath/"examples"))
			out << entry.path().filename() << '\n';

		ns::print(ns::ECHO, out.str().c_str());
		return;
	}

	std::string& path = pCtx->args.getString(0);
	if (std::filesystem::path(path).extension().string() != "cfg")
		path += ".cfg";

	if (!isSafeFileName(path))
		return;
	path = "examples/"+path;
	
	std::ifstream file{cfgPath/path};
	if (!file) {
		ns::printf(ns::ERROR, "Could not load file \"{}\"\n", path);
		return;
	}

	out << "SCRIPT:\n";
	while (file.good()) {
		std::string line = "";
		std::getline(file, line);
		out << line << '\n';
	}
	out << '\n';

	ns::printf(ns::ECHO, out.str().c_str());
	ns::parseFile(pCtx, path.c_str(), true);
}


void dc_command(ns::Context* pCtx, void*) {
	discord += pCtx->args.getString(0) + '\n';
}

void rcon_command(ns::Context* pCtx, void*) {
	if (!isUserOwner())
		return;

	[[discard]]system(pCtx->args.getString(0).c_str());
}