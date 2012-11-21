#include "setup/setupfilesmodel.h"
#include "util/textconv.h"

SetupFilesModel::SetupFilesModel(const QList<CsvFile> &list, QObject *parent) :
    QAbstractTableModel(parent), m_list(list)
{
    qSort(m_list.begin(), m_list.end(), itemComparator);
}

Qt::ItemFlags SetupFilesModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index)|Qt::ItemIsEditable;
}

QVariant SetupFilesModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::ToolTipRole &&
            role != Qt::StatusTipRole && role != Qt::EditRole)
        return QVariant();
    if (index.model() != this || index.row() < 0 || index.row() >= m_list.size())
        return QVariant();
    const CsvFile& f = m_list[index.row()];
    switch (index.column())
    {
    case 0:
        return QVariant(f.path());
    case 1:
        return QVariant(TextConv::fromChar(f.separator()));
    }
    return QVariant();
}

bool SetupFilesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;
    if (index.model() != this || index.row() < 0 || index.row() >= m_list.size())
        return false;
    int oldIdx = index.row();
    CsvFile f = m_list[oldIdx];
    switch (index.column())
    {
    case 0:
        f.setPath(value.toString());
        break;
    case 1:
        {
        QChar sep = TextConv::toChar(value.toString(), f.separator());
            if (f.separator() == sep)
                return false;
            f.setSeparator(sep);
        }
        break;
    default:
        return false;
    }
    QList<CsvFile>::iterator pos =
        qUpperBound(m_list.begin(), m_list.end(), f, itemComparator);
    int posIdx = pos - m_list.begin();
    m_list[oldIdx] = f;
    emit dataChanged(index, index);
    int newIdx = posIdx;
    if (oldIdx == 0 || itemComparator(m_list[oldIdx - 1], f))
    {
        if ((oldIdx + 1) >= m_list.size() || itemComparator(f, m_list[oldIdx + 1]))
            return true;
        newIdx--;
    }
    beginMoveRows(index.parent(), oldIdx, oldIdx, index.parent(), posIdx);
    m_list.move(oldIdx, newIdx);
    endMoveRows();
    return true;
}

QVariant SetupFilesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::ToolTipRole &&
            role != Qt::StatusTipRole)
        return QVariant();
    if (orientation != Qt::Horizontal)
        return QVariant(section + 1);
    switch (section)
    {
    case 0:
        return QVariant("File path");
    case 1:
        return QVariant("Separator");
    }
    return QVariant();
}

bool SetupFilesModel::itemComparator(const CsvFile &first, const CsvFile &second)
{
    if (first.path() < second.path())
        return true;
    if (first.path() == second.path() && first.separator() < second.separator())
        return true;
    return false;
}

bool SetupFilesModel::append(const CsvFile &file)
{
    QList<CsvFile>::iterator pos =
            qUpperBound(m_list.begin(), m_list.end(), file, itemComparator);
    if (pos != m_list.end() &&
            pos->path() == file.path() &&
            pos->separator() == file.separator())
        return false;
    int index = pos - m_list.begin();
    beginInsertRows(QModelIndex(), index, index);
    m_list.insert(pos, file);
    endInsertRows();
    return true;
}

void SetupFilesModel::remove(const QSet<int>& rows)
{
    if (rows.isEmpty())
        return;
    QList<int> rowList = rows.values();
    qSort(rowList);
    for (int i = rowList.size() - 1; i >= 0; i--)
    {
        beginRemoveRows(QModelIndex(), rowList[i], rowList[i]);
        m_list.removeAt(rowList[i]);
        endRemoveRows();
    }
}
