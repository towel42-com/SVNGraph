#include "SVNModel.h"
#include "SVNLoader.h"
#include "AutoWaitCursor.h"
#include "runCmd.h"
#include "DiffStat.h"


#include <sstream>
#include <string>
#include <iostream>

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>
#include <QTimeZone>
#include <QTimer>
#include <QLocale>
#include <QAction>
#include <thread>

CSVNModel::CSVNModel( const QString & url, QObject * parent ) :
    QSqlRelationalTableModel( parent ),
    fURL( url )
{
    initDatabase();
    connect( this, SIGNAL( dataChanged( const QModelIndex &, const QModelIndex & ) ), this, SLOT( slotDataChanged() ) );
    fLoader = new CSVNLoader( this );
    connect( fLoader, SIGNAL( sigFinished( int, bool ) ), this, SLOT( slotRunNext( int, bool) ) );
    
    connect( fLoader, SIGNAL( sigFinishedComputeTotalEntries() ), this, SLOT( slotFinishedComputeTotalEntries() ) );
    connect( fLoader, SIGNAL( sigStatus( const QString & ) ), this, SIGNAL( sigStatus( const QString & ) ) );
    connect( fLoader, SIGNAL( sigEntryAdded() ), this, SLOT( slotRefresh() ) );

    QTimer::singleShot( 100, this, SLOT( slotDataChanged() ) );
    fTimer.setInterval( 1000 );
    fTimer.setSingleShot( false );
    connect( &fTimer, SIGNAL( timeout() ), this, SLOT( slotUpdateUI() ) );
}

CSVNModel::~CSVNModel()
{
    for( auto & ii : fThreads )
    {
        if ( ii.second->fThread->joinable() )
            ii.second->fThread->join();
    }
    if ( fComputeFlag.joinable() )
        fComputeFlag.join();
}

void CSVNModel::initDatabase()
{
    setTable( "SVNEntries" );
    setRelation( EColumnIndex::eAuthor, QSqlRelation( "Authors", "AuthorID", "AuthorName" ) );
    setRelation( EColumnIndex::eSkippedAnalysis, QSqlRelation( "BoolType", "ID", "Value" ) );
    setRelation( EColumnIndex::eFailedAnalysis, QSqlRelation( "BoolType", "ID", "Value" ) );
    setRelation( EColumnIndex::eReloading, QSqlRelation( "BoolType", "ID", "Value" ) );

    setHeaderData( EColumnIndex::eID, Qt::Horizontal, tr( "ID" ) );
    setHeaderData( EColumnIndex::eRevision, Qt::Horizontal, tr( "Revision" ) );
    setHeaderData( EColumnIndex::eAuthor, Qt::Horizontal, tr( "Author" ) );
    setHeaderData( EColumnIndex::eDate, Qt::Horizontal, tr( "Date" ) );
    setHeaderData( EColumnIndex::eNumInserted, Qt::Horizontal, tr( "Inserted" ) );
    setHeaderData( EColumnIndex::eNumDeleted, Qt::Horizontal, tr( "Deleted" ) );
    setHeaderData( EColumnIndex::eNumModified, Qt::Horizontal, tr( "Modified" ) );
    setHeaderData( EColumnIndex::eSkippedAnalysis, Qt::Horizontal, tr( "Skipped Analysis" ) );
    setHeaderData( EColumnIndex::eFailedAnalysis, Qt::Horizontal, tr( "Failed Analysis" ) );
    setHeaderData( EColumnIndex::eReloading, Qt::Horizontal, tr( "Running" ) );
    setHeaderData( EColumnIndex::eLog, Qt::Horizontal, tr( "Log" ) );

    setSort( eRevision, Qt::DescendingOrder );
}

void CSVNModel::slotRefresh()
{
    select();
    slotDataChanged();

    QTimer::singleShot( 100, this, SLOT( slotComputeLastLoaded() ) );
    QTimer::singleShot( 100, this, SLOT( slotComputeTotalEntries() ) );
}

