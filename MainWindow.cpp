#include "MainWindow.h"
#include "Version.h"

#include "resource.h"

#include "Settings.h"
#include "TextToSpeech.h"
#include "CommonFunctions.h"
#include "TrayIcon.h"

extern Settings gSettings;
extern HINSTANCE gInstance;

#define HOTKEY_SPEAK    1
#define HOTKEY_QUIET    2
#define HOTKEY_SPEAK_SMART 3


wchar_t * TOOLTIP_SPEAK = L"Speak text below";
wchar_t * TOOLTIP_PAUSE_TOGGLE = L"Toggle Speak Pause/Resume";
wchar_t * TOOLTIP_EXIT = L"Shutdown Simple Screen Reader";
wchar_t * TOOLTIP_SPEAK_SMART = L"Smart Speak Text";
wchar_t* TOOLTIP_REFRESH_SMART = L"Reload Smart Filters";



HWND MainWindow::createVoiceCombo(HWND parent)
{
    HWND wnd = CreateWindow(WC_COMBOBOX, L"", CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        0, 0, 200, 100, parent, (HMENU)IDM_VOICE_COMBO, gInstance, 0);

    wstringlist voices = m_TTS->voices();

    if (voices.size() > 0)
    {
        Promixis::setComboItems(wnd, voices);

        std::wstring voice = gSettings.voice();

        int index = Promixis::wstringlistFind(voices, voice);
        if (index < 0)
        {
            gSettings.setVoice(voices.at(0));
            m_TTS->setVoice(voices.at(0));
            gSettings.save();
            Promixis::setComboIndex(wnd, 0);
        }
        else
        {
            gSettings.setVoice(voices.at(index));
            m_TTS->setVoice(voices.at(index));
            Promixis::setComboIndex(wnd, index);
        }
    }

    WPARAM Font = (WPARAM)GetStockObject(DEFAULT_GUI_FONT);

    SendMessage(wnd, WM_SETFONT, Font, MAKELPARAM(FALSE, 0));

    return wnd;
}


HWND MainWindow::createRateCombo(HWND parent)
{
    HWND wnd = CreateWindow(WC_COMBOBOX, L"", CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        0, 0, 80, 100, parent, (HMENU)IDM_SPEECH_RATE, gInstance, 0);

    PWCHAR rateStrings[21] = {
        L"Slowest",
        L"-9",
        L"-8",
        L"-7",
        L"-6",
        L"-5",
        L"-4",
        L"-3",
        L"-2",
        L"-1",
        L"Default",
        L"+1",
        L"+2",
        L"+3",
        L"+4",
        L"+5",
        L"+6",
        L"+7",
        L"+8",
        L"+9",
        L"Fastest"
    };
    for (int i = 0; i < 21; i++)
    {
        SendMessage(wnd, CB_ADDSTRING, (WPARAM)0, (LPARAM)rateStrings[i]);
    }

    WPARAM Font = (WPARAM)GetStockObject(DEFAULT_GUI_FONT);

    SendMessage(wnd, WM_SETFONT, Font, MAKELPARAM(FALSE, 0));

    return wnd;
}


