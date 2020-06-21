#include "CommonFunctions.h"
#include <Windows.h>

void Promixis::trimNullString(std::wstring & str)
{
    while ( str.length() > 0 && str.at(str.length() - 1) == 0)
    {
        str.pop_back();
    }
}

void Promixis::trimString(std::wstring & str)
{
    trimNullString(str);
    if (str.length() == 0)
    {
        return;
    }
    size_t first = str.find_first_not_of(L" \r\n\t");
    size_t last = str.find_last_not_of(L" \r\n\t");
    str =  str.substr(first, (last - first + 1));       
}

bool Promixis::getWndString(HWND wnd, std::wstring & text)
{
    text.clear();    
    int len = SendMessage(wnd, WM_GETTEXTLENGTH, 0, 0);
    if (len < 0)
    {
        return false;
    }

    text.resize(len + 1);    
    SendMessage(wnd, WM_GETTEXT, len + 1 , (LPARAM)&text[0]);
   
    return true;
}

bool Promixis::getDlgString(HWND wnd, int dlgItem, std::wstring & text)
{
    HWND control = GetDlgItem(wnd, dlgItem);
    if (control == NULL)
    {
        return false;
    }        
    return getWndString(control, text);
}


bool Promixis::setWndString(HWND wnd, const std::wstring & text)
{
    SendMessage(wnd, WM_SETTEXT, 0, (LPARAM)text.c_str());
    return true;
}


bool Promixis::setDlgString(HWND wnd, int dlgItem, const std::wstring & text)
{
    HWND control = GetDlgItem(wnd, dlgItem);
    if (control == NULL)
    {
        return false;
    }
    return setWndString(control, text);
}

bool Promixis::setComboItems(HWND control, const wstringlist & items)
{
    for (wstringlist::const_iterator ci = items.cbegin(); ci != items.cend(); ci++)
    {
        SendMessage(control, CB_ADDSTRING, 0, (LPARAM)(*ci).c_str());
    }
    return true;
}

bool Promixis::setComboItems(HWND wnd, int dlgItem, const wstringlist & items)
{
    HWND control = GetDlgItem(wnd, dlgItem);
    if (control == NULL)
    {
        return false;
    }


    return setComboItems(control, items);
}

bool Promixis::setComboIndex(HWND control, int index)
{
    SendMessage(control, CB_SETCURSEL, index, 0);
    return true;
}

bool Promixis::setComboIndex(HWND wnd, int dlgItem, int index)
{
    HWND control = GetDlgItem(wnd, dlgItem);
    if (control == NULL)
    {
        return false;
    }
    return setComboIndex(control, index);
}


int Promixis::wstringlistFind(const wstringlist & haystack, const std::wstring & needle)
{
    for (unsigned int i = 0; i < haystack.size(); i++)
    {
        if (haystack.at(i).compare(needle)==0)
        {
            return i;
        }
    }
    return -1;
}

/*
bool Promixis::setClipboardData(const std::wstring & text)
{
    HGLOBAL hdst = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (text.length() + 1) * sizeof(WCHAR));
    LPWSTR dst = (LPWSTR)GlobalLock(hdst);
    memcpy(dst, text.c_str(), text.length() * sizeof(WCHAR));
    dst[text.length()] = 0;
    GlobalUnlock(hdst);

    // Set clipboard data
    if (!OpenClipboard(NULL))
    {
        return false;
    }

    EmptyClipboard();
    if (!SetClipboardData(CF_UNICODETEXT, hdst))
    {
        return false;
    }
    CloseClipboard();
}*/

bool Promixis::getClipboardData(std::wstring & text)
{
    if (!OpenClipboard(NULL))
    {
        OutputDebugString(L"getClipboardData 1"); 
        return false;
    }

    HGLOBAL hglb = GetClipboardData(CF_UNICODETEXT);
    if (hglb == NULL)
    {
        OutputDebugString(L"getClipboardData 2");
        CloseClipboard();
        return false;
    }
        
    LPWSTR lpstr = (LPWSTR)GlobalLock(hglb);
    if (lpstr == NULL)
    {
        OutputDebugString(L"getClipboardData 3");
        CloseClipboard();
        return false;
    }

    text = lpstr;

    GlobalUnlock(hglb);
    CloseClipboard();
    return true;
}

bool Promixis::invokeControlCopy()
{
    #define key_count 4

    INPUT input[key_count];
    for (int i = 0; i < key_count; i++)
    {
        input[i].ki.dwFlags = 0;
        input[i].type = INPUT_KEYBOARD;
    }

    input[0].ki.wVk = VK_CONTROL;
    input[0].ki.wScan = MapVirtualKey(VK_CONTROL, MAPVK_VK_TO_VSC);
    input[1].ki.wVk = 0x43;
    input[1].ki.wScan = MapVirtualKey(0x43, MAPVK_VK_TO_VSC);
    input[2].ki.dwFlags = KEYEVENTF_KEYUP;
    input[2].ki.wVk = input[0].ki.wVk;
    input[2].ki.wScan = input[0].ki.wScan;
    input[3].ki.dwFlags = KEYEVENTF_KEYUP;
    input[3].ki.wVk = input[1].ki.wVk;
    input[3].ki.wScan = input[1].ki.wScan;

    if (!SendInput(key_count, (LPINPUT)input, sizeof(INPUT)))
    {        
        return false;
    }

    return true;
}

void Promixis::debugStr(const std::wstring& dbg)
{
    OutputDebugString(dbg.c_str());
}

void Promixis::debugStr(const std::string& dbg)
{
    OutputDebugStringA(dbg.c_str());
}


std::wstring Promixis::convertUTF8(const std::string& input)
{
    if (input.length() == 0)
    {
        return std::wstring();
    }

    std::wstring retVal;
    auto targetSize = MultiByteToWideChar(CP_UTF8, 0, input.c_str(), static_cast<int>(input.size()), NULL, 0);
    retVal.resize(targetSize);
    auto res = MultiByteToWideChar(CP_UTF8, 0, input.c_str(), static_cast<int>(input.size()),
        const_cast<LPWSTR>(retVal.data()), targetSize);
    if (res == 0)
    {
        // handle error, throw, do something...
        debugStr("Unable to convert UTF8 string to wstring: " + input);
        return std::wstring();
    }
    // debugStr(retVal + L"\n");
    return retVal;
}
