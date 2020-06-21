#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <Windows.h>
#include <Commctrl.h>

#include "TextToSpeech.h"
#include "TrayIcon.h"

#define APPLICATION_TITLE   L"Simple Screen Reader"

#include "SmartFilter.h"

class MainWindow
{
    HWND m_Wnd;    
    HWND m_VoiceComboWnd;
    HWND m_RateWnd;
    HWND m_ToolbarWnd;
    HWND m_RebarWnd;
    
    HIMAGELIST m_ImageList;

    TextToSpeech * m_TTS;
    TrayIcon * m_TrayIcon;
    SmartFilter m_SmartFilter;

    HWND createVoiceCombo(HWND parent);
    HWND createRateCombo(HWND parent);
    HWND createToolbar(HWND parent);
    HWND createRebar(HWND parent);
    
    BOOL initDialog(HWND wnd);
    BOOL onDestroy(HWND wnd);
    
    BOOL onSpeakClicked(HWND wnd);
    BOOL onSpeakSmartClicked(HWND wnd);
	BOOL onRefreshSpeakSmart(HWND wnd);
    BOOL onHotkeySpeak(HWND wnd);
    BOOL onHotkeySpeakSmart(HWND wnd);
    BOOL onPauseToggle(HWND wnd);
    BOOL onVoiceComboMessage(HWND wnd, WPARAM wParam, LPARAM lParam);
    BOOL onSpeechRateComboMessage(HWND wnd, WPARAM wParam, LPARAM lParam);
    BOOL onCloseWindow(HWND wnd);
    BOOL onHelpAbout(HWND wnd);
    BOOL toggleStartWithWindows(HWND wnd);
    BOOL onResize(HWND wnd, WPARAM wParam, LPARAM lParam);

    static BOOL CALLBACK staticWndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
    BOOL wndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);


    
public:
    MainWindow(LPSTR cmdLine);
    ~MainWindow();
    HWND wnd() const;
    void setCaption(const std::wstring & caption);
};


#endif
