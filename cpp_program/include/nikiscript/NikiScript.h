#pragma once

#include <string>

#include "Context.h"

namespace ns {
	/**
	 * @brief shows command usage
	 * @param ctx
	 * @note s[command?]
	 */
	void help_command(Context& ctx);

	/**
	 * @brief prints all the arguments passed
	 * @param ctx
	 * @note s[message]
	 */
	void echo_command(Context& ctx);

	/**
	 * @brief creates a variable
	 * @param ctx
	 * @note s[name] s[value]
	 */
	void var_command(Context& ctx);
	/**
	 * @brief deletes a variable
	 * @param ctx
	 * @note v[consoleVariable]
	 */
	void delvar_command(Context& ctx);

	/**
	 * @brief toggles a variable value between option1 and option2
	 * @param ctx
	 * @note v[variable] s[option1] s[option2]
	 */
	void toggle_command(Context& ctx);

	/**
	 * @brief parses a file as nikiscript
	 * @param ctx
	 * @note s[filePath]
	 */
	void exec_command(Context& ctx);

	/**
	 * @brief increments a variable value
	 * @param ctx
	 * @note v[variable] n[min] n[max] n[delta?]
	 */
	void incrementvar_command(Context& ctx);

	/**
	 * @brief adds default commands such as echo
	 * @param ctx
	 * @see echo_command
	 */
	void registerCommands(Context& ctx);

	/**
	 * @brief creates a variable and stores it in ns::Context::programVariables
	 * @param ctx
	 * @param name
	 * @param description
	 * @param pVar
	 * @param get
	 * @param set
	 */
	void registerVariable(Context& ctx, const std::string& name, const std::string& description, void* pVar, const GetProgramVariableValue& get, const SetProgramVariableValue& set);
}