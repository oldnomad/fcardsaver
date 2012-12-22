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
        const QString DATA_ROOT = "general/data-root";
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
    CardDisplay(), m_trace(), m_cardset_root(), m_cardset_files(), m_cardset()
{
    reset();
}

void SaverSettings::reset()
{
    m_trace = DEFAULT_TRACECLR;

    m_cardset_root = startup_t::get_data_root();

    m_cardset_files.clear();
    m_cardset.clear();

    clear();
    setBackground(DEFAULT_BACKGROUND);
    setPeriod(DEFAULT_PERIOD);
    setSplitX(DEFAULT_SPLITX);
    setSplitY(DEFAULT_SPLITY);

    CardCell cell;
    cell.setTextColor(DEFAULT_TEXTCOLOR);
    cell.setFontFamily(DEFAULT_FONTFAMILY);
    for (int i = 0; i < DEFAULT_CELLS.size(); i++)
    {
        cell.setPos(DEFAULT_CELLS[i]);
        setValue(i, cell);
    }
}

void SaverSettings::loadCardSet()
{
    m_cardset.clear();
    foreach (CsvFile file, m_cardset_files)
    {
        if (!file.open(CsvFile::FLAG_ANKI|CsvFile::FLAG_COMMENT, m_cardset_root))
        {
            qWarning("Loading '%s': ERROR: %s",
                     qPrintable(file.path()),
                     qPrintable(file.lastError()));
            continue;
        }
        int cnt = m_cardset.loadCsv(*this, file);
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
        setSplitX(splX);
    if (!splY.isEmpty())
        setSplitY(splY);

    QColor bgd =
            TextConv::toColor(set.value(Attr::General::BACKGROUND_COLOR).toString(),
                              background());
    setBackground(bgd);

    int per = set.value(Attr::General::PERIOD, QVariant(period())).toInt();
    if (per >= CardDisplay::MIN_PERIOD && per <= CardDisplay::MAX_PERIOD)
        setPeriod(per);

    QString root = set.value(Attr::General::DATA_ROOT,
                             m_cardset_root.absolutePath()).toString();
    if (!root.isEmpty())
    {
        m_cardset_root.setPath(root);
        m_cardset_root.makeAbsolute();
    }

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
        setValue(i, cells[i]);
    if (!cells.isEmpty())
        setMaxIndex(cells.size() - 1);

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
                 TextConv::fromIntList(splitX()));
    set.setValue(Attr::General::SPLIT_Y,
                 TextConv::fromIntList(splitY()));

    set.setValue(Attr::General::BACKGROUND_COLOR,
                 TextConv::fromColor(background()));
    set.setValue(Attr::General::PERIOD,
                 QVariant(period()));
    set.setValue(Attr::General::TRACE_COLOR,
                 TextConv::fromColor(m_trace));

    set.setValue(Attr::General::DATA_ROOT,
                 QDir::toNativeSeparators(m_cardset_root.absolutePath()));

    set.beginWriteArray(Attr::Display::ARRAY);
    for (int i = 0; i <= maxIndex(); i++)
    {
        set.setArrayIndex(i);
        const CardCell& cell = value(i);
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