HWND MainWindow::createToolbar(HWND parent)
{
    const int ImageListID = 0;
    const int numButtons = 5;
    const int bitmapSize = 32;

    m_ImageList = ImageList_Create(bitmapSize, bitmapSize, // Dimensions of individual bitmaps.
        ILC_COLOR32,
        numButtons, 0);

    int i = 0;

    HANDLE img1 = LoadImage(gInstance, MAKEINTRESOURCE(IDB_SPEAK), IMAGE_BITMAP, bitmapSize, bitmapSize, LR_CREATEDIBSECTION);    
    HANDLE img2 = LoadImage(gInstance, MAKEINTRESOURCE(IDB_EXIT), IMAGE_BITMAP, bitmapSize, bitmapSize, LR_CREATEDIBSECTION);    
    HANDLE img3 = LoadImage(gInstance, MAKEINTRESOURCE(IDB_PAUSE), IMAGE_BITMAP, bitmapSize, bitmapSize, LR_CREATEDIBSECTION);
    HANDLE img4 = LoadImage(gInstance, MAKEINTRESOURCE(IDB_SPEAK_SMART), IMAGE_BITMAP, bitmapSize, bitmapSize, LR_CREATEDIBSECTION);
    HANDLE img5 = LoadImage(gInstance, MAKEINTRESOURCE(IDB_REFRESH), IMAGE_BITMAP, bitmapSize, bitmapSize, LR_CREATEDIBSECTION);

    ImageList_Add(m_ImageList, (HBITMAP)img1, 0);
    ImageList_Add(m_ImageList, (HBITMAP)img2, 0);
    ImageList_Add(m_ImageList, (HBITMAP)img3, 0);
    ImageList_Add(m_ImageList, (HBITMAP)img4, 0);
    ImageList_Add(m_ImageList, (HBITMAP)img5, 0);


    TBBUTTON tbb[numButtons];
    ZeroMemory(tbb, sizeof(tbb));

    int idx = 0;
    tbb[idx].iBitmap = 0;
    tbb[idx].fsState = TBSTATE_ENABLED;
    tbb[idx].fsStyle = TBSTYLE_BUTTON;
    tbb[idx].idCommand = IDM_SPEAK_BUTTON;
    tbb[idx].iString = (INT_PTR)TOOLTIP_SPEAK;

    tbb[++idx].iBitmap = 2;
    tbb[idx].fsState = TBSTATE_ENABLED;
    tbb[idx].fsStyle = BTNS_CHECK;
    tbb[idx].idCommand = IDM_SPEAK_PAUSE;
    tbb[idx].iString = (INT_PTR)TOOLTIP_PAUSE_TOGGLE;

    tbb[++idx].iBitmap = 1;
    tbb[idx].fsState = TBSTATE_ENABLED;
    tbb[idx].fsStyle = TBSTYLE_BUTTON;
    tbb[idx].idCommand = ID_FILE_EXIT;
    tbb[idx].iString = (INT_PTR)TOOLTIP_EXIT;

    tbb[++idx].iBitmap = 3;
    tbb[idx].fsState = TBSTATE_ENABLED;
    tbb[idx].fsStyle = TBSTYLE_BUTTON;
    tbb[idx].idCommand = IDM_SPEAK_SMART;
    tbb[idx].iString = (INT_PTR)TOOLTIP_SPEAK_SMART;

    tbb[++idx].iBitmap = 4;
    tbb[idx].fsState = TBSTATE_ENABLED;
    tbb[idx].fsStyle = TBSTYLE_BUTTON;
    tbb[idx].idCommand = IDM_REFRESH_SMART;
    tbb[idx].iString = (INT_PTR)TOOLTIP_REFRESH_SMART;

    

    HWND wnd = CreateToolbarEx(parent,
        WS_CHILD | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | WS_CLIPCHILDREN |
        WS_CLIPSIBLINGS | CCS_NODIVIDER | CCS_NORESIZE | WS_VISIBLE,
        IDM_MAIN_TOOLBAR,
        numButtons,
        0,
        0,
        (LPCTBBUTTON)&tbb,
        numButtons,
        bitmapSize, bitmapSize,                  // width & height of buttons
        bitmapSize, bitmapSize,                  // width & height of bitmaps
        sizeof(TBBUTTON));

    SendMessage(wnd, TB_SETMAXTEXTROWS, 0, 0);
    SendMessage(wnd, TB_SETIMAGELIST,
        (WPARAM)0,
        (LPARAM)m_ImageList);


    return wnd;
}

