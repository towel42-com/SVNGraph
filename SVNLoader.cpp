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
#include <QXmlQuery>
#include <QDebug>
#include <QTimeZone>
#include <QTimer>
#include <QLocale>

QString CSVNLoader::kSVN{ "C:/Program Files (x86)/VisualSVN/bin/svn.exe" };

CSVNLoader::CSVNLoader( QObject * parent ) :
    QObject( parent )
{
}

//<?xml version="1.0" encoding="UTF-8"?>
//<log>
//<logentry
//   revision="44133">
//<author>build</author>
//<date>2017-12-26T17:24:05.915867Z</date>
//<msg>Create branch and rev the version for the build</msg>
//</logentry>
//</log>


//<?xml version="1.0" encoding="UTF-8"?>
//<log>
//<logentry
//   revision="2">
//<author>prab</author>
//<date>1998-03-26T14:52:08.000000Z</date>
//<paths>
//<path
//   prop-mods="false"
//   text-mods="false"
//   kind="dir"
//   action="A">/trunk/Veritable</path>
//<path
//   prop-mods="false"
//   text-mods="false"
//   kind="dir"
//   action="A">/trunk/Veritable/tcl8.4.4</path>
//<path
//   prop-mods="false"
//   text-mods="false"
//   kind="dir"
//   action="A">/trunk/Veritable/tcl8.4.4/library</path>
//<path
//   prop-mods="false"
//   text-mods="false"
//   kind="dir"
//   action="A">/trunk/Veritable/tcl8.4.4/library/http1.0</path>
//<path
//   prop-mods="true"
//   text-mods="true"
//   kind="file"
//   action="A">/trunk/Veritable/tcl8.4.4/library/http1.0/pkgIndex.tcl</path>
//</paths>
//<msg>Release 2.0.9
//</msg>
//</logentry>
//</log>

//<?xml version="1.0" encoding="UTF-8"?>
//<log>
//    <logentry
//       revision="44089">
//        <author>scott</author>
//        <date>2017-12-21T22:41:16.472133Z</date>
//        <msg>Work around for svn</msg>
//        <logentry
//           reverse-merge="false"
//           revision="44073">
//            <author>scott</author>
//            <date>2017-12-21T03:55:05.337215Z</date>
//            <msg>When reporting cyclic signals give more info</msg>
//        </logentry>
//    </logentry>
//</log>


// example of dir copy
//<?xml version="1.0" encoding="UTF-8"?>
//<log>
//<logentry
//   revision="44448">
//<author>simon</author>
//<date>2018-01-13T02:53:21.722086Z</date>
//<paths>
//<path
//   prop-mods="true"
//   text-mods="false"
//   kind="dir"
//   copyfrom-path="/trunk"
//   copyfrom-rev="44447"
//   action="A">/branches/Simon/FPGALibs20180112</path>
//</paths>
//<msg>Branch to work on FPGA libraries
//</msg>
//</logentry>
//</log>

