#include <QStringListIterator>
#include "cmdoption.h"

bool CmdOption::parseCmdLine(const QStringList &args,
                             const parse_t& parse,
                             arghash_t &argval)
{
    QRegExp optre(parse.optmatch);
    QStringListIterator argIt(args);
    argIt.next();   // Skip command name
    argval.clear();
    QString needVal;
    QString defVal;
    while (argIt.hasNext())
    {
        QString arg = argIt.next();
        if (optre.exactMatch(arg))
        {
            if (!needVal.isNull())
            {
                argval.insert(needVal, defVal);
                needVal = (char*)0;
            }
            QString opt = optre.cap(1);
            QString val;
            bool hasVal = false;
            if (optre.captureCount() > 1)
            {
                QString v = optre.cap(optre.captureCount());
                QString ctl = optre.captureCount() > 2 ? optre.cap(2) : v;
                if (!ctl.isEmpty())
                {
                    val = v;
                    hasVal = true;
                }
            }
            const CmdOption* ptr = 0;
            if (parse.optcase)
                opt = opt.toLower();
            for (int i = 0; i < parse.options.size(); i++)
                if (parse.options[i].m_option == opt)
                {
                    ptr = &parse.options[i];
                    break;
                }
            if (!ptr)
            {
                argval.insert("", arg);
                return false;
            }
            if (ptr->m_hasValue)
            {
                if (!hasVal)
                {
                    needVal = ptr->m_option;
                    defVal = ptr->m_defValue;
                    continue;
                }
            }
            else
            {
                val = ptr->m_defValue.isNull() ? ptr->m_option : ptr->m_defValue;
            }
            argval.insert(ptr->m_option, val);
            continue;
        }
        if (!needVal.isNull())
        {
            argval.insert(needVal, arg);
            needVal = (char*)0;
            continue;
        }
        argval.insert("", arg);
    }
    return true;
}
