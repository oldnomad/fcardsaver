#include <QTextDecoder>
#include "card/cardset.h"
#include "util/textconv.h"

const Card CardSet::NONE;

CardSet::CardSet() : base_t(), m_displays()
{
}

CardSet::CardSet(const CardSet &other) : base_t(), m_displays()
{
    *this = other;
}

CardSet& CardSet::operator=(const CardSet& other)
{
    clear();
    const CardDisplay* display = 0;
    foreach (Card card, other)
    {
        if (card.display())
        {
            if (!display || *display != *card.display())
            {
                m_displays.append(*card.display());
                display = &m_displays.last();
            }
        }
        else
            display = 0;
        append(Card(card.values(), display));
    }
    return *this;
}

void CardSet::clear()
{
    base_t::clear();
    m_displays.clear();
}

const Card& CardSet::get(int index) const
{
    if (isEmpty())
        return NONE;
    return at(index % size());
}

int CardSet::loadCsv(const CardDisplay& defDisplay, CsvFile& file)
{
    /*
     * This method reads Anki plain text file format:
     *  - plain text, UTF-8, BOM optional;
     *  - ignore lines starting with "#";
     *  - if first uncommented line starts with "tags:", use space-split words
     *    as additional facts;
     *  - recognize any delimiters;
     *  - use double quotes ("\"") as quote char, including quoting newline;
     *  - literal newline in field should be replaced by "<br>";
     *  - fields are interpreted as simplified HTML;
     *
     * Format is extended by following new features:
     *  - comments in format "#FCARD# <attr-name> <attr-value>" are interpreted as
     *    display attributes for following cards. Following attributes are
     *    recognized:
     *
     *      - "background <color-name>"
     *        specifies background color for cards from this set.
     *
     *      - "period <time-ms>"
     *        specifies period (in milliseconds) for which card will be shown.
     *
     *      - "split-{x|y} <width>,..."
     *        specifies horizontal or vertical split distribution.
     *
     *      - "cell <fact-index>,<cell-x>,<cell-y>,<color-name>,<font-family>,<pattern>,<stylesheet>"
     *        specifies display parameters for fact text.
     *
     *      - "protect {on|off},<keyword>,..."
     *        protects or unprotects specified keywords from further changes below that point.
     *
     *      - "include <filename>,<separator>"
     *        includes specified file; path may be relative to this file; separator
     *        may be specified as a literal character or a "#"-prefixed Unicode
     *        codepoint.
     */
    enum Keyword {
        UNKNOWN,
        BACKGROUND,
        PERIOD,
        SPLIT_X,
        SPLIT_Y,
        CELL,
        PROTECT,
        INCLUDE
    };
    static const struct KeywordDesc {
        Keyword     code;
        const char *name;
    } KEYWORD_LIST[] = {
        { BACKGROUND, "background" },
        { PERIOD,     "period"     },
        { SPLIT_X,    "split-x"    },
        { CELL,       "cell"       },
        { INCLUDE,    "include"    },
        { PROTECT,    "protect"    },
        { UNKNOWN,    0            }
    };
    QRegExp fcard("#FCARD#\\s*(\\w+)\\s+(.*)");
    fcard.setPatternSyntax(QRegExp::RegExp2);
    int cardCount = 0;
    QStringList tagSet;
    CardDisplay *display = 0;
    CardDisplay currDisplay(defDisplay);
    for (;;)
    {
        int flags = 0;
        QStringList line = file.readLine(&flags);
        if (line.isEmpty())
        {
            if (!file.lastError().isEmpty())
            {
                qWarning("ERROR: %s", qPrintable(file.lastError()));
                return -1;
            }
            return cardCount;
        }
        if (flags == CsvFile::FLAG_COMMENT)
        {
            if (fcard.exactMatch(line.first()))
            {
                QString aname = fcard.cap(1);
                QString aval = fcard.cap(2);
                QStringList alist;
                int ret;
                if ((ret = TextConv::smartSplit(alist, aval)) != 0)
                {
                    qWarning("ERROR: Field-list syntax error in '%s' near offset %d",
                             qPrintable(aval), ret - 1);
                    continue;
                }
                Keyword code = UNKNOWN;
                for (const KeywordDesc *kwd = KEYWORD_LIST; kwd->name != 0; kwd++)
                    if (aname == kwd->name)
                    {
                        code = kwd->code;
                        break;
                    }
                if (currDisplay.isProtected(code))
                    continue;
                switch (code)
                {
                case UNKNOWN:
                    break;
                case BACKGROUND:
                    {
                        QColor bgd = QColor(aval.trimmed());
                        if (bgd.isValid())
                            currDisplay.setBackground(bgd);
                    }
                    break;
                case PERIOD:
                    {
                        bool ok = false;
                        int period = aval.toInt(&ok);
                        if (ok &&
                                period >= CardDisplay::MIN_PERIOD &&
                                period <= CardDisplay::MAX_PERIOD)
                            currDisplay.setPeriod(period);
                    }
                    break;
                case SPLIT_X:
                case SPLIT_Y:
                    {
                        QList<int> spl;
                        bool ok = TextConv::toIntList(alist, spl);
                        if (ok && !spl.isEmpty())
                        {
                            if (code == SPLIT_X)
                                currDisplay.setSplitX(spl);
                            else
                                currDisplay.setSplitY(spl);
                        }
                    }
                    break;
                case CELL:
                    if (!alist.isEmpty())
                    {
                        QString indVal = alist.takeFirst();
                        bool ok = false;
                        int ind = indVal.toInt(&ok);
                        if (ok && ind >= 0)
                        {
                            CardCell cell;
                            cell.assign(alist);
                            currDisplay.setValue(ind, cell);
                        }
                    }
                    break;
                case PROTECT:
                    if (!alist.isEmpty())
                    {
                        QString bVal = alist.takeFirst();
                        bool value;
                        if (bVal == "on")
                            value = true;
                        else if (bVal == "off")
                            value = false;
                        else
                            break;
                        foreach (QString name, alist)
                        {
                            for (const KeywordDesc *kwd = KEYWORD_LIST; kwd->name != 0; kwd++)
                                if (name == kwd->name && kwd->code != PROTECT)
                                {
                                    currDisplay.setProtected(kwd->code, value);
                                    break;
                                }
                        }
                    }
                    break;
                case INCLUDE:
                    if (!alist.isEmpty())
                    {
                        QChar sep = CsvFile::DEFAULT_SEPARATOR;
                        if (alist.size() > 1)
                            sep = TextConv::toChar(alist[1], sep);
                        CsvFile inclFile(alist[0], sep);
                        int inclCnt = loadCsv(currDisplay, inclFile);
                        if (inclCnt < 0)
                            return inclCnt;
                        cardCount += inclCnt;
                    }
                    break;
                }
            }
            continue;
        }
        if (flags == CsvFile::FLAG_ANKI)
        {
            tagSet = line;
            continue;
        }
        for (int i = 0; i < line.size(); i++)
            line.replace(i, line[i].replace('\n', "<br>"));
        line.append(tagSet);
        const CardDisplay* lastDisplay = display ? display : &defDisplay;
        if (currDisplay != *lastDisplay)
        {
            m_displays.append(currDisplay);
            display = &m_displays.last();
        }
        Card card(line, display);
        append(card);
        cardCount++;
    }
}
