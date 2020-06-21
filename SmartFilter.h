#pragma once

#include <list>
#include <memory>
#include <string>
#include "ISmartSection.h"

typedef std::unique_ptr<ISmartSection> SmartSectionPtr;

class SmartFilter
{
	std::list<SmartSectionPtr> m_Sections;
public:
	
	bool load(const std::wstring& config);
	std::wstring filter(const std::wstring & source);
};

