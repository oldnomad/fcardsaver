#ifndef STARTUP_H
#define STARTUP_H

#include <QWidget>
#include <QString>
#include <QRegExp>
#include <QDir>
#include "util/cmdoption.h"

enum {
    SS_MODE_START = 0,
    SS_MODE_STARTFS,
    SS_MODE_SETUP,
    SS_MODE_USAGE
};

struct startup_t {
    WId     m_root;
    WId     m_window;
    int     m_mode;
    bool    m_keymode;

    static const QString ORG_NAME;
    static const QString APP_NAME;
    static const CmdOption::parse_t OPT_PARSE;

    startup_t();
    bool parse_arg(const CmdOption::arghash_t& argval);
    void reparent(QWidget& widget);
    QWidget* native_root();

    static void usage(QString appPath);
    static QDir get_data_root();
};

#endif // STARTUP_H
