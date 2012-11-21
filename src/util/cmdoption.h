#ifndef CMDOPTION_H
#define CMDOPTION_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QMultiHash>
#include <QRegExp>


class CmdOption
{
public:
    CmdOption(const QString& option, bool hasValue = false,
              const QString& defval = NULL) :
        m_option(option), m_hasValue(hasValue), m_defValue(defval)
    {}

    struct parse_t
    {
        QList<CmdOption> options;
        QRegExp          optmatch;
        bool             optcase;
    };
    typedef QMultiHash<QString, QString> arghash_t;

    static bool parseCmdLine(const QStringList& args,
                             const parse_t& parse,
                             arghash_t& argval);

private:
    QString m_option;
    bool    m_hasValue;
    QString m_defValue;
};

#endif // CMDOPTION_H
