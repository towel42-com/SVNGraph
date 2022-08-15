#include "EditUsers.h"
#include "ui_EditUsers.h"  
#include "AutoWaitCursor.h"

#include "SVNModel.h"

#include <QTimer> 
#include <QLineSeries>
#include <QCheckBox>
#include <map>
#include <QDebug>

CEditUsers::CEditUsers( QWidget *parent )
    : QDialog( parent ),
    fImpl( new Ui::CEditUsers )
{
    fImpl->setupUi( this );
    fModel = new QSqlTableModel( this );
    fModel->setTable( "Authors" );
    fModel->setEditStrategy( QSqlTableModel::OnManualSubmit );
    fModel->select();
    fImpl->table->setModel( fModel );
    fImpl->table->hideColumn( 0 );
    fImpl->table->sortByColumn( 1, Qt::AscendingOrder );

    fImpl->table->setItemDelegateForColumn( 2, new CCheckBoxDelegate( fImpl->table ) );

    setWindowIcon( QIcon( ":/resources/svn.png" ) );
}

CEditUsers::~CEditUsers()
{
}

void CEditUsers::accept()
{
    fModel->submitAll();
    QDialog::accept();
}

CCheckBoxDelegate::CCheckBoxDelegate( QObject *parent ) :
QItemDelegate( parent )
{
}

void CCheckBoxDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    drawDisplay( painter, option, option.rect, index.model()->data( index, Qt::DisplayRole ).toBool() ? QString( "      " ).append( tr( "Yes" ) ) : QString( "      " ).append( tr( "No" ) ) );
    drawFocus( painter, option, option.rect );
}

QWidget *CCheckBoxDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/ ) const
{
    fCheckBox = new QCheckBox( parent );
    QObject::connect( fCheckBox, SIGNAL( toggled( bool ) ), this, SLOT( setData( bool ) ) );
    return fCheckBox;
}

void CCheckBoxDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
    int val = index.model()->data( index, Qt::DisplayRole ).toInt();

    (static_cast<QCheckBox*>(editor))->setChecked( val );

}

void CCheckBoxDelegate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    model->setData( index, (int)(static_cast<QCheckBox*>(editor)->isChecked()) );
}


void CCheckBoxDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/ ) const
{
    editor->setGeometry( option.rect );
}

void CCheckBoxDelegate::setData( bool /*val*/ )
{
    emit commitData( fCheckBox );
}


