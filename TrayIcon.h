#ifndef __TRAYICON_H__
#define __TRAYICON_H__

#include <Windows.h>
#include <shellapi.h>
#include <string>

#define WM_TRAYICON WM_APP + 1

class TrayIcon
{
    NOTIFYICONDATA m_NotifyIconData;
    HMENU m_TrayMenu;
public:
    TrayIcon(HINSTANCE hInstance, const std::wstring & tip);
    ~TrayIcon();

    void add(HWND wnd);
    void remove();

    void onRightClick(HWND wnd);
    void onLeftClick(HWND wnd);

};


#endif

