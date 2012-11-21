#ifndef SETUPCELLDIALOG_H
#define SETUPCELLDIALOG_H

#include <QDialog>
#include <QStringList>
#include "setup/setupsplitcell.h"

namespace Ui {
    class SetupCellDialog;
}

class SetupCellDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SetupCellDialog(const SetupSplitCell& cell, QWidget *parent = 0);
    ~SetupCellDialog();

    const SetupSplitCell& cardCell() const { return m_cell; }

public slots:
    virtual void accept();

private slots:
    void indexChanged(int state);

private:
    Ui::SetupCellDialog *m_ui;
    SetupSplitCell m_cell;
};

#endif // SETUPCELLDIALOG_H
