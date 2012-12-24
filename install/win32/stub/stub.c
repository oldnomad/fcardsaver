#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static const char UNINSTALL_KEY[] =
    "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\FCardScreensaver";
static const char RUN_VALUE[] = "RunExecutable";

static char EXE_PATH[MAX_PATH] = "";

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
                     LPSTR lpCmdLine, int nCmdShow)
{
    LONG lRes;
    DWORD dwLen, dwRet;
    PROCESS_INFORMATION proc;

    dwLen = sizeof(EXE_PATH);
    lRes = RegGetValue(HKEY_LOCAL_MACHINE, UNINSTALL_KEY, RUN_VALUE,
                       RRF_RT_REG_SZ|RRF_ZEROONFAILURE, NULL, EXE_PATH,
                       &dwLen);
    if (lRes != ERROR_SUCCESS)
        return 0;
    if (!CreateProcess(EXE_PATH, GetCommandLine(), NULL, NULL, TRUE,
                       GetPriorityClass(GetCurrentProcess()), NULL, NULL,
                       NULL, &proc))
        return 0;
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
