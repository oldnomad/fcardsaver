#include "util/textconv.h"

QString TextConv::fromChar(QChar value)
{
    if (value.isLetterOrNumber() || value.isPunct() || value.isSymbol())
        return QString(value);
    QString s('#');
    QString v = QString::number(value.unicode(), 16);
    if (v.length() < 4)
        s.append(QString(4 - v.length(), '0'));
    else if (v.length() > 4 && v.length() < 8)
        s.append(QString(8 - v.length(), '0'));
    s.append(v);
    return s;
}

QChar TextConv::toChar(const QString &name, QChar defval)
{
    if (name.length() == 1)
        return name.at(0);
    if (name.startsWith('#'))
        return QChar(name.mid(1).toUInt(0, 16));
    return defval;
}

QString TextConv::fromColor(QColor color)
{
    if (!color.isValid() || color.alpha() == 0)
        return "transparent";
    return color.name();
}

QColor TextConv::toColor(const QString &name, QColor defval)
{
    QColor color(name);
    return color.isValid() && color.alpha() != 0 ? color : defval;
}

QVariant TextConv::fromIntList(const QList<int> &list)
{
    QList<QVariant> vlist;
    foreach (int v, list)
        vlist.append(QVariant(v));
    return QVariant(vlist);
}

bool TextConv::toIntList(const QVariant &value, QList<int> list)
{
    if (!value.isValid())
        return false;
    QList<int> vlist;
    foreach (QVariant v, value.toList())
    {
        bool ok = false;
        vlist.append(v.toInt(&ok));
        if (!ok)
            return false;
    }
    list = vlist;
    return true;
}

bool TextConv::toIntList(const QStringList &value, QList<int> list)
{
    QList<int> vlist;
    foreach (QString v, value)
    {
        bool ok = false;
        vlist.append(v.toInt(&ok));
        if (!ok)
            return false;
    }
    list = vlist;
    return true;
}

int TextConv::smartSplit(QStringList& list, const QString& text, QChar separator)
{
    QRegExp qs("^\\s*([^\\s\"" + QRegExp::escape(separator) + "]*)|" +
               "(?:\"((?:\\\\\"|[^\"])*)\")");
    QRegExp comma("^\\s*(?:" + QRegExp::escape(separator) + "|$)");
    list.clear();
    if (QRegExp("^\\s*").exactMatch(text))
        return 0;
    int offset = 0;
    while (offset < text.size())
    {
        int pos = qs.indexIn(text, offset, QRegExp::CaretAtOffset);
        if (pos != offset)
            return 1 + offset;
        if (qs.pos(1) < 0)
            list.append(qs.cap(2).replace("\\\"", "\""));
        else
            list.append(qs.cap(1));
        offset += qs.matchedLength();
        pos = comma.indexIn(text, offset, QRegExp::CaretAtOffset);
        if (pos != offset)
            return 1 + offset;
        offset += comma.matchedLength();
    }
    return 0;
}

QString TextConv::smartEscape(const QString &text, QChar separator)
{
    if (QRegExp("[^\\s\"" + QRegExp::escape(separator) + "]*").exactMatch(text))
        return text;
    QString escaped(text);
    return "\"" + escaped.replace('"', "\\\"") + "\"";
}
