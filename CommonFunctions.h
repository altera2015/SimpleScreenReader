#ifndef __COMMON_FUNCTIONS_H__
#define __COMMON_FUNCTIONS_H__

#include <string>
#include <Windows.h>
#include "CommonTypes.h"

namespace Promixis {

    void trimNullString(std::wstring & str);
    void trimString(std::wstring & str);
    bool getDlgString(HWND wnd, int dlgItem, std::wstring & text);
    bool getWndString(HWND wnd, std::wstring & text);
    bool setDlgString(HWND wnd, int dlgItem, const std::wstring & text);
    bool setWndString(HWND wnd, const std::wstring & text);

    bool setComboItems(HWND control, const wstringlist & items);
    bool setComboItems(HWND wnd, int dlgItem, const wstringlist & items);
    bool setComboIndex(HWND control, int index);
    bool setComboIndex(HWND wnd, int dlgItem, int index);

    int wstringlistFind(const wstringlist & haystack, const std::wstring & needle);

    // bool setClipboardData(const std::wstring & text);
    bool getClipboardData(std::wstring & text);
    bool invokeControlCopy();

    void debugStr(const std::wstring& dbg);
    void debugStr(const std::string& dbg);

    std::wstring convertUTF8(const std::string& input);
}

#endif
