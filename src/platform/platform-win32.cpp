#include "platform/startup.h"
#include "platform/nativeroot.h"

#ifdef Q_OS_WIN

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <tchar.h>

const CmdOption::parse_t startup_t::OPT_PARSE = {
    QList<CmdOption>() <<
        CmdOption("h") <<
        CmdOption("c", true, "0") <<
        CmdOption("p", true, "0") <<
        CmdOption("s") <<
        CmdOption("d"),
    QRegExp("(?:/|-)(\\w)(:(.*))?"),
    true
};

startup_t::startup_t() :
    m_root(0), m_window(0), m_mode(SS_MODE_SETUP), m_keymode(true)
{
}

bool startup_t::parse_arg(const CmdOption::arghash_t& argval)
{
    if (argval.contains("h"))
    {
        m_mode = SS_MODE_USAGE;
        return true;
    }
    if (argval.contains("c"))
    {
        QString val = argval.value("c");
        bool ok = false;
        m_root = (WId)val.toInt(&ok);
        if (!ok || !m_root)
        {
            qWarning("Invalid window handle: '%s'",
                     qPrintable(val));
            return false;
        }
        m_mode = SS_MODE_SETUP;
        return true;
    }
    if (argval.contains("p"))
    {
        QString val = argval.value("p");
        bool ok = false;
        m_root = (WId)val.toInt(&ok);
        if (!ok || !m_root)
        {
            qWarning("Invalid window handle: '%s'",
                     qPrintable(val));
            return false;
        }
        m_mode = SS_MODE_START;
        m_keymode = false;
        return true;
    }
    if (argval.contains("s"))
    {
        m_root = 0;
        m_mode = SS_MODE_STARTFS;
        return true;
    }
    if (argval.contains("d"))
    {
        m_root = 0;
        m_mode = SS_MODE_START;
        m_keymode = false;
        return true;
    }
    return true;
}

void startup_t::reparent(QWidget& widget)
{
    if (!m_root)
        return;
    HWND hwnd = (HWND)widget.winId();
    SetParent(hwnd, (HWND)m_root);
    SetWindowLong(hwnd, GWL_STYLE,
                  GetWindowLong(hwnd, GWL_STYLE)|WS_CHILD);
    RECT rc;
    GetClientRect((HWND)m_root, &rc);
    widget.setGeometry(0, 0, rc.right - rc.left, rc.bottom - rc.top);
}

QWidget* startup_t::native_root()
{
    if (!m_root)
        return 0;
    return new NativeRoot(m_root);
}

void startup_t::usage(QString appPath)
{
    qWarning("Usage: %s [options] [qtoptions]\n"
             "Options:\n"
             "  /h          print this help and exit;\n"
             "  /c [HWND]   show modal settings dialog [default];\n"
             "  /p HWND     show as a preview, in specified window;\n"
             "  /s          run as a screensaver;\n"
             "  /d          show in its own window;\n",
             qPrintable(appPath));
}

QDir startup_t::get_data_root()
{
    wchar_t path[MAX_PATH];
    HRESULT hRes = SHGetFolderPathW(NULL, CSIDL_COMMON_APPDATA|CSIDL_FLAG_CREATE,
                                    NULL, 0, path);
    if (!SUCCEEDED(hRes))
        wcscpy(path, L"C:\\Documents and Settings\\All Users\\Application Data");
    PathAppendW(path, ORG_NAME.toStdWString().c_str());
    CreateDirectoryW(path, NULL);
    PathAppendW(path, APP_NAME.toStdWString().c_str());
    CreateDirectoryW(path, NULL);
    return QDir(QString::fromWCharArray(path));
}

#endif
