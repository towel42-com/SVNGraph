#include "MainWindow.h"
#include "SVNModel.h"
#include "AutoWaitCursor.h"
#include "runCmd.h"
#include "ButtonEnabler.h"
#include "SVNLoader.h"
#include "EditUsers.h"
#include "ChartWindow.h"

#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QSqlTableModel>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QDebug>
#include <QDate>
#include <QHeaderView>
#include <QIcon>
#include <QItemSelectionModel>
#include <QTimer>
#include <QInputDialog>



CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent),
    fImpl( new Ui::CMainWindow )
{
    fImpl->setupUi(this);
    setWindowIcon( QIcon( ":/resources/svn.png" ) );
    initDatabase();

    connect( fModel.get(), SIGNAL( sigUpdateValues() ), this, SLOT( slotValuesChanged() ) );
    fImpl->tableView->setModel( fModel.get() );
    fImpl->tableView->hideColumn( eID );
   
    fImpl->tableView->horizontalHeader()->setSortIndicator( eRevision, Qt::DescendingOrder );

    connect( fImpl->loadFromSVN, SIGNAL( clicked() ), fModel.get(), SLOT( slotLoadFromSVN() ) );
    connect( fImpl->stopLoading, SIGNAL( clicked() ), fModel.get(), SLOT( slotStopLoading() ) );
    connect( fImpl->refresh, SIGNAL( clicked() ), fModel.get(), SLOT( slotRefresh() ) );
    connect( fModel.get(), SIGNAL( sigStatus( const QString & ) ), this, SLOT( slotStatus( const QString & ) ) );
    connect( fModel.get(), SIGNAL( sigRunning( bool ) ), this, SLOT( slotModelRunning( bool ) ) );
    connect( fImpl->threadCount, SIGNAL( valueChanged( int ) ), fModel.get(), SLOT( slotSetMaxNumThreads( int ) ) );
    connect( fImpl->delayTimeMinutes, SIGNAL( valueChanged( int ) ), fModel->getLoader(), SLOT( slotSetWaitTime( int ) ) );

    connect( fImpl->deleteRows, SIGNAL( clicked() ), this, SLOT( slotDeleteRows() ) );
    connect( fImpl->actionCurrentDevsOnly, SIGNAL( triggered() ), fModel.get(), SLOT( slotCurrentDevsOnly() ) );
    connect( fImpl->actionFailedAnalysisOnly, SIGNAL( triggered() ), fModel.get(), SLOT( slotFailedAnalysisOnly() ) );
    connect( fImpl->actionViewGraph, SIGNAL( triggered() ), this, SLOT( slotViewGraph() ) );
    connect( fImpl->actionEditCurrentUsers, SIGNAL( triggered() ), this, SLOT( slotEditCurrentUsers() ) );

    fModel->slotSetMaxNumThreads( fImpl->threadCount->value() );
    fModel->getLoader()->slotSetWaitTime( fImpl->delayTimeMinutes->value() );
    fImpl->actionCurrentDevsOnly->setChecked( fModel->currentDevsOnly() );
    fImpl->actionFailedAnalysisOnly->setChecked( fModel->failedAnalysisOnly() );
    setAttribute(Qt::WA_DeleteOnClose);
    slotModelRunning( false );
}

CMainWindow::~CMainWindow()
{
}

void CMainWindow::slotValuesChanged()
{
    fImpl->loadProgressBar->setValue( fModel->lastLoaded() );
    fImpl->loadProgressBar->setMaximum( fModel->numEntries() );
    fImpl->lastLoadedLabel->setText( tr( "Last Revision Loaded: %1 - %2" ).arg( fModel->lastLoaded() ).arg( fModel->lastLoadedDate().toString( "MM/dd/yyyy" ) ) );
    fImpl->numFailed->setText( tr( "Number of Failed Analysis Entries: %1" ).arg( fModel->numFailedEntries() ) );
    fImpl->totalRevisionsLabel->setText( tr( "Total Number of Revisions: %1" ).arg( fModel->numEntries() ) );
    fImpl->summaryLabel->setText( tr( "Summary: %1" ).arg( fModel->getSummary() ) );
    fImpl->threadsLabel->setText( tr( "Threads Running: %1" ).arg( fModel->getThreadSummary() ) );
}

