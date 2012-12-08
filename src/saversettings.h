#ifndef SAVERSETTINGS_H
#define SAVERSETTINGS_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QColor>
#include <QDir>
#include "card/cardset.h"
#include "card/carddisplay.h"
#include "util/csvfile.h"

class SaverSettings : public CardDisplay
{
public:
    static const QString PLAINTEXT_PATTERN;
    static const double  TEXTCELL_SCALE;
    static const QString TEXTCELL_STYLE;

    SaverSettings();
    SaverSettings(const SaverSettings& other) :
        CardDisplay(other),
        m_trace(other.m_trace),
        m_cardset_root(other.m_cardset_root),
        m_cardset_files(other.m_cardset_files),
        m_cardset(other.m_cardset) {}

    QColor trace() const { return m_trace; }
    const QDir& cardSetRoot() const { return m_cardset_root; }
    const QList<CsvFile>& cardSetFiles() const { return m_cardset_files; }
    const CardSet& cardSet() const { return m_cardset; }

    void setTrace(QColor trace) { m_trace = trace; }
    void setCardSetRoot(const QDir& root) { m_cardset_root = root; }
    void setCardSetFiles(const QList<CsvFile>& files) { m_cardset_files = files; }

    void reset();
    void read();
    bool write() const;

    void loadCardSet();

private:
    static const QColor         DEFAULT_BACKGROUND;
    static const int            DEFAULT_PERIOD;
    static const QColor         DEFAULT_TRACECLR;
    static const QColor         DEFAULT_TEXTCOLOR;
    static const QString        DEFAULT_FONTFAMILY;
    static const QList<int>     DEFAULT_SPLITX;
    static const QList<int>     DEFAULT_SPLITY;
    static const QList<QPoint>  DEFAULT_CELLS;
    static const QStringList    DEFAULT_CARDSETS;

    QColor         m_trace;        // Cell trace color, if any
    QDir           m_cardset_root; // Card set files root
    QList<CsvFile> m_cardset_files;// Card set files
    CardSet        m_cardset;      // Card set to use
};

#endif // SAVERSETTINGS_H
