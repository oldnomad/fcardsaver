#ifndef SETUPFILESMODEL_H
#define SETUPFILESMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QSet>
#include "util/csvfile.h"

class SetupFilesModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SetupFilesModel(const QList<CsvFile>& list, QObject* parent = 0);

    const QList<CsvFile> files() const { return m_list; }
    bool append(const CsvFile& file);
    void remove(const QSet<int>& rows);

    virtual int rowCount(const QModelIndex &parent) const
    {
        return parent.isValid() ? 0 : m_list.size();
    }
    virtual int columnCount(const QModelIndex &parent) const {
        (void)parent;
        return 2;
    }
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role) const;

private:
    QList<CsvFile> m_list;

    static bool itemComparator(const CsvFile& first, const CsvFile& second);
};

#endif // SETUPFILESMODEL_H
