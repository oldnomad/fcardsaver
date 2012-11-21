#include "util/csvfile.h"

const QChar CsvFile::DEFAULT_SEPARATOR('\t');

bool CsvFile::open(int flags, const QDir& root)
{
    close();
    m_file.setFileName(QFileInfo(root, m_path).absoluteFilePath());
    if (!m_file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        m_lastError = m_file.errorString();
        return false;
    }
    m_decoder = QTextCodec::codecForName("UTF-8")->makeDecoder();
    m_flags = flags | FLAG__FIRST;
    m_lineno = 0;
    return true;
}

QStringList CsvFile::readLine(int *flags)
{
    QStringList fieldSet;
    QString field;
    bool inQuote = false;
    QRegExp endQuote("((?:[^\"]|\"\")*)\"");
    QRegExp endBare("[\"" + QRegExp::escape(m_separator) + "]");
    for (;;)
    {
        QByteArray lineData = m_file.readLine();
        m_lineno++;
        if (lineData.isNull())
        {
            m_lastError.clear();
            return QStringList();
        }
        QString line = m_decoder->toUnicode(lineData);
        if (line.endsWith('\n'))
            line.resize(line.length() - 1);
        int index = 0;
        if (inQuote)
        {
            // Line break in quotes
            field.append("\n");
            goto IN_QFIELD;
        }
        if (line.startsWith('#'))
        {
            if ((m_flags & FLAG_COMMENT) != 0)
            {
                if (flags)
                    *flags = FLAG_COMMENT;
                return QStringList(line);
            }
            continue;
        }
        if ((m_flags & FLAG__FIRST) != 0)
        {
            if ((m_flags & FLAG_ANKI) != 0)
            {
                QRegExp tags("tags:\\s*(.*)");
                if (tags.exactMatch(line))
                {
                    if (flags)
                        *flags = FLAG_ANKI;
                    return QStringList(tags.cap(1).split(QRegExp("\\s+"),
                                                         QString::SkipEmptyParts));
                }
            }
            m_flags &= ~FLAG__FIRST;
        }
        while (index < line.length())
        {
            field.clear();
            if (line[index] == '"')
            {
                index++;
                inQuote = true;
IN_QFIELD:
                index = line.indexOf(endQuote, index);
                if (index < 0)
                {
                    field.append(line.replace("\"\"", "\""));
                    break;
                }
                field.append(endQuote.cap(1).replace("\"\"", "\""));
                fieldSet.append(field);
                index += endQuote.matchedLength();
                inQuote = false;
            }
            else
            {
                int efld = line.indexOf(endBare, index);
                if (efld < 0)
                {
                    fieldSet.append(line.mid(index));
                    index = line.length();
                }
                else
                {
                    fieldSet.append(line.mid(index, efld - index));
                    index = efld;
                }
            }
            if (index >= line.length())
                break;
            if (line[index] != m_separator)
            {
                m_lastError =
                        QString().sprintf(
                            "Syntax error: got %lc (%04X), expected %lc (%04X)"
                            " at line %d, unicode offset %d",
                            line[index].unicode(), line[index].unicode(),
                            m_separator.unicode(), m_separator.unicode(),
                            m_lineno, index);
                return QStringList();
            }
            index++;
        }
        if (!inQuote)
        {
            if (flags)
                *flags = 0;
            return fieldSet;
        }
    }
}

void CsvFile::close()
{
    if (m_decoder)
    {
        delete m_decoder;
        m_decoder = 0;
    }
    if (m_file.isOpen())
        m_file.close();
    m_flags = 0;
}
