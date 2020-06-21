#include "TextToSpeech.h"


TextToSpeech::TextToSpeech() : m_Paused(false)
{
    HRESULT res2 = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(res2))
    {
        m_Voice = 0;
        OutputDebugString(L"TextToSpeech::TextToSpeech / Unable to initialize COM system.\n");
        return;
    }

    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&m_Voice);
    if (!SUCCEEDED(hr))
    {
        m_Voice = 0;
        OutputDebugString(L"TextToSpeech::TextToSpeech / Could not initialize voice interface\n");
        return;
    }

    CComPtr<IEnumSpObjectTokens> voices;
    hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &voices);
    if (!SUCCEEDED(hr))
    {
        OutputDebugString(L"TextToSpeech::TextToSpeech / Could not enumerrate voices\n");
        return;
    }

    ULONG ulCount = 0;
    hr = voices->GetCount(&ulCount);
    if (!SUCCEEDED(hr))
    {
        OutputDebugString(L"TextToSpeech::TextToSpeech / Could not enumerrate voices 2\n");
        return;
    }

    CComPtr<ISpObjectToken> cpVoiceToken;
    for (unsigned int i = 0; i<ulCount; i++)
    {
        hr = voices->Next(1, &cpVoiceToken, NULL);
        WCHAR * name;
        SpGetDescription(cpVoiceToken, &name);
        std::wstring stringName(name);
        m_Voices.push_back(stringName);
        cpVoiceToken.Release();
    }    
}


TextToSpeech::~TextToSpeech()
{
    m_Voice.Release();    
    CoUninitialize();
}

bool TextToSpeech::hasTTS() const
{
    return m_Voice != 0;
}

bool TextToSpeech::setVoice(const std::wstring & voice)
{
    CComPtr<IEnumSpObjectTokens> voices;
    HRESULT hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &voices);
    if (!SUCCEEDED(hr))
    {        
        return false;
    }

    ULONG ulCount = 0;
    hr = voices->GetCount(&ulCount);
    if (!SUCCEEDED(hr))
    {        
        return false;
    }

    CComPtr<ISpObjectToken> cpVoiceToken;
    for (unsigned int i = 0; i<ulCount; i++)
    {
        hr = voices->Next(1, &cpVoiceToken, NULL);
        WCHAR * _name;
        SpGetDescription(cpVoiceToken, &_name);
        // QString name = QString::fromWCharArray(_name);
        std::wstring nameString(_name);

        if (nameString == voice)
        {
            if (SUCCEEDED(m_Voice->SetVoice(cpVoiceToken)))
            {
                m_CurrentVoiceName = voice;
                return true;
            }
            else
            {                
                return false;
            }
        }
        cpVoiceToken.Release();
    }
    return false;
}

bool TextToSpeech::speak(const std::wstring & text)
{
    if (!m_Voice)
    {
        return false;
    }
    
    m_Voice->Speak(text.data(), SPF_ASYNC | SPF_PURGEBEFORESPEAK, 0);
    resume();

    return false;
}

bool TextToSpeech::setVolume(int volume)
{
    return false;
}

bool TextToSpeech::setOutput(const std::wstring & output)
{
    return false;
}

wstringlist TextToSpeech::voices() const
{
    return m_Voices;
}

wstringlist TextToSpeech::outputs() const
{
    return wstringlist();
}

void TextToSpeech::setRate(int rate)
{
    m_Voice->SetRate(rate);
}

int TextToSpeech::getRate() const
{
    long rate = 0;
    m_Voice->GetRate(&rate);
    return rate;
}

void TextToSpeech::pause()
{
    if (m_Paused)
    {
        return;
    }

    m_Voice->Pause();
    m_Paused = true;
}

void TextToSpeech::resume()
{
    if (!m_Paused)
    {
        return;
    }

    m_Voice->Resume();    
    m_Paused = false;
}

bool TextToSpeech::isPaused() const
{
    return m_Paused;
}
