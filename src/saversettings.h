#ifndef SAVERSETTINGS_H
#define SAVERSETTINGS_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QColor>
#include "card/cardset.h"
#include "card/carddisplay.h"
#include "util/csvfile.h"

class SaverSettings
{
public:
    static const QString PLAINTEXT_PATTERN;
    static const double  TEXTCELL_SCALE;
    static const QString TEXTCELL_STYLE;

    SaverSettings();
    SaverSettings(const SaverSettings& other) :
        m_trace(other.m_trace),
        m_cardset_files(other.m_cardset_files),
        m_cardset(other.m_cardset),
        m_carddisplay(other.m_carddisplay) {}

    QColor trace() const { return m_trace; }
    const QList<CsvFile>& cardSetFiles() const { return m_cardset_files; }
    const CardSet& cardSet() const { return m_cardset; }
    const CardDisplay& cardDisplay() const { return m_carddisplay; }
    CardDisplay& cardDisplay() { return m_carddisplay; }

    void setTrace(QColor trace) { m_trace = trace; }
    void setCardSetFiles(const QList<CsvFile>& files) { m_cardset_files = files; }
    void setCardDisplay(const CardDisplay& display) { m_carddisplay = display; }

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
    QList<CsvFile> m_cardset_files;// Card set files
    CardSet        m_cardset;      // Card set to use
    CardDisplay    m_carddisplay;  // Default card display parameters
};

#endif // SAVERSETTINGS_H