HWND MainWindow::createRebar(HWND parent)
{

    HWND wnd = CreateWindowEx(
        0L,
        REBARCLASSNAME,
        NULL,
        WS_VISIBLE | WS_BORDER | WS_CHILD | WS_CLIPCHILDREN |
        WS_CLIPSIBLINGS | CCS_NODIVIDER |
        RBS_VARHEIGHT | RBS_BANDBORDERS,
        0, 0, 400, 275,
        parent,
        (HMENU)0,
        gInstance,
        NULL);

    if (!wnd)
    {
        return NULL;
    }
    REBARINFO ri = { 0 };
    //no imagelist to attach to rebar
    ri.cbSize = sizeof(REBARINFO);
    SendMessage(wnd, RB_SETBARINFO, 0, reinterpret_cast<LPARAM>(&ri));

    RECT rc;
    // Initialize band info used by both bands.
    REBARBANDINFO rbBand;
    rbBand.cbSize = sizeof(REBARBANDINFO);
    rbBand.hbmBack = 0;
    rbBand.cx = 0;

    rbBand.fMask =
        RBBIM_STYLE       // fStyle is valid.        
        | RBBIM_CHILD       // hwndChild is valid.
        | RBBIM_CHILDSIZE   // child size members are valid.
        | RBBIM_SIZE;       // cx is valid

    rbBand.fStyle = RBBS_CHILDEDGE | RBBS_NOGRIPPER;


    rbBand.hwndChild = m_ToolbarWnd;
    GetWindowRect(rbBand.hwndChild, &rc);
    rbBand.lpText = 0;
    rbBand.cxMinChild = rc.right - rc.left;
    rbBand.cyMinChild = rc.bottom - rc.top + 8;
    //rbBand.cyMaxChild = rbBand.cyMinChild;
    SendMessage(wnd, RB_INSERTBAND, (WPARAM)0, (LPARAM)&rbBand);


    rbBand.fStyle = RBBS_CHILDEDGE;
    rbBand.fMask =
        RBBIM_STYLE       // fStyle is valid.
        | RBBIM_TEXT        // lpText is valid.
        | RBBIM_CHILD       // hwndChild is valid.
        | RBBIM_CHILDSIZE   // child size members are valid.
        | RBBIM_SIZE;       // cx is valid


    // Set values unique to the band with the combo box.

    GetWindowRect(m_VoiceComboWnd, &rc);
    rbBand.lpText = TEXT("Voice");
    rbBand.hwndChild = m_VoiceComboWnd;
    rbBand.cxMinChild = rc.right - rc.left;
    rbBand.cyMinChild = rc.bottom - rc.top;
    // The default width should be set to some value wider than the text. The combo 
    // box itself will expand to fill the band.
    rbBand.cx = 100;

    // Add the band that has the combo box.
    SendMessage(wnd, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand);

    rbBand.fMask =
        RBBIM_STYLE       // fStyle is valid.
        | RBBIM_TEXT        // lpText is valid.
        | RBBIM_CHILD       // hwndChild is valid.
        | RBBIM_CHILDSIZE   // child size members are valid
        | RBBIM_SIZE;
    rbBand.cxHeader = 10;
    rbBand.clrBack = RGB(100, 0, 0);
    rbBand.clrFore = RGB(100, 100, 100);
    GetWindowRect(m_RateWnd, &rc);
    rbBand.hwndChild = m_RateWnd;
    rbBand.lpText = TEXT("Rate");
    rbBand.cxMinChild = rc.right - rc.left;
    rbBand.cyMinChild = rc.bottom - rc.top;



    SendMessage(wnd, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand);



    ShowWindow(wnd, SW_SHOW);
    UpdateWindow(wnd);
    return wnd;
}


BOOL MainWindow::initDialog(HWND wnd)
{
    m_VoiceComboWnd = createVoiceCombo(wnd);
    m_RateWnd = createRateCombo(wnd);
    m_ToolbarWnd = createToolbar(wnd);
    m_RebarWnd = createRebar(wnd);
    UpdateWindow(wnd);    
    
    setCaption(APPLICATION_TITLE);
    SendMessage(GetDlgItem(wnd, IDC_TEXT_TO_SPEAK), WM_SETTEXT, 0, (LPARAM)L"Welcome to Simple Screen Reader.\r\n\r\nTo read any text on your screen in any application simply highlight it with your mouse and press\r\ncontrol+F11. To make the reader stop simply press control+F10.\r\n\r\nYou can change the voice of the reader with the voice selector above.");

    m_TTS->setRate(gSettings.speechRate());
    SendMessage(m_RateWnd, CB_SETCURSEL, gSettings.speechRate() + 10, 0);

    CheckMenuItem(GetMenu(wnd), IDC_START_WITH_WINDOWS, MF_BYCOMMAND | (gSettings.getRunOnStartup() ? MF_CHECKED : MF_UNCHECKED));

    HICON hIcon = (HICON)LoadImage(gInstance,
        MAKEINTRESOURCE(IDI_ICON1),
        IMAGE_ICON,
        GetSystemMetrics(SM_CXSMICON),
        GetSystemMetrics(SM_CYSMICON),
        0);

    SendMessage(wnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);


    m_TrayIcon = new TrayIcon(gInstance, APPLICATION_TITLE);
    m_TrayIcon->add(wnd);



    if (!RegisterHotKey(wnd, HOTKEY_SPEAK, MOD_CONTROL | MOD_NOREPEAT, VK_F11))
    {
        OutputDebugString(L"MainWindow::MainWindow / Could not register speak hotkey\n");
    }
    if (!RegisterHotKey(wnd, HOTKEY_QUIET, MOD_CONTROL | MOD_NOREPEAT, VK_F10))
    {
        OutputDebugString(L"MainWindow::MainWindow / Could not register quiet hotkey\n");
    }
    if (!RegisterHotKey(wnd, HOTKEY_SPEAK_SMART, MOD_CONTROL | MOD_NOREPEAT, VK_F12))
    {
        OutputDebugString(L"MainWindow::MainWindow / Could not register speak hotkey\n");
    }
    
    return TRUE;
}

