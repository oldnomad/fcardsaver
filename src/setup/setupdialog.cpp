#include <QFileDialog>
#include <QFileInfo>
#include <QColorDialog>
#include "setup/setupdialog.h"
#include "setup/setupfilesmodel.h"
#include "setup/setupsplitmodel.h"
#include "setup/setupsplitdelegate.h"
#include "platform/startup.h"
#include "ui_setupdialog.h"

const QString SetupDialog::FILE_FILTERS(
        "CSV files (*.csv *.tsv);;"
        "Text files (*.txt);;"
        "All files (*)");

SetupDialog::SetupDialog(SaverSettings& set, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SetupDialog),
    m_set(set),
    m_split_model(0),
    m_split_delegate(new SetupSplitDelegate),
    m_files_model(new SetupFilesModel(set.cardSetFiles(), this))
{
    m_ui->setupUi(this);

    m_ui->periodSpinner->setMinimum(CardDisplay::MIN_PERIOD/1000.0);
    m_ui->periodSpinner->setMaximum(CardDisplay::MAX_PERIOD/1000.0);
    m_ui->periodSpinner->setValue(m_set.cardDisplay().period()/1000.0);
    m_ui->colorButton->setColorValue(m_set.cardDisplay().background());

    m_ui->rowsSpinner->setValue(m_set.cardDisplay().splitX().size());
    m_ui->colsSpinner->setValue(m_set.cardDisplay().splitY().size());
    m_split_model = new SetupSplitModel(m_set.cardDisplay(), this);
    m_ui->cellTable->setModel(m_split_model);
    m_ui->cellTable->setItemDelegate(m_split_delegate);
    m_ui->cellTable->resizeColumnsToContents();
    connect(m_ui->rowsSpinner, SIGNAL(valueChanged(int)), this,
            SLOT(cellRowCount(int)));
    connect(m_ui->colsSpinner, SIGNAL(valueChanged(int)), this,
            SLOT(cellColumnCount(int)));

    m_ui->csetTable->setModel(m_files_model);
    m_ui->csetTable->resizeColumnsToContents();

    connect(m_ui->csetAddBtn, SIGNAL(clicked()), this, SLOT(csetAddClicked()));
    connect(m_ui->csetDelBtn, SIGNAL(clicked()), this, SLOT(csetDelClicked()));
}

SetupDialog::~SetupDialog()
{
    delete m_ui;
    delete m_split_delegate;
}

void SetupDialog::cellRowCount(int value)
{
    int oldcnt = m_split_model->rowCount();
    int newcnt = value + 1;
    if (newcnt == oldcnt)
        return;
    if (newcnt < oldcnt)
        m_split_model->removeRows(newcnt, oldcnt - newcnt);
    else
        m_split_model->insertRows(oldcnt, newcnt - oldcnt);
}

void SetupDialog::cellColumnCount(int value)
{
    int oldcnt = m_split_model->columnCount();
    int newcnt = value + 1;
    if (newcnt == oldcnt)
        return;
    if (newcnt < oldcnt)
        m_split_model->removeColumns(newcnt, oldcnt - newcnt);
    else
        m_split_model->insertColumns(oldcnt, newcnt - oldcnt);
}

void SetupDialog::csetAddClicked()
{
    QDir rootDir = startup_t::get_data_root();
    QStringList files =
            QFileDialog::getOpenFileNames(this, QString(), rootDir.path() + '/',
                                          FILE_FILTERS);
    for (int i = 0; i < files.size(); i++)
    {
        QString path = files[i];
        QString frel = rootDir.relativeFilePath(path);
        if (!frel.startsWith(".."))
            path = frel;
        m_files_model->append(CsvFile(path));
    }
}

void SetupDialog::csetDelClicked()
{
    QModelIndexList cells = m_ui->csetTable->selectionModel()->selectedIndexes();
    QSet<int> rows;
    foreach (QModelIndex index, cells)
        rows.insert(index.row());
    m_files_model->remove(rows);
}

void SetupDialog::accept()
{
    CardDisplay& display = m_set.cardDisplay();
    QColor bgd = m_ui->colorButton->colorValue();
    if (bgd.isValid() && bgd.alpha() != 0)
        display.setBackground(bgd);
    display.setPeriod((int)(m_ui->periodSpinner->value()*1000));
    display.setSplitX(m_split_model->display().splitX());
    display.setSplitY(m_split_model->display().splitY());
    display.setValues(m_split_model->display());
    m_set.setCardSetFiles(m_files_model->files());
    QDialog::accept();
}
