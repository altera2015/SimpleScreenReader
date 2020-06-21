#ifndef __TEXT_TO_SPEECH_H__
#define __TEXT_TO_SPEECH_H__


#include <sapi.h>
#include <sphelper.h>
#include "CommonTypes.h"


class TextToSpeech
{
    wstringlist m_Voices;
    std::wstring m_CurrentVoiceName;
    CComPtr<ISpVoice> m_Voice;
    bool m_Paused;

public:
    TextToSpeech();
    ~TextToSpeech();

    bool hasTTS() const;

    bool setVoice(const std::wstring & voice);
    bool speak(const std::wstring & text);
    bool setVolume(int volume);
    bool setOutput(const std::wstring & output);
    wstringlist voices() const;
    wstringlist outputs() const;
    void setRate(int rate);
    int getRate() const;

    void pause();
    void resume();
    bool isPaused() const;





    
};


#endif
