#include "DuplicateUser.h"
#include "CommonFunctions.h"
#include <array>

using namespace Promixis;

DuplicateUser::DuplicateUser(json section)
{
    debugStr("DuplicateUser");
	if (section.contains("name"))
	{
		m_Name = convertUTF8(section["name"].get<std::string>());
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

std::wstring DuplicateUser::filter(const std::wstring& source)
{    
    std::vector<std::wstring> strings;
    std::wstring delimiter = L"\r\n";
    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = source.find(delimiter, prev)) != std::string::npos)
    {
        strings.push_back(source.substr(prev, pos - prev));
        prev = pos + delimiter.size();
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.push_back(source.substr(prev));
    
    if (strings.size() < 2)
    {
        return source;
    }

    int cnt = 0;
    for (size_t i=0; i < strings.size()-1; i++)
    {
        if (strings[i].length() > 0 && strings[i + 1].find(strings[i]) == 0)
        {
            strings[i] = L"";
            cnt++;
        }
    }

    if ( cnt == 0 )
    {
        return source;
    }

    std::wstring result;
    for (std::wstring & s : strings)
    {
        result += s + L"\r\n";
    }

    return result;
    // return strings;
}

std::wstring DuplicateUser::name() const
{
	return m_Name;
}
