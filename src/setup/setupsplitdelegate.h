#ifndef SETUPSPLITDELEGATE_H
#define SETUPSPLITDELEGATE_H

#include <QStyledItemDelegate>

class SetupSplitDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SetupSplitDelegate(QObject *parent = 0);

    virtual QString displayText(const QVariant &value, const QLocale &locale) const;

protected:
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model,
                             const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    
public slots:
    
};

#endif // SETUPSPLITDELEGATE_H
