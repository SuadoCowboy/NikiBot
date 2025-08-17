#pragma once

#include <string>

#include <nikiscript/Context.h>

extern bool isOwner; ///< if the current input is from one of the owners
extern std::string discord; ///< string for the bot to parse

void registerCommands(ns::Context* pCtx);

void exec_command(ns::Context* pCtx, void*);
void save_command(ns::Context* pCtx, void*);
void vars_command(ns::Context* pCtx, void*);
void pvars_command(ns::Context* pCtx, void*);
void cfgs_command(ns::Context* pCtx, void*);
void ex_command(ns::Context* pCtx, void*); // ex = example

void dc_command(ns::Context* pCtx, void*);
void rcon_command(ns::Context* pCtx, void*);