#pragma once

#include <string>
#include <unordered_map>

namespace ns {
	struct ProgramVariable;
	
	typedef std::string(*GetProgramVariableValue)(ProgramVariable* pVar);
	typedef void(*SetProgramVariableValue)(ProgramVariable* pVar, const std::string& str);
	
	struct Context;

	struct ProgramVariable {
		void* pValue = nullptr;

		GetProgramVariableValue get = nullptr;
		SetProgramVariableValue set = nullptr;

		ProgramVariable();
		ProgramVariable(void* pVariable, const GetProgramVariableValue& get, const SetProgramVariableValue& set);
	};

	typedef std::unordered_map<std::string, ProgramVariable> ProgramVariables;

	std::string getString(ProgramVariable* pVar);
	void setString(ProgramVariable* pVar, const std::string& str);
}