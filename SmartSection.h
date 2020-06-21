#pragma once

#include <string>
#include <regex>
#include "ISmartSection.h"

#include "json.hpp"
using json = nlohmann::json;

class SmartSection : public ISmartSection {
	bool m_Valid;
	std::wstring m_Name;

	bool m_HasCondition;
	std::wregex m_Condition;

	std::wregex m_Regexp;
	std::wstring m_Replace;
	
public:
	SmartSection(json section);
	virtual ~SmartSection() {};

	virtual std::wstring filter(const std::wstring& source);
	virtual std::wstring name() const;
};


