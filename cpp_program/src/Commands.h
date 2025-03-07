#pragma once

#include <string>

#include <nikiscript/Context.h>

extern bool isOwner; ///< if the current input is from one of the owners
extern std::string discord; ///< string for the bot to parse

void registerCommands(ns::Context& ctx);

void exec_command(ns::Context& ctx);
void save_command(ns::Context& ctx);
void vars_command(ns::Context& ctx);
void pvars_command(ns::Context& ctx);
void scripts_command(ns::Context& ctx);
void dc_command(ns::Context& ctx);