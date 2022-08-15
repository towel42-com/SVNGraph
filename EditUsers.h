#ifndef CEditUsers_H
#define CEditUsers_H

#include <QDialog>
#include <QItemDelegate>
#include <memory>
class QSqlTableModel;
class QCheckBox;
namespace Ui{ class CEditUsers; }

class CCheckBoxDelegate: public QItemDelegate
{
    Q_OBJECT
public:
    CCheckBoxDelegate(QObject *parent = 0);

    void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
    QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
    void setEditorData( QWidget *editor, const QModelIndex &index ) const;
    void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;
    void updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

public slots:
    void setData(bool val);
private:
    mutable QCheckBox * fCheckBox;
};

class CEditUsers : public QDialog
{
    Q_OBJECT
public:
    CEditUsers( QWidget *parent = 0);
    ~CEditUsers();

    virtual void accept() override;
private:
    QSqlTableModel * fModel{ nullptr };
    std::unique_ptr< Ui::CEditUsers > fImpl;
};

#endif // ORDERPROCESSOR_H
