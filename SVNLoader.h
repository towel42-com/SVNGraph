#ifndef SVNLOADER_H
#define SVNLOADER_H

#include <mutex>
#include <QObject>
class QString;
class QByteArray;
class QDateTime;

class CSVNLoader : public QObject
{
Q_OBJECT;
public:
    static QString kSVN;
    static void extractLogInfo( const QByteArray & data, int & revision, QString & user, QDateTime & dateTime, QString & msg, bool & isDirCopy, bool & isMerge );
public:
    CSVNLoader( QObject * parent );

    static void loadSVNData( CSVNLoader *, int revisionNumber );
    static void analyzeSVNData( CSVNLoader *, int revisionNumber, const QString & log, bool isDirCopy, bool isMerge );
    
    static void computeTotalEntries( CSVNLoader *, int & numEntries );
    bool shouldAnalyze( const QString & msg );
signals:
    void sigFinished( int revision, bool aOK );
    void sigFinishedAnalysis( int revision, bool aOK );
    void sigFinishedComputeTotalEntries();
    void sigStatus( const QString & status );
    void sigEntryAdded();
public slots:
    void slotSetWaitTime( int seconds ){ fWaitTime = seconds; }

private:
    void loadSVNDataInternal( int revisionNumber );
    void computeTotalEntriesInternal( int & numEntries );
    void analyzeEntry( int revision, const QString & log, bool isDirCopy, bool isMerge );

    void setRunning( int revision, bool running );

    int getExtensionID( const QString & extension );

    void addEntry( int revision, int userID, const QDateTime & dt, const QString & log );

    int addUser( QString author );
    std::map< QString, std::pair< int, bool > > fAuthorMap;
    std::mutex fMutex;
    bool fAnalyzeOnly{false};
    int fWaitTime{ 90 };
};


#endif // ORDERPROCESSOR_H
