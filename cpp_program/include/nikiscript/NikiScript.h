#pragma once

#include "Context.h"

namespace ns {
	/**
	 * @brief shows command usage
	 * @param command s[command?]
	 */
	void help_command(Context& ctx);

	/**
	 * @brief prints all the arguments passed
	 * @param message s[message]
	 */
	void echo_command(Context& ctx);

	void var_command(Context& ctx);
	void delvar_command(Context& ctx);

    void math_command(Context& ctx);

	void toggle_command(Context& ctx);

	void exec_command(Context& ctx);

	/**
	 * @brief adds default commands such as echo
	 * @param ctx 
	 * @see echo_command
	 */
	void registerCommands(Context& ctx);

	void registerVariable(Context& ctx, const std::string& name, void* pVar, const GetProgramVariableValue& get, const SetProgramVariableValue& set);
}