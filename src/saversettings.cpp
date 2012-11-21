#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include "saversettings.h"
#include "card/cardset.h"
#include "util/textconv.h"
#include "platform/startup.h"

namespace Attr {
    namespace General {
        const QString SPLIT_X = "general/split-x";
        const QString SPLIT_Y = "general/split-y";
        const QString TRACE_COLOR = "general/trace";
        const QString BACKGROUND_COLOR = "general/background";
        const QString PERIOD = "general/period";
    }
    namespace Display {
        const QString ARRAY = "display";
        const QString CELL = "cell";
        const QString TEXT_COLOR = "color";
        const QString FONT = "font";
        const QString LINES = "lines";
        const QString PATTERN = "pattern";
        const QString STYLESHEET = "stylesheet";
    }
    namespace CardSet {
        const QString ARRAY = "cardsets";
        const QString PATH = "path";
        const QString SEPARATOR = "separator";
    }
}

const QColor        SaverSettings::DEFAULT_BACKGROUND = QColor(0, 0, 0);
const int           SaverSettings::DEFAULT_PERIOD = 3000;
const QColor        SaverSettings::DEFAULT_TRACECLR = QColor(Qt::transparent);
const QColor        SaverSettings::DEFAULT_TEXTCOLOR = QColor(255, 255, 255);
const QString       SaverSettings::DEFAULT_FONTFAMILY = "Helvetica";
const QList<int>    SaverSettings::DEFAULT_SPLITX =
        QList<int>() << 20 << 60 << 20;
const QList<int>    SaverSettings::DEFAULT_SPLITY =
        QList<int>() << 20 << 60 << 20;
const QList<QPoint> SaverSettings::DEFAULT_CELLS =
        QList<QPoint>() << QPoint(1, 1) << QPoint(2, 2) << QPoint(0, 0);
const QStringList   SaverSettings::DEFAULT_CARDSETS =
        QStringList() << "hiragana.tsv" << "katakana.tsv" << "kanjidic2-G1.tsv";

const QString       SaverSettings::PLAINTEXT_PATTERN = "<span>%1</span>";
const double        SaverSettings::TEXTCELL_SCALE = 0.95;
const QString       SaverSettings::TEXTCELL_STYLE = "* { color: %1; }\n";

SaverSettings::SaverSettings() :
    m_trace(), m_cardset_files(), m_cardset(), m_carddisplay()
{
    reset();
}

void SaverSettings::reset()
{
    m_trace = DEFAULT_TRACECLR;

    m_cardset_files.clear();
    m_cardset.clear();

    m_carddisplay.clear();
    m_carddisplay.setBackground(DEFAULT_BACKGROUND);
    m_carddisplay.setPeriod(DEFAULT_PERIOD);
    m_carddisplay.setSplitX(DEFAULT_SPLITX);
    m_carddisplay.setSplitY(DEFAULT_SPLITY);

    CardCell cell;
    cell.setTextColor(DEFAULT_TEXTCOLOR);
    cell.setFontFamily(DEFAULT_FONTFAMILY);
    for (int i = 0; i < DEFAULT_CELLS.size(); i++)
    {
        cell.setPos(DEFAULT_CELLS[i]);
        m_carddisplay.setValue(i, cell);
    }
}

void SaverSettings::loadCardSet()
{
    m_cardset.clear();
    QDir fileRoot = startup_t::get_data_root();
    // TODO: Settings parameter for data root
    foreach (CsvFile file, m_cardset_files)
    {
        if (!file.open(CsvFile::FLAG_ANKI|CsvFile::FLAG_COMMENT, fileRoot))
        {
            qWarning("Loading '%s': ERROR: %s",
                     qPrintable(file.path()),
                     qPrintable(file.lastError()));
            continue;
        }
        int cnt = m_cardset.loadCsv(m_carddisplay, file);
        file.close();
        if (cnt < 0)
            qWarning("Loading '%s': ERROR: %d", qPrintable(file.path()), cnt);
        else
            qDebug("Loading '%s': %d cards", qPrintable(file.path()), cnt);
    }
}