void CSVNModel::slotDataChanged()
{
    computeValues( true );
}

void CSVNModel::computeValues( bool aOK )
{
    if ( !aOK )
    {
        emit sigUpdateValues();
        return;
    }
    
    updateAuthorInfo( false );

    QSqlQuery query = QSqlQuery( "select Date FROM SVNEntries ORDER BY Date Desc LIMIT 1" );
    if ( runCmd( query ) && query.next() )
    {
        fLastLoadedDate = query.value( 0 ).toDateTime();
    }

    fNumFailedEntries = 0;
    query = QSqlQuery( "SELECT COUNT(*) FROM SVNEntries WHERE FailedAnalysis=1" );
    if ( runCmd( query ) && query.next() )
    {
        fNumFailedEntries = query.value( 0 ).toInt();
    }
    setFilter();
    emit sigUpdateValues();
}

void CSVNModel::updateAuthorInfo( bool update )
{
    updateAuthorMap();
    fAuthorInfo.clear();
    for( auto ii : fAuthorMap )
    {
        QSqlQuery query = QSqlQuery( "select sum( NumInserted + NumDeleted + NumModified ), MIN( date ), max( date ) from SVNEntries where AuthorID=?" );
        if( runCmd( query, QList< QVariant >() << ii.second.first ) )
        {
            if( query.next() )
            {
                const auto & author = ii.first;
                auto lines = query.value( 0 ).toInt();
                auto startDate = query.value( 1 ).toDateTime();
                auto endDate = query.value( 2 ).toDateTime();
                fAuthorInfo.insert( std::make_pair( lines, std::make_tuple( author, ii.second.second, startDate, endDate ) ) );
            }
        }
    }
    if ( update )
    {
        setFilter();
        emit sigUpdateValues();
    }
}

void CSVNModel::updateAuthorMap()
{
    QSqlQuery query( "SELECT AuthorName, AuthorID, Current from Authors" );
    if( !runCmd( query ) )
    {
        emit sigUpdateValues();
        return;
    }
    while( query.next() )
    {
        auto author = query.value( 0 ).toString();
        fAuthorMap[ author ] = std::make_pair( query.value( 1 ).toInt(), (query.value( 2 ).toInt() ? true : false) );
    }
}

bool CSVNModel::select()
{
    bool retVal = QSqlRelationalTableModel::select();
    if ( !retVal )
    {
        qDebug() << lastError();
    }
    return retVal;
}

Qt::ItemFlags CSVNModel::flags( const QModelIndex & index ) const
{
    Qt::ItemFlags retVal = QSqlRelationalTableModel::flags( index );
    retVal = retVal & ~Qt::ItemIsEditable;
    return retVal;
}

QString CSVNModel::getSummary() const
{
    QString retVal = "<ul>\n";
    QLocale locale;
    for( auto ii = fAuthorInfo.rbegin(); ii != fAuthorInfo.rend(); ++ii )
    {
        int lines = (*ii).first;
        auto author = std::get< 0 >( (*ii).second );
        auto current = std::get< 1 >( (*ii).second );

        if ( fCurrentDevsOnly && !current )
            continue;

        auto startDate = std::get< 2 >( (*ii).second );
        auto endDate = std::get< 3 >( (*ii).second );
        if ( ( author == "build" ) || ( author == "<system>" ) )
            continue;

        //qDebug() << startDate.toString();
        //qDebug() << endDate.toString();
        int numDays = startDate.daysTo( endDate ) + 1;
        auto codePerDay = 1.0*lines/numDays;

        retVal += 
            tr( "   <li>%7%1%8( Lines of Code: %2, Dates Active %3-%4: Number of Days: %5 Code/Day: %6 )</li>\n" )
            .arg( author.toHtmlEscaped() )
            .arg( locale.toString( lines ) )
            .arg( locale.toString( startDate.date() ) )
            .arg( locale.toString( endDate.date() ) )
            .arg( locale.toString( numDays ) )
            .arg( codePerDay )
            .arg( current ? "<b>" : "" )
            .arg( current ? "</b>" : "" )
            ;
    }
    retVal += "</ul>";
    return retVal;

}

