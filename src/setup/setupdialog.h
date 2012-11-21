#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>
#include <QAbstractTableModel>
#include "saversettings.h"

namespace Ui {
    class SetupDialog;
}

class SetupSplitModel;
class SetupFilesModel;
class SetupSplitDelegate;

class SetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupDialog(SaverSettings& set, QWidget *parent = 0);
    ~SetupDialog();

public slots:
    virtual void accept();

private slots:
    void cellRowCount(int value);
    void cellColumnCount(int value);
    void csetAddClicked();
    void csetDelClicked();

private:
    static const QString FILE_FILTERS;

    Ui::SetupDialog*    m_ui;
    SaverSettings&      m_set;
    SetupSplitModel*    m_split_model;
    SetupSplitDelegate* m_split_delegate;
    SetupFilesModel*    m_files_model;
};

#endif // SETUPDIALOG_H