void CSVNLoader::extractLogInfo( const QByteArray & data, int & revision, QString & user, QDateTime & dateTime, QString & msg, bool & isDirCopy, bool & isMerge )
{
    isDirCopy = isMerge = false;

    QXmlQuery query;
    if ( !query.setFocus( data ) )
        return;

    query.setQuery( "/log/logentry/author[1]/string()" );
    Q_ASSERT( query.isValid() );
    query.evaluateTo( &user );
    user = user.trimmed();
    if( user.isEmpty() )
        user = "<system>";

    if ( user.indexOf( " " ) != -1 )
        int xyz = 0;
    query.setQuery( "(/log/logentry/@revision)[1]/string()" );
    Q_ASSERT( query.isValid() );
    QString tmp;
    query.evaluateTo( &tmp );
    tmp = tmp.trimmed();
    revision = tmp.toInt();

    query.setQuery( "/log/logentry/date[1]/string()" );
    Q_ASSERT( query.isValid() );
    query.evaluateTo( &tmp );
    tmp = tmp.trimmed();
    dateTime = QDateTime::fromString( tmp, Qt::ISODate );
    Q_ASSERT( dateTime.isValid() );

    query.setQuery( "/log/logentry/msg/string()" );
    Q_ASSERT( query.isValid() );
    query.evaluateTo( &msg );
    msg = msg.trimmed();

    query.setQuery( "/log/logentry/paths/path/@action/string()" );
    Q_ASSERT( query.isValid() );
    QStringList actions;
    query.evaluateTo( &actions );
                     
    query.setQuery( "/log/logentry/paths/path/@kind/string()" );
    Q_ASSERT( query.isValid() );
    QStringList kinds;
    query.evaluateTo( &kinds );

    query.setQuery( "/log/logentry/paths/path/string()" );
    Q_ASSERT( query.isValid() );
    QStringList paths;
    query.evaluateTo( &paths );

    bool hasNonDirCopy = false;
    std::map< QString, std::pair< bool, bool > > dirsCopied;
    if ( actions.size() || kinds.size() || paths.size() )
    {
        Q_ASSERT( actions.size() == kinds.size() );
        Q_ASSERT( actions.size() == paths.size() );

        for( int ii = 0; ii < actions.size(); ++ii )
        {
            actions[ ii ] = actions[ ii ].trimmed();
            kinds[ ii ] = kinds[ ii ].trimmed();
            paths[ ii ] = paths[ ii ].trimmed();
            if ( ( actions[ ii ] == "A" || actions[ ii ] == "D" ) && ( kinds[ ii ] == "dir" ) )
            {
                dirsCopied[ paths[ ii ] ].first = dirsCopied[ paths[ ii ] ].first || actions[ ii ] == "A";
                dirsCopied[ paths[ ii ] ].second = dirsCopied[ paths[ ii ] ].second || actions[ ii ] == "D";
            }
            else
                hasNonDirCopy = true;
        }
    }
    if ( !hasNonDirCopy )
    {
        for( auto && ii : dirsCopied )
        {
            if ( !ii.second.first && !ii.second.second )
            {
                hasNonDirCopy = true;
                break;
            }
        }
        if ( !hasNonDirCopy )
            isDirCopy = true;
    }

    query.setQuery( "/log/logentry/logentry/@revision/string()" );
    Q_ASSERT( query.isValid() );
    QStringList merges;
    query.evaluateTo( &merges );

    isMerge = !merges.isEmpty();
}


int CSVNLoader::addUser( QString author )
{
    author = author.toLower();
    auto pos = fAuthorMap.find( author );
    if ( pos == fAuthorMap.end() )
    {
        std::lock_guard< std::mutex > lock( fMutex );
        QSqlQuery query( "SELECT AuthorID, Current FROM Authors WHERE AuthorName=?" );
        if ( !runCmd( query, QList< QVariant >() << author ) )
            return -1;
        if ( query.next() )
        {
            int id = query.value( 0 ).toInt();
            bool current = query.value( 1 ).toInt() ? true : false;
            fAuthorMap[ author ] = std::make_pair( id, current );
            return id;
        }
    
        query = QSqlQuery( "INSERT INTO Authors ( AuthorName, Current ) VALUES ( ?, ? )" );
        if ( !runCmd( query, QList< QVariant >() << author << 1 ) )
            return -1;

        int id = query.lastInsertId().toInt();
        fAuthorMap[ author ] = std::make_pair( id, true );
        return id;
    }
    else
        return (*pos).second.first;
}

void CSVNLoader::analyzeSVNData( CSVNLoader * loader, int revision, const QString & log, bool isDirCopy, bool isMerge )
{
    loader->fAnalyzeOnly = true;
    loader->analyzeEntry( revision, log, isDirCopy, isMerge );
}

int CSVNLoader::getExtensionID( const QString & extension )
{
    std::lock_guard< std::mutex > lock( fMutex );
    QSqlQuery query;
    if ( !runCmd( query, "SELECT ID FROM ExtensionDefs WHERE Name=?", QList< QVariant >() << extension ) )
    {
        return -1;
    }

    if ( query.next() )
    {
        return query.value( 0 ).toInt();
    }
    if ( !runCmd( query, "INSERT INTO ExtensionDefs ( Name ) VALUES (?)", QList< QVariant >() << extension ) )
    {
        return -1;
    }

    return query.lastInsertId().toInt();
}

