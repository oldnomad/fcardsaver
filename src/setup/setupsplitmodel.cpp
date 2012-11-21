#include <algorithm>
#include "setup/setupsplitmodel.h"

SetupSplitModel::SetupSplitModel(const CardDisplay& display, QObject *parent) :
    QAbstractTableModel(parent), m_display(display)
{
}

Qt::ItemFlags SetupSplitModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QAbstractTableModel::flags(index);
    if (index.row() != 0 || index.column() != 0)
        f |= Qt::ItemIsEditable;
    return f;
}

const SetupSplitCell SetupSplitModel::findCell(int row, int column) const
{
    for (int i = 0; i <= m_display.maxIndex(); i++)
    {
        CardCell cell = m_display.value(i);
        if (cell.pos_x() == column && cell.pos_y() == row)
            return SetupSplitCell(i, cell);
    }
    return SetupSplitCell();
}

const SetupSplitCell SetupSplitModel::findCell(int index) const
{
    CardCell cell = m_display.value(index);
    if (!cell.isValid())
        return SetupSplitCell();
    return SetupSplitCell(index, cell);
}

QVariant SetupSplitModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::ToolTipRole &&
            role != Qt::StatusTipRole && role != Qt::EditRole)
        return QVariant();
    if (index.model() != this || (index.row() == 0 && index.column() == 0))
        return QVariant();
    if (index.row() == 0)
        return QVariant(m_display.splitX().at(index.column() - 1));
    if (index.column() == 0)
        return QVariant(m_display.splitY().at(index.row() - 1));
    SetupSplitCell pos = findCell(index.row() - 1, index.column() - 1);
    return qVariantFromValue(pos);
}

bool SetupSplitModel::setData(const QModelIndex &index, const QVariant &value,
                              int role)
{
    if (role != Qt::EditRole)
        return false;
    if (index.model() != this || (index.row() == 0 && index.column() == 0))
        return false;
    if (index.row() == 0)
    {
        m_display.setSplitX(index.column() - 1, value.toInt());
        emit dataChanged(index, index);
        return true;
    }
    if (index.column() == 0)
    {
        m_display.setSplitY(index.row() - 1, value.toInt());
        emit dataChanged(index, index);
        return true;
    }
    SetupSplitCell cell = qVariantValue<SetupSplitCell>(value);
    if (cell.index() == Card::INVALID_INDEX)
    {
        // Delete cell
        SetupSplitCell oldCell = findCell(index.row() - 1, index.column() - 1);
        if (oldCell.index() == Card::INVALID_INDEX)
            return false;
        m_display.removeValue(oldCell.index());
        emit dataChanged(index, index);
        return true;
    }
    SetupSplitCell oldCell = findCell(cell.index());
    m_display.setValue(cell.index(), cell);
    if (!oldCell.isValid())
        emit dataChanged(index, index);
    else
    {
        int row1 = oldCell.pos_y() + 1, row2 = cell.pos_y() + 1;
        int col1 = oldCell.pos_x() + 1, col2 = cell.pos_x() + 1;
        if (row1 > row2)
            std::swap(row1, row2);
        if (col1 > col2)
            std::swap(col1, col2);
        emit dataChanged(createIndex(row1, col1), createIndex(row2, col2));
    }
    return true;
}

bool SetupSplitModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row < 1 || count < 1)
        return false;
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; i++)
        m_display.addSplitY(row - 1, 1);
    endInsertRows();
    return true;
}

bool SetupSplitModel::removeRows(int row, int count, const QModelIndex &parent)
{
    int nrows = m_display.splitY().size();
    if (row < 1 || row > nrows || count < 1)
        return false;
    if ((row + count - 1) > nrows)
        count = nrows - row + 1;
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; i++)
        m_display.delSplitY(row - 1);
    endRemoveRows();
    return true;
}

bool SetupSplitModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    if (column < 1 || count < 1)
        return false;
    beginInsertColumns(parent, column, column + count - 1);
    for (int i = 0; i < count; i++)
        m_display.addSplitX(column - 1, 1);
    endInsertColumns();
    return true;
}

bool SetupSplitModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    int ncols = m_display.splitX().size();
    if (column < 1 || column > ncols || count < 1)
        return false;
    if ((column + count - 1) > ncols)
        count = ncols - column + 1;
    beginRemoveColumns(parent, column, column + count - 1);
    for (int i = 0; i < count; i++)
        m_display.delSplitX(column - 1);
    endRemoveColumns();
    return true;
}

QVariant SetupSplitModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    (void)orientation;
    if (role != Qt::DisplayRole && role != Qt::ToolTipRole &&
            role != Qt::StatusTipRole)
        return QVariant();
    if (section == 0)
        return QVariant(QString("Size"));
    return QVariant(section);
}
