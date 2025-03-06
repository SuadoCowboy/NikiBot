#pragma once

#include <nikiscript/Context.h>

void registerCommands(ns::Context& ctx);

void exec_command(ns::Context& ctx);
void save_command(ns::Context& ctx);
void vars_command(ns::Context& ctx);
void pvars_command(ns::Context& ctx);
void scripts_command(ns::Context& ctx);