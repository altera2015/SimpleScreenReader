#include "SmartSection.h"
#include "CommonFunctions.h"

using namespace Promixis;

SmartSection::SmartSection(json section)
{
	m_Valid = false;
	m_HasCondition = false;

	if (section.contains("name"))
	{
		m_Name = convertUTF8(section["name"].get<std::string>());
	}

	if (section.contains("condition"))
	{
		m_HasCondition = true;
		m_Condition = std::wregex(convertUTF8(section["condition"].get<std::string>()));

	}

	if (section.contains("find"))
	{
		m_Valid = true;
		m_Regexp = std::wregex(convertUTF8(section["find"].get<std::string>()));
	}

	if (section.contains("replace"))
	{
		m_Replace = convertUTF8(section["replace"].get<std::string>());
	}

	if (section.contains("active"))
	{
		setActive(section["active"].get<bool>());
	}
	else
	{
		setActive(true);
	}
}

std::wstring SmartSection::filter(const std::wstring& source)
{
	if (!m_Valid)
	{
		return source;
	}

	if (m_HasCondition)
	{
		std::wsmatch match;
		if (!std::regex_search(source, match, m_Condition))
		{
			return source;
		}		
	}

	return std::regex_replace(source, m_Regexp, m_Replace);
}

std::wstring SmartSection::name() const
{
	return m_Name;
}
