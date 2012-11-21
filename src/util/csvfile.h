#ifndef CSVFILE_H
#define CSVFILE_H

#include <QString>
#include <QStringList>
#include <QTextDecoder>
#include <QFile>
#include <QDir>

class CsvFile
{
public:
    CsvFile(const QString& path = QString(), QChar separator = DEFAULT_SEPARATOR) :
        m_path(path), m_separator(separator),
        m_file(), m_lineno(-1), m_flags(0), m_decoder(0), m_lastError() {}
    CsvFile(const CsvFile& other) :
        m_path(other.m_path), m_separator(other.m_separator),
        m_file(), m_lineno(-1), m_flags(0), m_decoder(0), m_lastError() {}
    CsvFile& operator =(const CsvFile& other)
    {
        close();
        m_path = other.m_path;
        m_separator = other.m_separator;
        return *this;
    }
    ~CsvFile() { if (m_decoder) delete m_decoder; }

    static const QChar DEFAULT_SEPARATOR;

    enum {
        FLAG_ANKI    = 0x0001,
        FLAG_COMMENT = 0x0002
    };

    const QString& path() const { return m_path; }
    void setPath(const QString& path) { close(); m_path = path; }
    int flags() const { return m_flags; }
    QChar separator() const { return m_separator; }
    void setSeparator(QChar sep) { close(); m_separator = sep; }
    const QString& lastError() const { return m_lastError; }
    int lastLineNo() const { return m_lineno; }

    bool open(int flags, const QDir& root = QDir());
    QStringList readLine(int* flags = 0);
    void close();

private:
    enum {
        FLAG__FIRST = 0x8000
    };

    QString         m_path;
    QChar           m_separator;
    QFile           m_file;
    int             m_lineno;
    int             m_flags;
    QTextDecoder*   m_decoder;
    QString         m_lastError;
};

#endif // CSVFILE_H