QString CSVNModel::getThreadSummary() const
{
    qint64 totalMsecs = 0;
    QString threadInfo;
    for( auto & ii : fThreads )
    {
        int revision = ii.first;
        auto threadID = ii.second->fThread->get_id();
        auto diffTime = ii.second->fDateTime.msecsTo( QDateTime::currentDateTime() );
        totalMsecs += diffTime;
        std::ostringstream oss;
        oss << threadID;
        bool joinable = ii.second->fThread->joinable();
        bool singleShot = ii.second->fSingleShot;
        threadInfo += tr( "   <li>Revision: %1 Runtime: %2ms ( %3, %4 )</li>\n" )
            .arg( singleShot ? ( revision + 1 ) : revision )
            .arg( diffTime )
            .arg( QString::fromStdString( oss.str() ) )
            .arg( joinable ? "Active" : "Waiting to Finish" )
            ;
    }

    int numThreads = threadCount();
    double avgRunTime = numThreads ? std::round( 1.0*totalMsecs/numThreads ) : 0.0;
    QString retVal = QString( "%1 Avg Runtime: %2ms" ).arg( numThreads ).arg( avgRunTime, 0, 'f', 0 );
    retVal += "<ol>\n" + threadInfo + "</ol>";
    return retVal;

}

QVariant CSVNModel::data( const QModelIndex &idx, int role /*= Qt::DisplayRole*/ ) const
{
    if( role == eFailedAnalysisRole )
    {
        return QSqlRelationalTableModel::data( this->index( idx.row(), eFailedAnalysis ), Qt::DisplayRole ).toBool();
    }
    else if( role == eReloadingRole)
    {
        return QSqlRelationalTableModel::data( this->index( idx.row(), eReloading ), Qt::DisplayRole ).toBool();
    }
    else if( role == eRevisionNumRole )
    {
        return QSqlRelationalTableModel::data( this->index( idx.row(), eRevision ), Qt::DisplayRole ).toInt();
    }
    return QSqlRelationalTableModel::data( idx, role );
}

bool CSVNModel::isRunning() const
{
    return !isStopping() && !fThreads.empty();
}

size_t CSVNModel::threadCount() const
{
    size_t retVal =0;
    for( auto & ii : fThreads )
    {
        if ( ii.second->fThread->joinable() )
            retVal++;
    }

    if ( fComputeFlag.joinable() )
        retVal++;
    return retVal;
}

void CSVNModel::slotComputeLastLoaded()
{
    fLastLoaded = 0;
    QSqlQuery query = QSqlQuery( "SELECT MAX( Revision ) FROM SVNEntries" );
    if( !runCmd( query ) )
        return;
    if( !query.next() )
        return;
    fLastLoaded = query.value( 0 ).toInt();

    emit sigUpdateValues();
}

void CSVNModel::slotStopLoading()
{
    fStopped = true;
    emit sigStatus( "Stopping" );
}

void CSVNModel::slotUpdateUI()
{
    if ( fThreads.empty() && fTimer.isActive() )
        fTimer.stop();
    if ( !fThreads.empty() && !fTimer.isActive() )
        fTimer.start();
        
    computeValues( true );
    //select();
    emit sigUpdateValues();
    emit sigRunning( fThreads.size() > 0  );
}

void CSVNModel::slotReloadRevision()
{
    if ( isStopping() )
        return;

    QAction * action = dynamic_cast< QAction * >( sender() );
    if ( !action )
        return;

    auto revision = action->data().toInt();

    emit sigRunning( true );
    if ( isStopping() )
    {
        slotUpdateUI();
        return;
    }

    fThreads[ revision ] = std::make_shared< SThreadData >( QDateTime::currentDateTime(), new std::thread( &CSVNLoader::loadSVNData, fLoader, revision ), true );
    if ( !fTimer.isActive() )
        fTimer.start();
}

