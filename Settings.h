#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <Windows.h>
#include <string>

class Settings
{
    std::wstring m_Voice;
    int m_Rate;
    std::wstring m_FilterLocation;

public:
    Settings();
    ~Settings();

    std::wstring voice() const;
    void setVoice(const std::wstring & voice );

    int speechRate() const;
    void setSpeechRate(int rate);

    bool getRunOnStartup() const;
    void setRunOnStartup(bool run);


    std::wstring filterLocation() const;

    bool load();
    bool save();
};

#endif
