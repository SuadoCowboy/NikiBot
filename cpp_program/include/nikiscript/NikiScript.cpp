#include "NikiScript.h"

#include <sstream>

#include "Utils.h"
#include "Parser.h"
#include "Lexer.h"

void ns::help_command(Context& ctx) {
	if (ctx.arguments.arguments.size() == 0) {
		std::stringstream oss;
		for (auto& command : ctx.commands.commands)
			oss << command.second.name << ' ' << command.second.getArgumentsNames() << '\n';

		ns::print(ns::PrintLevel::ECHO, oss.str());

	} else {
		std::string& commandName = ctx.arguments.getString();
		trim(commandName);

		Command* pCommand = ctx.commands.get(commandName);
		if (pCommand == nullptr) {
			ns::printf(ns::PrintLevel::ERROR, "Command \"{}\" not found\n", commandName);
			return;
		}

		pCommand->printAsDataTree();
	}
}

void ns::echo_command(Context& ctx) {
	ns::printf(ns::PrintLevel::ECHO, "{}\n", ctx.arguments.getString());
}

void ns::var_command(Context& ctx) {
	std::string& name = ctx.arguments.getString();

	if (name.empty()) {
		ns::print(PrintLevel::ERROR, "Variable name can not be empty\n");
		return;
	}

	for (size_t i = 0; i < name.size(); ++i) {
		if (isspace(name[i])) {
			ns::print(PrintLevel::ERROR, "Variable name can not contain whitespace\n");
			return;
		}

		switch (name[i]) {
		case NIKISCRIPT_LOOP_VARIABLE:
			if (i == 0 && name.size() > 1)
				break;
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name alone or after the first character\n", NIKISCRIPT_LOOP_VARIABLE);
			return;

		case NIKISCRIPT_TOGGLE_ON:
			if (i == 0 && name.size() > 1)
				break;
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name alone or after the first character\n", NIKISCRIPT_TOGGLE_ON);
			return;

		case NIKISCRIPT_TOGGLE_OFF:
			if (i == 0 && name.size() > 1)
				break;
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name alone or after the first character\n", NIKISCRIPT_TOGGLE_OFF);
			return;

		case NIKISCRIPT_REFERENCE:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_REFERENCE);
			return;

		case NIKISCRIPT_REFERENCE_OPEN:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_REFERENCE_OPEN);
			return;

		case NIKISCRIPT_REFERENCE_CLOSE:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_REFERENCE_CLOSE);
			return;

		case NIKISCRIPT_ARGUMENTS_SEPARATOR:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_ARGUMENTS_SEPARATOR);
			return;

		case NIKISCRIPT_ARGUMENTS_CLOSE:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_ARGUMENTS_CLOSE);
			return;

		case NIKISCRIPT_ARGUMENTS_OPEN:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_ARGUMENTS_OPEN);
			return;

		case NIKISCRIPT_STATEMENT_SEPARATOR:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_STATEMENT_SEPARATOR);
			return;

		case NIKISCRIPT_COMMENT_LINE:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_COMMENT_LINE);
			return;

		case NIKISCRIPT_COMMENT_LINES:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_COMMENT_LINES);
			return;
		}
	}

	if (ctx.programVariables.count(name) != 0) {
		ns::print(PrintLevel::ERROR, "A program variable already uses this name and therefore can not be replaced\n");
		return;
	}

	if (ctx.commands.commands.count(name) != 0) {
		ns::print(PrintLevel::ERROR, "A command already uses this name and therefore can not be replaced\n");
		return;
	}

	if (ctx.arguments.arguments.size() == 1)
		ctx.consoleVariables[name] = "";
	else
		ctx.consoleVariables[name] = ctx.arguments.getString();
}

void ns::delvar_command(Context& ctx) {
	const std::string& varName = ctx.arguments.getString();

	if (ctx.consoleVariables.count(varName) == 0) {
		ns::printf(PrintLevel::ERROR, "Expected console variable\n");
		return;
	}

	for (size_t i = 0; i < ctx.loopVariablesRunning.size(); ++i) {
		if (ctx.loopVariablesRunning[i]->first == varName) {
			ctx.loopVariablesRunning.erase(ctx.loopVariablesRunning.begin()+i);
			break;
		}
	}

	if (varName.size() > 1 && varName[0] == '+') {
		std::string toggleVarName = varName.substr(1);

		for (size_t i = 0; i < ctx.toggleVariablesRunning.size(); ++i) {
			if (ctx.toggleVariablesRunning[i]->first == toggleVarName) {
				ctx.toggleVariablesRunning.erase(ctx.toggleVariablesRunning.begin()+i);
				break;
			}
		}
	}

	ctx.consoleVariables.erase(varName);
}

void ns::toggle_command(ns::Context& ctx) {
	const std::string& varName = ctx.arguments.getString();
	const std::string& option1 = ctx.arguments.getString();
	const std::string& option2 = ctx.arguments.getString();
	ctx.arguments.clear();

	if (ctx.consoleVariables.count(varName) != 0) {
		std::string& varValue = ctx.consoleVariables[varName];

		if (varValue == option1)
			varValue = option2;
		else
			varValue = option1;

		return;
	}

	ns::ProgramVariable& var = ctx.programVariables[varName];
	std::string varValue = var.get(&var);

	if (varValue == option1)
		var.set(&var, option2);
	else
		var.set(&var, option1);
}

void ns::exec_command(Context& ctx) {
	parseFile(ctx, ctx.arguments.getString().c_str(), true);
}

void ns::registerCommands(ns::Context& ctx) {
	ctx.commands.add(Command("echo", 1, 1, echo_command, "prints the passed message to console", {"s[message]", "content to print to console"}));
	ctx.commands.add(Command("help", 0,1, help_command, "prints a list of commands with their usages or the usage of the specified command", {"s[command?]", "command to see usage"}));
	ctx.commands.add(Command("var", 1,2, var_command, "creates a variable", {"s[name]", "variable name", "s[value?]", "if value is not specified, variable becomes an empty string"}));
	ctx.commands.add(Command("delvar", 1,1, delvar_command, "deletes a variable", {"v[variable]", "variable to delete"}));
	ctx.commands.add(Command("toggle", 3,3, toggle_command, "toggles value between option1 and option2", {"v[variable]", "variable to modify value", "s[option1]", "option to set variable in case variable value is option2", "s[option2]", "option to set variable in case variable value is option1"}));
	ctx.commands.add(Command("exec", 1,1, exec_command, "parses a script file", {"s[filePath]", "path to the file"}));
}

void ns::registerVariable(ns::Context& ctx, const std::string& name, void* pVar, const GetProgramVariableValue& get, const SetProgramVariableValue& set) {
	ctx.programVariables[name] = ProgramVariable(pVar, get, set);
}