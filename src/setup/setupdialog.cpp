#include <QFileDialog>
#include <QFileInfo>
#include <QColorDialog>
#include "setup/setupdialog.h"
#include "setup/setupfilesmodel.h"
#include "setup/setupsplitmodel.h"
#include "setup/setupsplitdelegate.h"
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
    m_ui->periodSpinner->setValue(m_set.period()/1000.0);
    m_ui->colorButton->setColorValue(m_set.background());

    m_ui->rowsSpinner->setValue(m_set.splitX().size());
    m_ui->colsSpinner->setValue(m_set.splitY().size());
    m_split_model = new SetupSplitModel(m_set, this);
    m_ui->cellTable->setModel(m_split_model);
    m_ui->cellTable->setItemDelegate(m_split_delegate);
    m_ui->cellTable->resizeColumnsToContents();
    connect(m_ui->rowsSpinner, SIGNAL(valueChanged(int)), this,
            SLOT(cellRowCount(int)));
    connect(m_ui->colsSpinner, SIGNAL(valueChanged(int)), this,
            SLOT(cellColumnCount(int)));

    m_ui->csetRoot->setText(set.cardSetRoot().absolutePath());
    connect(m_ui->csetRoot, SIGNAL(editingFinished()), this,
            SLOT(csetRootChanged()));
    connect(m_ui->csetRootButton, SIGNAL(clicked()), this,
            SLOT(csetRootSelect()));
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

void SetupDialog::csetRootChanged()
{
    QDir rootDir(m_ui->csetRoot->text());
    if (!rootDir.makeAbsolute() ||
            !rootDir.exists() ||
            !rootDir.isReadable())
    {
        m_ui->csetRoot->setText(m_set.cardSetRoot().absolutePath());
        return;
    }
    m_set.setCardSetRoot(rootDir);
}

void SetupDialog::csetRootSelect()
{
    QDir rootDir = m_set.cardSetRoot();
    QString path = QFileDialog::getExistingDirectory(this, "Data root directory",
                                                     rootDir.absolutePath(),
                                                     QFileDialog::ShowDirsOnly|
                                                     QFileDialog::DontResolveSymlinks);
    if (path.isNull())
        return;
    rootDir.setPath(path);
    if (!rootDir.makeAbsolute() ||
            !rootDir.exists() ||
            !rootDir.isReadable())
        return;
    m_set.setCardSetRoot(rootDir);
    m_ui->csetRoot->setText(rootDir.absolutePath());
}

void SetupDialog::csetAddClicked()
{
    QDir rootDir = m_set.cardSetRoot();
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
    QColor bgd = m_ui->colorButton->colorValue();
    if (bgd.isValid() && bgd.alpha() != 0)
        m_set.setBackground(bgd);
    m_set.setPeriod((int)(m_ui->periodSpinner->value()*1000));
    m_set.setSplitX(m_split_model->display().splitX());
    m_set.setSplitY(m_split_model->display().splitY());
    m_set.setValues(m_split_model->display());
    m_set.setCardSetFiles(m_files_model->files());
    QDialog::accept();
}
