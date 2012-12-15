#include "platform/startup.h"

#ifdef Q_OS_UNIX

#include <QX11Info>
#include "platform/vroot.h"

const CmdOption::parse_t startup_t::OPT_PARSE = {
    QList<CmdOption>() <<
        CmdOption("help") <<
        CmdOption("caption", true, "") <<
        CmdOption("icon", true, "") <<
        CmdOption("setup") <<
        CmdOption("window", true, "0") <<
        CmdOption("root") <<
        CmdOption("demo"),
    QRegExp("-(\\w+)(=(.*))?"),
    false
};

startup_t::startup_t() :
    m_root(0), m_window(0), m_mode(SS_MODE_START), m_keymode(false)
{
}

bool startup_t::parse_arg(const CmdOption::arghash_t &argval)
{
    if (argval.contains("help"))
    {
        m_mode = SS_MODE_USAGE;
        return true;
    }
    if (argval.contains("setup"))
    {
        m_mode = SS_MODE_SETUP;
        return true;
    }
    if (argval.contains("window"))
    {
        QString val = argval.value("window");
        bool ok;
        m_window = m_root = (WId)val.toInt(&ok);
        if (!ok || !m_root)
            return false;
        m_mode = SS_MODE_START;
        return true;
    }
    if (argval.contains("root"))
    {
        QX11Info inf;
        m_window = m_root = DefaultRootWindow(inf.display());
        m_mode = SS_MODE_START;
        return true;
    }
    if (argval.contains("demo"))
    {
        m_window = m_root = 0;
        m_mode = SS_MODE_START;
        return true;
    }
    return true;
}

void startup_t::reparent(QWidget& widget)
{
    (void)widget;
    // Do nothing
}

QWidget* startup_t::native_root()
{
    return 0;
}

void startup_t::usage(QString appPath)
{
    qWarning("Usage: %s [options] [qtoptions]\n"
             "Options:\n"
             "  -help       print this help and exit;\n"
             "  -setup      show settings dialog;\n"
             "  -window ID  show as a preview, in specified window;\n"
             "  -root       show in root window;\n"
             "  -demo       show in its own window [default];\n",
             qPrintable(appPath));
}

QDir startup_t::get_data_root()
{
    return QDir("/usr/share/fcardsaver");
}

#endif
