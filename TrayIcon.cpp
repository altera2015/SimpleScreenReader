#include "TrayIcon.h"

#include <Windows.h>
#include <shellapi.h>
#include "resource.h"


#define TRAY_MENU_SHOW  0x100
#define TRAY_MENU_EXIT  0x101

TrayIcon::TrayIcon(HINSTANCE hInstance, const std::wstring & tip)
{
    memset(&m_NotifyIconData, 0, sizeof(NOTIFYICONDATA));
    m_NotifyIconData.cbSize = sizeof(NOTIFYICONDATA);

    
    m_NotifyIconData.uID = 0x1000;

    m_NotifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

    m_NotifyIconData.uCallbackMessage = WM_TRAYICON;

    m_NotifyIconData.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

    wcscpy_s(m_NotifyIconData.szTip, tip.c_str());
   
    m_TrayMenu = CreatePopupMenu();
    AppendMenu(m_TrayMenu, MF_STRING, TRAY_MENU_SHOW, L"Show");
    AppendMenu(m_TrayMenu, MF_SEPARATOR, 0, 0);
    AppendMenu(m_TrayMenu, MF_STRING, TRAY_MENU_EXIT, L"Exit");

}


TrayIcon::~TrayIcon()
{
    DestroyMenu(m_TrayMenu);
}


void TrayIcon::add(HWND wnd)
{
    m_NotifyIconData.hWnd = wnd;
    Shell_NotifyIcon(NIM_ADD, &m_NotifyIconData);
}

void TrayIcon::remove()
{
    Shell_NotifyIcon(NIM_DELETE, &m_NotifyIconData);
}

void TrayIcon::onLeftClick(HWND wnd)
{
    ShowWindow(wnd, SW_SHOW);
}

void TrayIcon::onRightClick(HWND wnd)
{
    POINT curPoint;
    GetCursorPos(&curPoint);
    SetForegroundWindow(wnd);
    UINT clicked = TrackPopupMenu(

        m_TrayMenu,
        TPM_RIGHTBUTTON | TPM_RETURNCMD,
        curPoint.x,
        curPoint.y,
        0,
        wnd,
        NULL

        );

    PostMessage(wnd, WM_NULL, 0, 0);

    switch (clicked)
    {
    case TRAY_MENU_SHOW:
        ShowWindow(wnd, SW_SHOW);
        break;
    case TRAY_MENU_EXIT:
        DestroyWindow(wnd);
        break;
    }
}