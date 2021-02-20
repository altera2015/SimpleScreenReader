#include "Settings.h"

#include <Windows.h>
#include <Shlobj.h>

#include "CommonFunctions.h"

static bool getRegistry(HKEY key, LPCWSTR value, std::wstring & data, const std::wstring & def = L"")
{
    DWORD size;
    if (RegGetValue(key, 0, value, RRF_RT_REG_SZ, 0, 0, &size) != ERROR_SUCCESS)
    {        
        int x = GetLastError();
        data = def;
        return false;
    }

    int wcharSize = size / sizeof(WCHAR);
    data.resize(wcharSize);

    if (RegGetValue(key, 0, value, RRF_RT_REG_SZ, 0, &data[0], &size) != ERROR_SUCCESS)
    {
        data = def;        
        return false;
    }

    if (data.compare(L"") == 0)
    {
        data = def;
        return true;
    }

    Promixis::trimNullString(data);
    
    return true;
}

static bool getRegistry(HKEY key, LPCWSTR value, int & data, int def = 0)
{
    DWORD size = sizeof(DWORD);
    DWORD d;
    

    if (RegGetValue(key, 0, value, RRF_RT_REG_DWORD, 0, &d, &size) != ERROR_SUCCESS)
    {
        data = def;
        return false;
    }
    data = d;
    return true;
}

static bool getRegistry(HKEY key, LPCWSTR value, FILETIME & data)
{
    DWORD size = sizeof(FILETIME);
    FILETIME d;


    if (RegGetValue(key, 0, value, RRF_RT_REG_BINARY, 0, &d, &size) != ERROR_SUCCESS)
    {
        return false;
    }
    data = d;
    return true;
}


static bool setRegistryKey(HKEY key, LPCWSTR value, const std::wstring & data)
{
    if (RegSetValueEx(key, value, 0, REG_SZ, (BYTE*)&data[0], ( data.length() ) * sizeof(WCHAR) ) != ERROR_SUCCESS)
    {        
        return false;
    }
    return true;
}

static bool setRegistryKey(HKEY key, LPCWSTR value, int data)
{
    DWORD d = data;
    if (RegSetValueEx(key, value, 0, REG_DWORD, (BYTE*)&d, sizeof(DWORD)) != ERROR_SUCCESS)
    {
        return false;
    }
    return true;
}


Settings::Settings() : 
    m_Rate(0),
    m_FilterLocation(getHomeDir() + L"\\smartFilter.json")
{
}


Settings::~Settings()
{
    if (!save())
    {
        OutputDebugString(L"Settings::~Settings / unable to save settings.");
    }
}


std::wstring Settings::voice() const
{
    return m_Voice;
}

void Settings::setVoice(const std::wstring & voice)
{
    m_Voice = voice;
}

std::wstring Settings::filterLocation() const
{
    return m_FilterLocation;
}

bool Settings::load()
{
    HKEY key;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Promixis\\SimpleReader", 0, KEY_READ, &key) != ERROR_SUCCESS)
    {
        return false;
    }

    getRegistry(key, L"voice", m_Voice);
    getRegistry(key, L"rate", m_Rate);    
    getRegistry(key, L"filterLocation", m_FilterLocation);

    RegCloseKey(key);
    return true;
}

bool Settings::save()
{
    HKEY key;

    if (RegCreateKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Promixis\\SimpleReader", 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, 0, &key, 0) != ERROR_SUCCESS)
    {
        return false;
    }

    setRegistryKey(key, L"voice", m_Voice);
    setRegistryKey(key, L"rate", m_Rate);
    setRegistryKey(key, L"filterLocation", m_FilterLocation);
    RegCloseKey(key);
    return true;
}

std::wstring Settings::getHomeDir() const
{
    WCHAR path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path))) {
        return std::wstring(path);
    }
    return std::wstring();
}



int Settings::speechRate() const
{
    return m_Rate;
}

void Settings::setSpeechRate(int rate)
{
    m_Rate = rate;
}

bool Settings::getRunOnStartup() const
{
    HKEY key;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &key) != ERROR_SUCCESS)
    {
        return false;
    }

    std::wstring path;
    bool found = getRegistry(key, L"SimpleReader", path);

    RegCloseKey(key);
    return found;
}

void Settings::setRunOnStartup(bool run)
{
    HKEY key;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ|KEY_WRITE, &key) != ERROR_SUCCESS)
    {
        return;
    }

    if (run)
    {
        WCHAR szFileName[MAX_PATH];
        GetModuleFileName(NULL, szFileName, MAX_PATH);
        std::wstring startup = L"\"";
        startup += std::wstring(szFileName);
        startup += L"\" --hide";

        setRegistryKey(key, L"SimpleReader", startup.c_str());
    }
    else
    {
        RegDeleteKeyValue(key, 0, L"SimpleReader");
    }

    RegCloseKey(key);
}