void CSVNLoader::analyzeEntry( int revision, const QString & log, bool isDirCopy, bool isMerge )
{
    emit sigStatus( tr( "Analyzing %1" ).arg( revision ) );

    setRunning( revision, true );
    if ( isMerge || isDirCopy || !shouldAnalyze( log ) )
    {
        std::lock_guard< std::mutex > lock( fMutex );
        QSqlQuery query;
        if( !runCmd( query, "UPDATE SVNEntries SET NumInserted=0, NumDeleted=0, NumModified=0, SkippedAnalysis=1, FailedAnalysis=0, Reloading=0 WHERE Revision=?", QList< QVariant >() << revision ) )
        {
            emit sigStatus( tr( "Analyzing %1 - SKIPPED" ).arg( revision ) );
            if ( fAnalyzeOnly )
                emit sigFinishedAnalysis( revision, false );
            return;
        }
        return;
    }


    QProcess process;
    QStringList args = 
        QStringList() << "diff" << "-c" << QString::number( revision ) << "http://bpsvn/svn";
    process.start( kSVN, args );
    emit sigStatus( tr( "svn diff %1" ).arg( revision ) );
    bool aOK = process.waitForStarted();
    aOK = aOK && process.waitForFinished( fWaitTime == -1 ? -1 : fWaitTime * 1000 );
    auto result = aOK ? process.readAll() : QByteArray();
    emit sigStatus( tr( "svn diff %1 - Finished" ).arg( revision ) );


    std::map< std::string, std::map< CDiffStat::EModType, int > > diffMap;
    if ( !result.isEmpty() )
    {
        emit sigStatus( tr( "Computing line changes %1" ).arg( revision ) );
        CDiffStat df;
        std::istringstream iss( result.toStdString() );
        df.setStream( &iss );
        df.setMerge( true );
        df.setExclude( "\\/testcases.*" );

        if( !df.parse() )
        {
            emit sigStatus( tr( "Analyzing (Computing line changes) %1 - FAILED" ).arg( revision ) );
            if( fAnalyzeOnly )
                emit sigFinishedAnalysis( revision, false );
            setRunning( revision, false );
            return;
        }

        diffMap = df.getResults();
        emit sigStatus( tr( "Analyzing (Computing line changes) %1 - SUCCESSFUL" ).arg( revision ) );
    }
    else
        emit sigStatus( tr( "Analyzing (svn diff) %1 - Empty" ).arg( revision ) );


    {
        std::lock_guard< std::mutex > lock( fMutex );
        QSqlQuery query;
        if( !runCmd( query, "DELETE FROM LinesOfCode WHERE Revision=?", QList< QVariant >() << revision ) )
        {
            emit sigStatus( tr( "Analyzing %1 - FAILED" ).arg( revision ) );
            if( fAnalyzeOnly )
                emit sigFinishedAnalysis( revision, false );
            return;
        }
    }

    int totInserted = 0;
    int totDeleted = 0;
    int totModified = 0;
    for( auto ii : diffMap )
    {
        int extensionID = getExtensionID( QString::fromStdString( ii.first ) );
        const auto & values = ii.second;
        int numInserted = (*values.find( CDiffStat::EModType::eAdd )).second;
        int numModified = (*values.find( CDiffStat::EModType::eModified )).second;
        int numDeleted = (*values.find( CDiffStat::EModType::eDelete )).second;

        totInserted += numInserted;
        totDeleted += numDeleted;
        totModified += numModified;

        std::lock_guard< std::mutex > lock( fMutex );
        QSqlQuery query;
        if( !runCmd( query,
            "INSERT INTO LinesOfCode ( Revision, ExtensionID, NumInserted, NumDeleted, NumModified ) VALUES ( ?, ?, ?, ?, ? )",
            QList< QVariant >() << revision << extensionID << numInserted << numDeleted << numModified ) )
        {
            emit sigStatus( tr( "Analyzing %1 - FAILED" ).arg( revision ) );
            if( fAnalyzeOnly )
                emit sigFinishedAnalysis( revision, false );
            return;
        }
    }

    {
        std::lock_guard< std::mutex > lock( fMutex );
        QSqlQuery query;
        if( !runCmd( query,
            "UPDATE SVNEntries SET NumInserted=?, NumDeleted=?, NumModified=?, SkippedAnalysis=0, FailedAnalysis=? WHERE Revision=?",
            QList< QVariant >() << totInserted << totDeleted << totModified << ( aOK ? 0 : 1 ) << revision ) )
        {
            emit sigStatus( tr( "Analyzing %1 - FAILED" ).arg( revision ) );
            if( fAnalyzeOnly )
                emit sigFinishedAnalysis( revision, false );
            return;
        }
    }
    emit sigStatus( tr( "Analyzing %1 - %2" ).arg( revision ).arg( aOK ? "SUCCESSFUL" : "FAILED" ) );
    if( fAnalyzeOnly )
        emit sigFinishedAnalysis( revision, true );
    setRunning( revision, false );
    return;
}

