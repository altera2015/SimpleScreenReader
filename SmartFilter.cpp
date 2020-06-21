#include "SmartFilter.h"
#include "SmartSection.h"
#include "DuplicateUser.h"
#include <fstream>
#include "CommonFunctions.h"

#include "json.hpp"

using namespace Promixis;
using namespace nlohmann;


bool SmartFilter::load(const std::wstring& config)
{
	m_Sections.clear();
	std::ifstream ifs(config, std::ios::binary);
	if (!ifs.is_open())
	{
		debugStr(L"Failed to open config file" + config + L"\n");
		return false;
	}

	json c;

	try
	{
		c = json::parse(ifs);
	}
	catch (nlohmann::json::parse_error pe)
	{
		debugStr(L"Failed to parse :" + config + L"\n");
		debugStr(std::string(pe.what()) + "\n");
		return false;
	}

	if (c.contains("filters") && c["filters"].is_array())
	{
		for (auto& filter : c["filters"]) {

			if (filter.contains("class") )
			{
				std::string cls = filter["class"].get<std::string>();
				debugStr("Got " + cls);
				if (cls == "DuplicateUser")
				{
					m_Sections.push_back(std::make_unique<DuplicateUser>(filter));
					continue;
				}
			}
						
			m_Sections.push_back(std::make_unique<SmartSection>(filter));
			
		}
	}
	return true;
}

std::wstring SmartFilter::filter(const std::wstring& source)
{
	std::wstring filtered = source;


	/* std::wstring::size_type pos = 0;
	while ((pos = filtered.find(L"\r\n", pos)) != std::wstring::npos)
	{
		filtered.replace(pos, 2, L"\n");
	}*/

	debugStr(source + L"\n");
	for (auto& section : m_Sections) 
	{		
		if (section->isActive())
		{
			debugStr(section->name() + L"\n");
			filtered = section->filter(filtered);
			debugStr(filtered + L"\n");
		}
	}
	return filtered;
}


