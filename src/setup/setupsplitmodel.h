#ifndef SETUPSPLITMODEL_H
#define SETUPSPLITMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "card/carddisplay.h"
#include "setup/setupsplitcell.h"

class SetupSplitModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SetupSplitModel(const CardDisplay& display, QObject *parent = 0);

    const CardDisplay& display() const { return m_display; }

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        return parent.isValid() ? 0 : (m_display.splitY().size() + 1);
    }
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const {
        return parent.isValid() ? 0 : (m_display.splitX().size() + 1);
    }
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value,
                         int role = Qt::EditRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    virtual bool insertRows(int row, int count,
                            const QModelIndex &parent = QModelIndex());
    virtual bool removeRows(int row, int count,
                            const QModelIndex &parent = QModelIndex());
    virtual bool insertColumns(int column, int count,
                               const QModelIndex &parent = QModelIndex());
    virtual bool removeColumns(int column, int count,
                               const QModelIndex &parent = QModelIndex());

signals:
    
public slots:

private:
    const SetupSplitCell findCell(int row, int column) const;
    const SetupSplitCell findCell(int index) const;

    CardDisplay m_display;
};

#endif // SETUPSPLITMODEL_H
