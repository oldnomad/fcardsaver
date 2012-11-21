#include <QApplication>
#include "saverwindow.h"
#include "setup/setupdialog.h"
#include "platform/startup.h"

const QString startup_t::ORG_NAME("ALKOSoft");
const QString startup_t::APP_NAME("FCardSaver");

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(startup_t::ORG_NAME);
    QCoreApplication::setApplicationName(startup_t::APP_NAME);
    QApplication app(argc, argv);

    startup_t ss;
    CmdOption::arghash_t argval;
    if (!CmdOption::parseCmdLine(QCoreApplication::arguments(),
                                 startup_t::OPT_PARSE, argval))
    {
        qWarning("Unrecognized option '%s'\n",
                 qPrintable(argval.value("")));
        ss.m_mode = SS_MODE_USAGE;
    }
    else
    {
        if (!argval.values("").isEmpty())
        {
            qWarning("Unrecognized argument '%s'\n",
                     qPrintable(argval.values("").last()));
            ss.m_mode = SS_MODE_USAGE;
        }
        if (!ss.parse_arg(argval))
            ss.m_mode = SS_MODE_USAGE;
    }

    SaverSettings set;
    set.read();
    switch (ss.m_mode)
    {
    case SS_MODE_START:
    case SS_MODE_STARTFS:
        {
            set.loadCardSet();
            SaverWindow saver(ss.m_window, ss.m_keymode, set);
            if (ss.m_mode == SS_MODE_STARTFS)
                saver.showFullScreen();
            else
            {
                saver.hide();
                ss.reparent(saver);
                saver.show();
            }
            app.setActiveWindow(&saver);
            return app.exec();
        }
    case SS_MODE_SETUP:
        {
            SetupDialog dlg(set, ss.native_root());
            if (dlg.exec() == QDialog::Accepted)
                set.write();
        }
        break;
    default:
        ss.usage(QCoreApplication::applicationFilePath());
        break;
    }
    return 0;
}