BOOL MainWindow::onDestroy(HWND wnd)
{
    DestroyWindow(m_VoiceComboWnd);
    DestroyWindow(m_RateWnd);
    DestroyWindow(m_ToolbarWnd);
    DestroyWindow(m_RebarWnd);
    ImageList_Destroy(m_ImageList);

    UnregisterHotKey(m_Wnd, HOTKEY_SPEAK);
    UnregisterHotKey(m_Wnd, HOTKEY_QUIET);
    UnregisterHotKey(m_Wnd, HOTKEY_SPEAK_SMART);
    
    m_TrayIcon->remove();
    delete m_TrayIcon;
    m_TrayIcon = 0;

    m_Wnd = 0;

    return TRUE;
}

BOOL MainWindow::onSpeakClicked(HWND wnd)
{
    std::wstring textToSpeak;
    if (Promixis::getDlgString(wnd, IDC_TEXT_TO_SPEAK, textToSpeak))
    {
        SendMessage(m_ToolbarWnd, TB_SETSTATE, IDM_SPEAK_PAUSE, LOWORD(TBSTATE_ENABLED));
        m_TTS->speak(textToSpeak);
    }
    return TRUE;
}


BOOL MainWindow::onSpeakSmartClicked(HWND wnd)
{
    std::wstring textToSpeak;
    if (Promixis::getDlgString(wnd, IDC_TEXT_TO_SPEAK, textToSpeak))
    {
        textToSpeak = m_SmartFilter.filter(textToSpeak);
        Promixis::setDlgString(wnd, IDC_TEXT_TO_SPEAK, textToSpeak);

        SendMessage(m_ToolbarWnd, TB_SETSTATE, IDM_SPEAK_PAUSE, LOWORD(TBSTATE_ENABLED));
        m_TTS->speak(textToSpeak);
    }
    return TRUE;
}


BOOL MainWindow::onRefreshSpeakSmart(HWND wnd)
{
    if (!m_SmartFilter.load(gSettings.filterLocation()))
    {
        MessageBox(0, L"Unable to load Smart Filters.", L"Parse Failed", MB_OK | MB_ICONEXCLAMATION);
    }
    return TRUE;
}


BOOL MainWindow::onHotkeySpeak(HWND wnd)
{
    DWORD clipBoardCounter = GetClipboardSequenceNumber();

    std::wstring text;

    if (!Promixis::invokeControlCopy())
    {
        OutputDebugString(L"onHotkeySpeak / Could not invoke control copy.\n");
    }

    // wait for the clipboard to change...
    int maxLoops = 100;
    while ((clipBoardCounter == GetClipboardSequenceNumber()) && maxLoops>0)
    {
        Sleep(10);
        maxLoops--;
    }

    // get clipboard data, might have to retry.
    maxLoops = 10;
    while (!Promixis::getClipboardData(text) && maxLoops > 0)
    {
        Sleep(10);
        maxLoops--;
    }

    // that's not good.
    if (maxLoops == 0)
    {
        OutputDebugString(L"onHotkeySpeak / Could not get clipboard data.\n");
        return TRUE;
    }

    SendMessage(m_ToolbarWnd, TB_SETSTATE, IDM_SPEAK_PAUSE, LOWORD(TBSTATE_ENABLED));
    m_TTS->speak(text);

    return TRUE;
}

