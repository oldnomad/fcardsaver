#include <QEvent>
#include "setup/setupsplitdelegate.h"
#include "setup/setupsplitcell.h"
#include "setup/setupcelldialog.h"

SetupSplitDelegate::SetupSplitDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QString SetupSplitDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    if (value.canConvert<SetupSplitCell>())
    {
        SetupSplitCell pos = value.value<SetupSplitCell>();
        if (pos.index() < 0)
            return QString();
        return QString().sprintf("[%d]", pos.index());
    }
    return QStyledItemDelegate::displayText(value, locale);
}

bool SetupSplitDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index)
{
    QVariant value = index.data(Qt::EditRole);
    if (value.canConvert<SetupSplitCell>())
    {
        if ((option.state & QStyle::State_Enabled) == 0 ||
                (index.flags() & Qt::ItemIsEnabled) == 0)
            return false;
        if (event->type() != QEvent::MouseButtonDblClick &&
                event->type() != QEvent::KeyPress)
            return false;
        SetupSplitCell cell = value.value<SetupSplitCell>();
        SetupCellDialog dlg(cell);
        if (dlg.exec() == QDialog::Accepted)
        {
            cell = dlg.cardCell();
            cell.setPos(index.column() - 1, index.row() - 1);
            model->setData(index, qVariantFromValue(cell));
        }
        return true;
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