void CMainWindow::initDatabase()
{
    QDir dataDir = QDir( QCoreApplication::applicationDirPath() );
    QString envDir = qgetenv( "SVNGRAPHDIR" );
    if ( !envDir.isEmpty() )
        dataDir = QDir( envDir );
    if ( !dataDir.exists() && !dataDir.mkpath( "." ) )
    {
        QMessageBox::warning( nullptr, tr( "Could not create data directory" ), tr( "Could not create data directory : %1" ).arg( dataDir.absolutePath( ) ) );
        return;
    }

    QString dbPath = dataDir.absoluteFilePath( "svngraph.db" );
    QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setDatabaseName( dbPath );
    if( !db.open() )
    {
        QMessageBox::warning( nullptr, tr( "Could not open database" ), tr( "Could not database: %1" ).arg( dbPath ) );
        return;
    }

    QSqlQuery query( 
        "CREATE TABLE IF NOT EXISTS Authors( "
            "AuthorID INTEGER PRIMARY KEY AUTOINCREMENT, "
            "AuthorName TEXT,"
            "Current Integer"
            ")" );

    if ( !runCmd( query ) )
        return;

    query = QSqlQuery( "CREATE INDEX IF NOT EXISTS IDX_AUTHORS ON Authors( AuthorName )" );
    if ( !runCmd( query ) )
        return;

    query = QSqlQuery( 
        "CREATE TABLE IF NOT EXISTS ExtensionDefs( "
            "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
            "Name TEXT"
            ")" );

    if ( !runCmd( query ) )
        return;

    query = QSqlQuery( "CREATE INDEX IF NOT EXISTS IDX_EXTENSIONDEFS ON ExtensionDefs( Name )" );
    if ( !runCmd( query ) )
        return;

    query = QSqlQuery( 
        "CREATE TABLE IF NOT EXISTS LinesOfCode( "
            "Revision INTEGER, "
            "ExtensionID INTEGER, "
            "NumInserted INTEGER, "
            "NumDeleted INTEGER, "
            "NumModified INTEGER"
            ")" );

    if ( !runCmd( query ) )
        return;

    query = QSqlQuery( "CREATE INDEX IF NOT EXISTS IDX_LINESOFCODE_REVISION ON LinesOfCode( Revision )" );
    if ( !runCmd( query ) )
        return;

    query = QSqlQuery( "CREATE INDEX IF NOT EXISTS IDX_LINESOFCODE_REV_EXT ON LinesOfCode( Revision, ExtensionID )" );
    if ( !runCmd( query ) )
        return;

    query = QSqlQuery( 
        "CREATE TABLE IF NOT EXISTS SVNEntries( "
            "EntryID INTEGER PRIMARY KEY AUTOINCREMENT, "
            "Revision INTEGER NOT NULL, "
            "AuthorID INTEGER NOT NULL, "
            "Date TEXT NOT NULL, "
            "NumInserted INTEGER, "
            "NumDeleted INTEGER, "
            "NumModified INTEGER, "
            "SkippedAnalysis INTEGER, "
            "FailedAnalysis INTEGER, "
            "Reloading INTEGER, "
            "Log TEXT "
            ")" );

    if ( !runCmd( query ) )
        return;

    query = QSqlQuery( "CREATE INDEX IF NOT EXISTS IDX_SVNREVISION ON SVNEntries( Revision )" );
    if ( !runCmd( query ) )
        return;

    query = QSqlQuery( "CREATE INDEX IF NOT EXISTS IDX_SVNAUTHORID ON SVNEntries( AuthorID )" );
    if ( !runCmd( query ) )
        return;

    query = QSqlQuery( "CREATE INDEX IF NOT EXISTS IDX_SVNDATE ON SVNEntries( Date )" );
    if ( !runCmd( query ) )
        return;

    query = QSqlQuery( 
        "CREATE TABLE IF NOT EXISTS BoolType( "
            "ID INTEGER PRIMARY KEY, "
            "Value TEXT NOT NULL "
            ")" );

    if ( !runCmd( query ) )
        return;

    query = QSqlQuery( "CREATE INDEX IF NOT EXISTS IDX_BOOLTYPE ON BoolType( ID )" );
    if ( !runCmd( query ) )
        return;

    query = QSqlQuery( "SELECT * FROM BoolType" );
    if ( !runCmd( query ) )
        return;
    if ( !query.next() )
    {
        runCmd( QSqlQuery( "INSERT INTO BoolType ( ID, Value ) VALUES( 0, 'false' )" ), true );
        runCmd( QSqlQuery( "INSERT INTO BoolType ( ID, Value ) VALUES( 1, 'true' )" ), true );
    }
    runCmd( QSqlQuery( "UPDATE SVNEntries SET Reloading=0" ), true );
    fModel = std::make_shared< CSVNModel >( "http://bpsvn/svn", this );

    QTimer::singleShot( 100, fModel.get(), SLOT( slotComputeLastLoaded() ) );
    QTimer::singleShot( 100, fModel.get(), SLOT( slotComputeTotalEntries() ) );

    return;
}

