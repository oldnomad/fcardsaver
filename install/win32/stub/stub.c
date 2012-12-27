#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static const TCHAR UNINSTALL_KEY[] =
    TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\FCardScreensaver");
static const TCHAR RUN_VALUE[] = TEXT("RunExecutable");

static TCHAR EXE_PATH[MAX_PATH + 1] = TEXT("");

static LONG get_exe_path()
{
    LONG lRes;
    HKEY hKey;
    DWORD dwType, dwLen;

    if ((lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, UNINSTALL_KEY, 0,
                             KEY_QUERY_VALUE, &hKey)) != ERROR_SUCCESS)
        return lRes;
    dwLen = sizeof(EXE_PATH) - 1;
    lRes = RegQueryValueEx(hKey, RUN_VALUE, NULL, &dwType,
                           (LPBYTE)EXE_PATH, &dwLen);
    RegCloseKey(hKey);
    if (lRes != ERROR_SUCCESS || dwType != REG_SZ)
    {
        EXE_PATH[0] = '\0';
        return lRes;
    }
    EXE_PATH[dwLen] = '\0';
    return ERROR_SUCCESS;
}

static DWORD WINAPI wait_thread(LPVOID param)
{
    MSG msg;

    (void)param;    // UNUSED
    // We need this thread because Windows insists that a screensaver
    // must have a message queue.
    while (GetMessage(&msg, NULL, 0, 0))
    {
        // Do nothing
    }
    return 0;
}

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
                     LPSTR lpCmdLine, int nCmdShow)
{
    LONG lRes;
    DWORD dwRet;
    STARTUPINFO start;
    PROCESS_INFORMATION proc;

    (void)hInst;        // UNUSED
    (void)hPrevInst;    // UNUSED
    (void)lpCmdLine;    // UNUSED
    (void)nCmdShow;     // UNUSED
    if ((lRes = get_exe_path()) != ERROR_SUCCESS)
        return 0;
    if (EXE_PATH[0] == '\0')
        return 0;
    memset(&start, 0, sizeof(start));
    start.cb = sizeof(start);
    GetStartupInfo(&start);
    if (!CreateProcess(EXE_PATH, GetCommandLine(), NULL, NULL, TRUE,
                       GetPriorityClass(GetCurrentProcess()), NULL, NULL,
                       &start, &proc))
        return 0;

    CreateThread(NULL, 0, wait_thread, NULL, 0, NULL);
    do
    {
        WaitForSingleObject(proc.hProcess, INFINITE);
        dwRet = 0;
        if (!GetExitCodeProcess(proc.hProcess, &dwRet))
            break;
    }
    while (dwRet == STILL_ACTIVE);
    return dwRet;
}
