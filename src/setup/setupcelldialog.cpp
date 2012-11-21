#include "setup/setupcelldialog.h"
#include "ui_setupcelldialog.h"

SetupCellDialog::SetupCellDialog(const SetupSplitCell& cell, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SetupCellDialog),
    m_cell(cell)
{
    m_ui->setupUi(this);
    int index = cell.index();
    if (index < 0)
    {
        m_ui->indexCheck->setChecked(false);
        m_ui->indexSpinner->setEnabled(false);
    }
    else
    {
        m_ui->indexCheck->setChecked(true);
        m_ui->indexSpinner->setEnabled(true);
        m_ui->indexSpinner->setValue(cell.index());
    }
    connect(m_ui->indexCheck, SIGNAL(stateChanged(int)), this,
            SLOT(indexChanged(int)));

    m_ui->colorButton->setColorValue(cell.textColor());
    // XXX: HACK see <https://bugreports.qt-project.org/browse/QTBUG-13369>
    m_ui->fontComboBox->setCurrentFont(QFont(cell.fontFamily()));
    m_ui->fontComboBox->setCurrentFont(QFont(cell.fontFamily()));
    // XXX: END OF HACK
    m_ui->lineSpinner->setValue(cell.fontLines());
    m_ui->patternEdit->setPlainText(cell.pattern());
    m_ui->stylesheetEdit->setPlainText(cell.stylesheet());
}

SetupCellDialog::~SetupCellDialog()
{
    delete m_ui;
}

void SetupCellDialog::indexChanged(int state)
{
    m_ui->indexSpinner->setEnabled(state == Qt::Checked);
}

void SetupCellDialog::accept()
{
    if (m_ui->indexCheck->checkState() == Qt::Checked)
    {
        int index = m_ui->indexSpinner->value();
        m_cell.setIndex(index);
        m_cell.setTextColor(m_ui->colorButton->colorValue());
        m_cell.setFontFamily(m_ui->fontComboBox->currentFont().family());
        m_cell.setFontLines(m_ui->lineSpinner->value());
        m_cell.setPattern(m_ui->patternEdit->document()->toPlainText());
        m_cell.setStylesheet(m_ui->stylesheetEdit->document()->toPlainText());
    }
    else
        m_cell.setIndex(Card::INVALID_INDEX);
    QDialog::accept();
}
