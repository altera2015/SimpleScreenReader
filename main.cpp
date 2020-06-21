#include <Windows.h>
#include "Settings.h"
#include "Mutex.h"
#include "MainWindow.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


Settings gSettings;
HINSTANCE gInstance;
static Mutex gMutex(L"Local\\SimpleScreenReaderSingleton");


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{   
    gInstance = hInstance;

    /* Detect if this is the first instance of the application. */
    bool firstInstance = true;
    if (!gMutex.valid() || !gMutex.lock(0) )
    {
        firstInstance = false;
        HWND wnd = FindWindow(0, APPLICATION_TITLE);
        ShowWindow(wnd, SW_SHOW);
        SetForegroundWindow(wnd);
        return 0;
    }

    MutexScope mutexScope(gMutex, true);


    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_WIN95_CLASSES | ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_USEREX_CLASSES;
    if (!InitCommonControlsEx(&icex))
    {
        OutputDebugString(L"Could not initialize the common controls system. This is bad.");
        return 0;
    }


    /* Load the settings */
    gSettings.load();
        
    MainWindow * w = 0;

    try 
    {
        w = new MainWindow(lpCmdLine);
    }
    catch (std::exception & e)
    {
        OutputDebugStringA(e.what());
        return -1;
    }
   
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {        
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    delete w;
    w = 0;
    
    return msg.wParam;
}