BOOL MainWindow::onHotkeySpeakSmart(HWND wnd)
{
    DWORD clipBoardCounter = GetClipboardSequenceNumber();

    std::wstring text;

    if (!Promixis::invokeControlCopy())
    {
        OutputDebugString(L"onHotkeySpeakSmart / Could not invoke control copy.\n");
    }

    // wait for the clipboard to change...
    int maxLoops = 100;
    while ((clipBoardCounter == GetClipboardSequenceNumber()) && maxLoops > 0)
    {
        Sleep(10);
        maxLoops--;
    }

    // get clipboard data, might have to retry.
    maxLoops = 10;
    while (!Promixis::getClipboardData(text) && maxLoops > 0)
    {
        Sleep(10);
        maxLoops--;
    }

    // that's not good.
    if (maxLoops == 0)
    {
        OutputDebugString(L"onHotkeySpeakSmart / Could not get clipboard data.\n");
        return TRUE;
    }

    text = m_SmartFilter.filter(text);

    SendMessage(m_ToolbarWnd, TB_SETSTATE, IDM_SPEAK_PAUSE, LOWORD(TBSTATE_ENABLED));
    m_TTS->speak(text);

    return TRUE;
}

BOOL MainWindow::onPauseToggle(HWND wnd)
{
    if (m_TTS->isPaused())
    {
        m_TTS->resume();        
        SendMessage(m_ToolbarWnd, TB_SETSTATE, IDM_SPEAK_PAUSE, LOWORD(TBSTATE_ENABLED));
    }
    else
    {
        m_TTS->pause();        
        SendMessage(m_ToolbarWnd, TB_SETSTATE, IDM_SPEAK_PAUSE, LOWORD(TBSTATE_ENABLED | TBSTATE_CHECKED));
    }

    return TRUE;
}


BOOL MainWindow::onVoiceComboMessage(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    if (HIWORD(wParam) == CBN_SELCHANGE)
    {
        int index = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
        if (index >= 0)
        {
            wstringlist voices = m_TTS->voices();
            if ((unsigned int)index < voices.size())
            {
                gSettings.setVoice(voices.at(index));
                m_TTS->setVoice(voices.at(index));
                gSettings.save();
            }
        }
        return TRUE;
    }

    return FALSE;
}

BOOL MainWindow::onSpeechRateComboMessage(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    if (HIWORD(wParam) == CBN_SELCHANGE)
    {
        int index = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
        if (index >= 0)
        {
            index -= 10;
            gSettings.setSpeechRate(index);
            m_TTS->setRate(index);
            gSettings.save();
        }
        return TRUE;
    }

    return FALSE;
}

BOOL MainWindow::onCloseWindow(HWND wnd)
{
    ShowWindow(wnd, SW_HIDE);
    return TRUE;
}

BOOL MainWindow::onHelpAbout(HWND wnd)
{
    wchar_t buffer[1000];
    swprintf_s(buffer, 1000, L"Simple Screen Reader %s\r\nThis application was intended to provide a quick and simple way to read any text selectable on screen.", VER_LFILEVERSION_STR);
    MessageBox(wnd,buffer, L"About Simple Screen Reader", MB_ICONINFORMATION | MB_OK);
    return TRUE;
}

BOOL MainWindow::toggleStartWithWindows(HWND wnd)
{
    UINT flags = GetMenuState(GetMenu(wnd), IDC_START_WITH_WINDOWS, MF_BYCOMMAND);

    bool currentlyChecked = (flags & MF_CHECKED) == MF_CHECKED;

    bool newState = !currentlyChecked;

    if (newState)
    {
        CheckMenuItem(GetMenu(wnd), IDC_START_WITH_WINDOWS, MF_BYCOMMAND | MF_CHECKED);
    }
    else
    {
        CheckMenuItem(GetMenu(wnd), IDC_START_WITH_WINDOWS, MF_BYCOMMAND | MF_UNCHECKED);
    }
    gSettings.setRunOnStartup(newState);
    return newState;

}

BOOL MainWindow::onResize(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    SendMessage(m_RebarWnd, WM_SIZE, wParam, lParam);

    RECT rebarRect, windowRect;
    GetWindowRect(m_RebarWnd, &rebarRect);
    GetClientRect(wnd, &windowRect);

    int rebarHeight = rebarRect.bottom - rebarRect.top;
    int windowHeight = windowRect.bottom - windowRect.top;

    HWND textBox = GetDlgItem(wnd, IDC_TEXT_TO_SPEAK);

    MoveWindow(textBox, windowRect.left + 8, rebarHeight + 8, windowRect.right - windowRect.left - 16, windowHeight - rebarHeight - 16, TRUE);

    return FALSE;
}


