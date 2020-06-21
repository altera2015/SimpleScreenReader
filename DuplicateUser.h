#pragma once

#include "ISmartSection.h"
#include "json.hpp"
using json = nlohmann::json;

class DuplicateUser : public ISmartSection
{
	std::wstring m_Name;	
public:
	DuplicateUser(json section);
	virtual ~DuplicateUser() {};
	virtual std::wstring filter(const std::wstring& source);
	virtual std::wstring name() const;

};