void CMainWindow::slotModelRunning( bool modelRunning )
{
    fImpl->loadFromSVN->setEnabled( !modelRunning );
    fImpl->stopLoading->setEnabled( modelRunning && !fModel->isStopping() );
    fImpl->refresh->setEnabled( !modelRunning );
    fImpl->deleteRows->setEnabled( !modelRunning );
}

void CMainWindow::slotStatus( const QString & status )
{
    QString msg = tr( "%1 - %2" ).arg( QTime::currentTime().toString() ).arg( status );
    fImpl->log->appendPlainText( msg );
    fImpl->log->moveCursor( QTextCursor::End );
    fImpl->log->ensureCursorVisible();
    fImpl->statusbar->showMessage( msg );
    fImpl->threadsLabel->setText( tr( "Threads Running: %1" ).arg( fModel->getThreadSummary() ) );
}

void CMainWindow::slotDeleteRows()
{
    bool aOK;
    int deleteFrom = QInputDialog::getInt( this, "Earliest Revision to Keep", "Revision Number", fModel->lastLoaded(), 0, fModel->lastLoaded(), 1, &aOK );
    if ( !aOK )
        return;

    QSqlQuery query( "DELETE FROM LinesOfCode WHERE Revision >= ?" );
    if ( !runCmd( query, QList< QVariant >() << deleteFrom ) )
        return;

    query = QSqlQuery( "DELETE FROM SVNEntries WHERE Revision >= ?" );
    if ( !runCmd( query, QList< QVariant >() << deleteFrom ) )
        return;

    fModel->slotRefresh();
}

void CMainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint pt = fImpl->tableView->viewport()->mapFromGlobal( event->globalPos() );
    QModelIndex idx = fImpl->tableView->indexAt( pt );
    if ( idx.isValid() )
    {
        event->ignore();
        QMenu * menu = nullptr;
        if ( !idx.data( CSVNModel::eReloadingRole ).toBool() )
        {
            menu = new QMenu( this );
            auto revision = idx.data( CSVNModel::eRevisionNumRole ).toInt();
            auto action = menu->addAction( QString( "Re-load revision '%1'" ).arg( revision ) );
            action->setData( revision );
            connect( action, SIGNAL( triggered() ), fModel.get(), SLOT( slotReloadRevision() ) );
        }

        if (menu)
        {
            if (!menu->isEmpty())
            {
                menu->setAttribute(Qt::WA_DeleteOnClose);
                menu->popup(event->globalPos());
                event->accept();
            }
            else
            {
                delete menu;
            }
        }
    }
}

void CMainWindow::slotViewGraph()
{
    CChartWindow * chartWindow = new CChartWindow( fModel, this );
    chartWindow->show();
}

void CMainWindow::slotEditCurrentUsers()
{
    CEditUsers dlg;
    if ( dlg.exec() == QDialog::Accepted )
    {
        fModel->updateAuthorInfo( true );
    }
}