void CSVNModel::setFilter()
{
    QStringList filters;
    if( fCurrentDevsOnly )
    {
        std::list< int > currentAuthors;
        for( auto && ii : fAuthorMap )
        {
            if( ii.second.second )
            {
                currentAuthors.push_back( ii.second.first );
            }
        }
        QString filter;
        if( !currentAuthors.empty() )
        {
            filter = "SVNEntries.AuthorID in ( ";
            bool first = true;
            for( auto ii : currentAuthors )
            {
                if( !first )
                    filter += ",";
                filter += QString::number( ii );
                filter += " ";
                first = false;
            }
            filter += " )";
        }
        filters << "( " + filter + " )";
    }

    if ( fFailedAnalysisOnly )
    {
        filters << "( SVNEntries.FailedAnalysis <> 0 )";
    }
    QString filter = filters.join( " AND " );
        
    QSqlRelationalTableModel::setFilter( filter );
}

void CSVNModel::slotCurrentDevsOnly()
{
    fCurrentDevsOnly = !fCurrentDevsOnly;
    auto action = dynamic_cast<QAction *>( sender() );
    if( !action )
        return;
    action->setChecked( fCurrentDevsOnly );

    setFilter();

    emit sigUpdateValues();
}

void CSVNModel::slotFailedAnalysisOnly()
{
    fFailedAnalysisOnly = !fFailedAnalysisOnly;
    auto action = dynamic_cast<QAction *>( sender() );
    if( !action )
        return;
    action->setChecked( fFailedAnalysisOnly );

    setFilter();

    emit sigUpdateValues();
}

void CSVNModel::slotRunNext( int revisionFinished, bool aOK )
{
    auto ii = fThreads.find( revisionFinished );
    if( revisionFinished != -1 && ( ii == fThreads.end() ) )
    {
        fTimer.stop();
        slotUpdateUI();
        return;
    }

    bool singleShot = false;
    if ( ii != fThreads.end() )
    {
        auto && threadInfo = ( *ii ).second; // wait for the thread to end
        singleShot = threadInfo->fSingleShot;
        if( threadInfo->fThread->joinable() )
            threadInfo->fThread->join();

        if ( threadInfo->fThread->joinable() )
            int xyz = 0;

        fThreads.erase( ii );
    }

    emit sigRunning( true );
    if ( fStopped )
    {
        fTimer.stop();
        slotUpdateUI();
        return;
    }

    if ( singleShot )
    {
        fTimer.stop();
        slotUpdateUI();
        return;
    }

    if ( !aOK )
    {
        fTimer.stop();
        slotUpdateUI();
        return;
    }
    
    while( ( static_cast< int >( fThreads.size() ) < fMaxNumThreads ) && ( fLastLoaded < fNumEntries ) )
    {
        if ( fThreads.find( fLastLoaded ) == fThreads.end() )
            fThreads[ fLastLoaded ] = std::make_shared< SThreadData >( QDateTime::currentDateTime(), new std::thread( &CSVNLoader::loadSVNData, fLoader, fLastLoaded ), false );
        fLastLoaded++;
        if ( !fTimer.isActive() )
            fTimer.start();
    }
}

void CSVNModel::slotLoadFromSVN()
{
    fStopped = false;
    slotRunNext( -1, true );
}

void CSVNModel::slotFinishedComputeTotalEntries()
{
    if ( fComputeFlag.joinable() )
        fComputeFlag.join();

    emit sigUpdateValues();
    emit sigRunning( false );
}

void CSVNModel::slotComputeTotalEntries()
{
    if ( fNumEntries != 0 )
        return;
    if ( fComputeFlag.joinable() )
        fComputeFlag.join();

    emit sigRunning( true );
    fComputeFlag = std::thread( &CSVNLoader::computeTotalEntries, fLoader, std::ref( fNumEntries ) );
    if ( !fTimer.isActive() )
        fTimer.start();
}

