#include "Context.h"

std::string& ns::Arguments::getString() {
	return arguments[offset++];
}

float ns::Arguments::getFloat() {
	return std::stof(arguments[offset++]);
}

double ns::Arguments::getDouble() {
	return std::stod(arguments[offset++]);
}

void ns::Arguments::clear() {
	arguments.clear();
	offset = 0;
}

ns::Context ns::copyContext(const Context& source) {
	Context copy{source};

	for (size_t i = 0; i < copy.loopVariablesRunning.size(); ++i)
		copy.loopVariablesRunning[i] = &*copy.consoleVariables.find(copy.loopVariablesRunning[i]->first);

	for (size_t i = 0; i < copy.toggleVariablesRunning.size(); ++i)
		copy.toggleVariablesRunning[i] = &*copy.consoleVariables.find(copy.toggleVariablesRunning[i]->first);

	for (size_t i = 0; i < copy.toggleCommandsRunning.size(); ++i)
		copy.toggleCommandsRunning[i] = &copy.commands.commands.find(copy.toggleCommandsRunning[i]->name)->second;

	return copy;
}

uint8_t operator|(ns::OriginType l, ns::OriginType r) {
	return static_cast<uint8_t>(l)|static_cast<uint8_t>(r);
}

uint8_t operator|(uint8_t l, ns::OriginType r) {
	return l|static_cast<uint8_t>(r);
}

uint8_t operator|(ns::OriginType l, uint8_t r) {
	return static_cast<uint8_t>(l)|r;
}

uint8_t& operator|=(uint8_t& l, ns::OriginType r) {
	l = l|r;
	return l;
}

uint8_t operator&(uint8_t l, ns::OriginType r) {
	return l&static_cast<uint8_t>(r);
}

uint8_t operator&(ns::OriginType l, uint8_t r) {
	return static_cast<uint8_t>(l)&r;
}

uint8_t& operator&=(uint8_t& l, ns::OriginType r) {
	l = l & r;
	return l;
}

uint8_t operator~(ns::OriginType l) {
	return ~static_cast<uint8_t>(l);
}