void CSVNLoader::loadSVNData( CSVNLoader * loader, int revision )
{
    loader->loadSVNDataInternal( revision );
}

bool CSVNLoader::shouldAnalyze( const QString & log )
{
    return !log.startsWith( "This commit was manufactured by cvs2svn" ) && !log.startsWith( "Create branch and rev the version for the build" );
}

void CSVNLoader::loadSVNDataInternal( int revision )
{
    QDateTime startTime = QDateTime::currentDateTime();
    QProcess process;
    QStringList args = 
        QStringList() << "log" << "--xml" << "--verbose" << "--use-merge-history" << "-r" << QString::number( revision + 1 ) << "http://bpsvn/svn";
    process.start( kSVN, args );
    emit sigStatus( tr( "Loading %1" ).arg( revision + 1 ) );
    if( !process.waitForStarted() )
    {
        emit sigStatus( tr( "Loading %1 - FAILED" ).arg( revision + 1 ) );
        emit sigFinished( revision, false );
        return;
    }

    if ( !process.waitForFinished( fWaitTime == -1 ? -1 : fWaitTime * 1000 ) )
    {
        emit sigStatus( tr( "Loading %1 - FAILED" ).arg( revision + 1 ) );
        emit sigFinished( revision, false );
        return;
    }

    auto result = process.readAll();

    int newRevision=0;
    QString user;
    QDateTime dt;
    QString log;
    bool isDirCopy;
    bool isMerge;
    extractLogInfo( result, newRevision, user, dt, log, isDirCopy, isMerge );
    int userID = addUser( user );

    addEntry( newRevision, userID, dt, log );

    analyzeEntry( newRevision, log, isDirCopy, isMerge );

    emit sigStatus( tr( "Loading %1 - SUCCESSFUL" ).arg( revision + 1 ) );
    emit sigFinished( revision, true );
}

void CSVNLoader::addEntry( int revision, int userID, const QDateTime & dt, const  QString & log )
{
    std::lock_guard< std::mutex > lock( fMutex );
    QSqlQuery query( "SELECT * FROM SVNEntries WHERE Revision=?" );
    if( !runCmd( query, QList< QVariant >() << revision ) )
    {
        emit sigFinished( revision, false );
        return;
    }
    if( !query.next() )
    {
        QSqlQuery query( "INSERT INTO SVNEntries ( Revision, AuthorID, Date, Log, SkippedAnalysis, FailedAnalysis ) VALUES ( ?, ?, ?, ?, ?, ? )" );
        if( !runCmd( query, QList< QVariant >() << revision << userID << dt << log<< 0 << 0) )
        {
            emit sigFinished( revision, false );
            return;
        }
        emit sigEntryAdded();
    }
    else
    {
        QSqlQuery query( "UPDATE SVNEntries SET AuthorID=?, Date=?, Log=? WHERE Revision=?" );
        if( !runCmd( query, QList< QVariant >() << userID << dt << log << revision ) )
        {
            emit sigFinished( revision, false );
            return;
        }
    }
}

void CSVNLoader::computeTotalEntries( CSVNLoader * loader, int & numEntries )
{
    loader->computeTotalEntriesInternal( numEntries );
}

void CSVNLoader::computeTotalEntriesInternal( int & numEntries )
{
    numEntries = 0;
    QProcess process;
    QStringList args =
        QStringList() << "log" << "--xml" << "-r" << "HEAD" << "http://bpsvn/svn";
    process.start( CSVNLoader::kSVN, args );
    if( !process.waitForStarted() )
    {
        return;
    }

    if( !process.waitForFinished( fWaitTime == -1 ? -1 : fWaitTime * 1000 ) )
    {
        return;
    }

    auto result = process.readAll();

    QString user;
    QDateTime dt;
    QString msg;
    bool isDirCopy;
    bool isMerge;
    CSVNLoader::extractLogInfo( result, numEntries, user, dt, msg, isDirCopy, isMerge );
    emit sigFinishedComputeTotalEntries();
}

void CSVNLoader::setRunning( int revision, bool running )
{
    std::lock_guard< std::mutex > lock( fMutex );
    QSqlQuery query;
    runCmd( query, "UPDATE SVNEntries SET Reloading=? WHERE Revision=?", QList< QVariant >() << ( running ? 1 : 0 ) << revision );
}