BOOL CALLBACK MainWindow::staticWndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    MainWindow * mw = 0;
    if (msg != WM_INITDIALOG)
    {
        mw = (MainWindow*)GetWindowLongPtr(wnd, GWLP_USERDATA);
    }
    else
    {
        mw = (MainWindow*)lParam;
    }     
    return mw->wndProc(wnd, msg, wParam, lParam);
}

BOOL MainWindow::wndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        initDialog(wnd);
        break;

    case WM_ENDSESSION:
        DestroyWindow(wnd);
        return TRUE; // sure let's exit.

    case WM_CLOSE:
        return onCloseWindow(wnd);  // let's minimize instead.

    case WM_DESTROY:

        onDestroy(wnd);
        PostQuitMessage(0);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_START_WITH_WINDOWS:
            return toggleStartWithWindows(wnd);
            
        case IDM_SPEAK_BUTTON:
            return onSpeakClicked(wnd);

        case IDM_SPEAK_SMART:
            return onSpeakSmartClicked(wnd);

        case IDM_REFRESH_SMART:
            return onRefreshSpeakSmart(wnd);

        case IDM_SPEAK_PAUSE:
            return onPauseToggle(wnd);

        case IDM_VOICE_COMBO:
            return onVoiceComboMessage(wnd, wParam, lParam);

        case IDM_SPEECH_RATE:
            return onSpeechRateComboMessage(wnd, wParam, lParam);

        case ID_FILE_EXIT:
            DestroyWindow(wnd);
            return TRUE;

        case ID_HELP_ABOUT:
            return onHelpAbout(wnd);            

        }
        break;
    case WM_HOTKEY:
        switch (wParam) 
        {
        case HOTKEY_SPEAK:
            return onHotkeySpeak(wnd);
        case HOTKEY_QUIET:
            return onPauseToggle(wnd);
        case HOTKEY_SPEAK_SMART:
            return onHotkeySpeakSmart(wnd);
        }        
        break;

    case WM_TRAYICON:
        switch (lParam)
        {
        case WM_LBUTTONUP:
            m_TrayIcon->onLeftClick(wnd);
            return TRUE;

        case WM_RBUTTONDOWN:
            m_TrayIcon->onRightClick(wnd);
            return TRUE;
        }
        break;

    case WM_SYSCOMMAND:
        switch (wParam & 0xfff0)
        {
        case SC_MINIMIZE:
            ShowWindow(wnd, SW_HIDE);
            return TRUE;
        }
    case WM_SIZE:
        return onResize(wnd, wParam, lParam);

    default:
        return FALSE;
    }

    return FALSE;
}


MainWindow::MainWindow(LPSTR cmdLine) : 
    m_Wnd(0),
    m_VoiceComboWnd(0),
    m_RateWnd(0),
    m_ToolbarWnd(0),
    m_RebarWnd(0),
    m_ImageList(nullptr),
    m_TTS(0), 
    m_TrayIcon(0)    
{
    /* Initialize the TTS engine. */
    m_TTS = new TextToSpeech();
    if (!m_TTS->hasTTS())
    {
        MessageBox(0, L"No Text To Speech system found, this application requires Windows 7 or higher.", L"Startup failed", MB_OK | MB_ICONEXCLAMATION);
        throw std::exception("No Text To Speech Engine found.");
    }

    /* Create the Window and setup the trayicon, hotkeys and message pump. */
    m_Wnd = CreateDialogParam(gInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), 0, staticWndProc, (LPARAM)this);
    SetWindowLongPtr(m_Wnd, GWLP_USERDATA, (LONG_PTR)this);

    if (strstr(cmdLine, "--hide") == 0)
    {
        ShowWindow(m_Wnd, SW_SHOW);
    }

    //L"c:\\SimpleReaderFilters.json"

    if (!m_SmartFilter.load(gSettings.filterLocation()))
    {
        MessageBox(0, L"Unable to load SmartFilters.", L"Filter Startup failed", MB_OK | MB_ICONEXCLAMATION);
    }
}


MainWindow::~MainWindow()
{
    delete m_TTS;
    m_TTS = 0;
}


void MainWindow::setCaption(const std::wstring & caption)
{
    SetWindowText(m_Wnd, caption.c_str());
}

HWND MainWindow::wnd() const
{
    return m_Wnd;
}
