#include "ProgramVariable.h"

#include "PrintCallback.h"
#include "Context.h"

ns::ProgramVariable::ProgramVariable() {}

ns::ProgramVariable::ProgramVariable(void* pVariable, const GetProgramVariableValue& get, const SetProgramVariableValue& set)
 : pValue(pVariable), get(get), set(set) {}

std::string ns::getString(ProgramVariable* pVar) {
	return *static_cast<std::string*>(pVar->pValue);
}

void ns::setString(ProgramVariable* pVar, const std::string& str) {
	*static_cast<std::string*>(pVar->pValue) = str;
}