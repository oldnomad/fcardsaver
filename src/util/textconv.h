#ifndef TEXTCONV_H
#define TEXTCONV_H

#include <QString>
#include <QColor>
#include <QList>
#include <QVariant>

namespace TextConv
{
QString fromChar(QChar value);
QChar toChar(const QString& name, QChar defval);

QString fromColor(QColor color);
QColor toColor(const QString& name, QColor defval);

QVariant fromIntList(const QList<int>& list);
bool toIntList(const QVariant& value, QList<int> list);
bool toIntList(const QStringList& value, QList<int> list);

int smartSplit(QStringList& list, const QString& text, QChar separator = ',');
QString smartEscape(const QString& text, QChar separator = ',');
}

#endif // TEXTCONV_H