void SaverSettings::read()
{
    QSettings set;
    set.setIniCodec("UTF-8");

    m_trace =
            TextConv::toColor(set.value(Attr::General::TRACE_COLOR).toString(),
                              m_trace);

    QList<int> splX, splY;
    TextConv::toIntList(set.value(Attr::General::SPLIT_X), splX);
    TextConv::toIntList(set.value(Attr::General::SPLIT_Y), splY);
    if (!splX.isEmpty())
        m_carddisplay.setSplitX(splX);
    if (!splY.isEmpty())
        m_carddisplay.setSplitY(splY);

    QColor background =
            TextConv::toColor(set.value(Attr::General::BACKGROUND_COLOR).toString(),
                              m_carddisplay.background());
    m_carddisplay.setBackground(background);

    int period = set.value(Attr::General::PERIOD,
                           QVariant(m_carddisplay.period())).toInt();
    if (period >= CardDisplay::MIN_PERIOD && period <= CardDisplay::MAX_PERIOD)
        m_carddisplay.setPeriod(period);

    int ncells = set.beginReadArray(Attr::Display::ARRAY);
    QList<CardCell> cells;
    for (int i = 0; i < ncells; i++)
    {
        set.setArrayIndex(i);
        CardCell cell;
        cell.setPos(set.value(Attr::Display::CELL).toString());
        cell.setTextColor(
                    TextConv::toColor(
                        set.value(Attr::Display::TEXT_COLOR).toString(),
                        cell.textColor()));
        cell.setFontFamily(set.value(Attr::Display::FONT).toString());
        cell.setFontLines(set.value(Attr::Display::LINES).toDouble());
        cell.setPattern(set.value(Attr::Display::PATTERN).toString());
        cell.setStylesheet(set.value(Attr::Display::STYLESHEET).toString());
        cells.append(cell);
    }
    set.endArray();
    for (int i = 0; i < cells.size(); i++)
        m_carddisplay.setValue(i, cells[i]);
    m_carddisplay.setMaxIndex(cells.size() - 1);

    int ncardsets = set.beginReadArray(Attr::CardSet::ARRAY);
    qDebug("Reading cardsets [%d]", ncardsets);
    QList<CsvFile> files;
    for (int i = 0; i < ncardsets; i++)
    {
        set.setArrayIndex(i);
        QString path = set.value(Attr::CardSet::PATH).toString();
        if (path.isEmpty())
            continue;
        QChar separator =
                TextConv::toChar(set.value(Attr::CardSet::SEPARATOR).toString(),
                                 CsvFile::DEFAULT_SEPARATOR);
        files.append(CsvFile(path, separator));
    }
    set.endArray();
    if (files.isEmpty())
    {
        for (int i = 0; i < DEFAULT_CARDSETS.size(); i++)
            files.append(CsvFile(DEFAULT_CARDSETS[i]));
    }
    m_cardset_files = files;
    m_cardset.clear();
}

bool SaverSettings::write() const
{
    QSettings set;
    set.setIniCodec("UTF-8");

    set.setValue(Attr::General::SPLIT_X,
                 TextConv::fromIntList(m_carddisplay.splitX()));
    set.setValue(Attr::General::SPLIT_Y,
                 TextConv::fromIntList(m_carddisplay.splitY()));

    set.setValue(Attr::General::BACKGROUND_COLOR,
                 TextConv::fromColor(m_carddisplay.background()));
    set.setValue(Attr::General::PERIOD,
                 QVariant(m_carddisplay.period()));
    set.setValue(Attr::General::TRACE_COLOR,
                 TextConv::fromColor(m_trace));
    set.beginWriteArray(Attr::Display::ARRAY);
    for (int i = 0; i <= m_carddisplay.maxIndex(); i++)
    {
        set.setArrayIndex(i);
        const CardCell& cell = m_carddisplay.value(i);
        set.setValue(Attr::Display::CELL, cell.posAsString());
        set.setValue(Attr::Display::TEXT_COLOR,
                     TextConv::fromColor(cell.textColor()));
        set.setValue(Attr::Display::FONT, cell.fontFamily());
        set.setValue(Attr::Display::LINES, cell.fontLines());
        set.setValue(Attr::Display::PATTERN, cell.pattern());
        set.setValue(Attr::Display::STYLESHEET, cell.stylesheet());
    }
    set.endArray();

    set.beginWriteArray(Attr::CardSet::ARRAY);
    for (int i = 0; i < m_cardset_files.size(); i++)
    {
        set.setArrayIndex(i);
        const CsvFile& file = m_cardset_files[i];
        set.setValue(Attr::CardSet::PATH,
                     QVariant(file.path()));
        set.setValue(Attr::CardSet::SEPARATOR,
                     QVariant(TextConv::fromChar(file.separator())));
    }
    set.endArray();

    set.sync();
    return true;
}
