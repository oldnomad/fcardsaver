#include <QPainter>
#include <QTextDocument>
#include <QMouseEvent>
#include <QTime>
#include "saverwindow.h"

SaverWindow::SaverWindow(WId window, bool keymode, const SaverSettings& set) :
    m_keymode(keymode), m_lastx(-1), m_lasty(-1), m_set(set),
    m_index(0), m_card(), m_display(), m_timer()
{
    create(window);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    if (m_keymode)
    {
        setFocusPolicy(Qt::StrongFocus);
        setMouseTracking(true);
    }
    qsrand(QTime::currentTime().msec());
    m_timer.setSingleShot(true);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timerUpdate();
}

void SaverWindow::keyPressEvent(QKeyEvent *event)
{
    (void)event;    // UNUSED
    if (!m_keymode)
        return;
    close();
}

void SaverWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_keymode)
        return;
    int x = event->globalX(), y = event->globalY();
    if (m_lastx >= 0 || m_lasty >= 0)
    {
        int dx = x < m_lastx ? m_lastx - x : x - m_lastx;
        int dy = y < m_lasty ? m_lasty - y : y - m_lasty;
        if (dx > 2 || dy > 2)
            close();
    }
    m_lastx = x;
    m_lasty = y;
}

void SaverWindow::focusOutEvent(QFocusEvent *event)
{
    (void)event;    // UNUSED
    if (!m_keymode)
        return;
    close();
}

void SaverWindow::timerUpdate()
{
    m_timer.start(nextCard());
    QMetaObject::invokeMethod(this, "repaint", Qt::AutoConnection);
}

int SaverWindow::nextCard()
{
    m_index = qrand();
    m_card = m_set.cardSet().get(m_index);
    m_display = m_set;
    m_display += m_card.display();
    return m_display.period();
}

SaverWindow::paint_context SaverWindow::buildPaint() const
{
    const QList<int>& xspl = m_display.splitX();
    const QList<int>& yspl = m_display.splitY();
    int xmax = width(), ymax = height();

    paint_context ctxt;
    int xsum = 0, ysum = 0;
    foreach (int v, xspl)
        xsum += v;
    foreach (int v, yspl)
        ysum += v;
    if (xsum == 0 || ysum == 0)
        return ctxt;

    ctxt.xcnt = xspl.size();
    ctxt.ycnt = yspl.size();

    ctxt.xpos.reserve(ctxt.xcnt + 1);
    for (int i = 0, off = 0; i < ctxt.xcnt; i++)
    {
        int x = xspl.at(i);
        off += (x < 0 ? -x : (xmax*x/xsum));
        ctxt.xpos.append(off);
    }
    ctxt.xpos.append(xmax);

    ctxt.ypos.reserve(ctxt.ycnt + 1);
    for (int i = 0, off = 0; i < ctxt.ycnt; i++)
    {
        int y = yspl.at(i);
        off += (y < 0 ? -y : (ymax*y/ysum));
        ctxt.ypos.append(off);
    }
    ctxt.ypos.append(ymax);

    return ctxt;
}

void SaverWindow::paintText(QPainter* paint, const paint_context &ctxt,
                            const CardCell &disp, const QString &text) const
{
    if (!disp.isValid() || disp.pos_x() > ctxt.xcnt || disp.pos_y() > ctxt.ycnt)
        return;
    QRectF clip(0, 0, 0, 0);
    if (disp.pos_x() > 0)
        clip.setLeft(ctxt.xpos[disp.pos_x() - 1]);
    if (disp.pos_y() > 0)
        clip.setTop(ctxt.ypos[disp.pos_y() - 1]);
    clip.setRight(ctxt.xpos[disp.pos_x()]);
    clip.setBottom(ctxt.ypos[disp.pos_y()]);
    if (clip.isEmpty())
        return;
    QTextDocument doc;
    paint->save();
    QTextOption opt(Qt::AlignCenter);
    opt.setWrapMode(QTextOption::WordWrap);
    doc.setDefaultTextOption(opt);
    QColor textColor = disp.textColor();
    QString style;
    if (textColor.isValid() && textColor.alpha() != 0)
        style = SaverSettings::TEXTCELL_STYLE.arg(textColor.name());
    style += disp.stylesheet();
    if (!style.isEmpty())
        doc.setDefaultStyleSheet(style);
    {
        QString fontFamily = disp.fontFamily();
        double fsize = disp.fontLines();
        if (fsize < 1)
            fsize = 1;
        fsize = SaverSettings::TEXTCELL_SCALE*clip.height()/fsize;
        QFont font(fontFamily);
        font.setPixelSize((int)fsize);
        doc.setDefaultFont(font);
    }
    QString altText = text;
    QString pattern = disp.pattern();
    // TODO: More intelligent pattern for plain text
    if (!pattern.isEmpty())
        altText = pattern.arg(text);
    else if (!text.startsWith('<') || !text.endsWith('>'))
        altText = SaverSettings::PLAINTEXT_PATTERN.arg(text);
    doc.setHtml(altText);
    QSizeF csize = clip.size()*SaverSettings::TEXTCELL_SCALE;
    doc.setTextWidth(csize.width());
    QSizeF dsize = doc.size();
    while (dsize.width() > csize.width() ||
           dsize.height() > csize.height())
    {
        QFont font = doc.defaultFont();
        QFontInfo finf(font);
        font.setPixelSize(finf.pixelSize() - 1);
        doc.setDefaultFont(font);
        dsize = doc.size();
    }
    paint->translate(clip.topLeft());
    clip = clip.translated(-clip.topLeft());
    if (dsize.width() < clip.width())
    {
        paint->translate((clip.width() - dsize.width())/2, 0);
        clip.setWidth(dsize.width());
    }
    if (dsize.height() < clip.height())
    {
        paint->translate(0, (clip.height() - dsize.height())/2);
        clip.setHeight(dsize.height());
    }
    doc.drawContents(paint, clip);
    if (m_set.trace().alpha() != 0)
    {
        paint->setPen(m_set.trace());
        paint->drawRect(clip);
    }
    paint->restore();
}

void SaverWindow::paintEvent(QPaintEvent *event)
{
    (void)event;    // UNUSED

    qDebug("Paint called");
    QPainter p(this);
    p.setRenderHint(QPainter::TextAntialiasing);
    p.setBackground(QBrush(m_display.background()));
    p.eraseRect(QRectF(0, 0, width(), height()));

    paint_context ctxt = buildPaint();

    QColor trace = m_set.trace();
    if (trace.alpha() != 0)
    {
        p.setPen(trace);
        for (int i = 0; i < ctxt.xcnt; i++)
            p.drawLine(ctxt.xpos[i], 0, ctxt.xpos[i], ctxt.ypos[ctxt.ycnt]);
        for (int i = 0; i < ctxt.ycnt; i++)
            p.drawLine(0, ctxt.ypos[i], ctxt.xpos[ctxt.xcnt], ctxt.ypos[i]);
    }

    int last_index = m_display.maxIndex();
    for (int i = 0; i <= last_index; i++)
        paintText(&p, ctxt, m_display.value(i), m_card.value(i));
}
