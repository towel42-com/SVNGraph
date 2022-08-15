#ifndef SVNMODEL_H
#define SVNMODEL_H

#include <QDate>
#include <map>
#include <QSqlRelationalTableModel>
#include <memory>
#include <thread>
#include <set>
#include <tuple>
#include <memory>
#include <QTimer>

class CSVNLoader;
enum EColumnIndex
{
    eID,
    eRevision,
    eAuthor,
    eDate,
    eNumInserted,
    eNumDeleted,
    eNumModified,
    eSkippedAnalysis,
    eFailedAnalysis,
    eReloading,
    eLog
};

class CSVNModel : public QSqlRelationalTableModel
{
Q_OBJECT;
public:
    struct SThreadData
    {
        SThreadData( const QDateTime && dataTime, std::thread * thread, bool singleShot ) :
            fDateTime( dataTime ),
            fThread( thread ),
            fSingleShot( singleShot )
        {
        }
        ~SThreadData()
        {
            delete fThread;
        }
        QDateTime fDateTime;
        std::thread * fThread;
        bool fSingleShot;
    };

    enum ERoles
    {
        eFailedAnalysisRole = Qt::UserRole+1,
        eRevisionNumRole,
        eReloadingRole
    };
    CSVNModel( const QString & url, QObject * parent );
    ~CSVNModel();

    void initDatabase();

    //QString selectStatement() const;
    virtual Qt::ItemFlags flags( const QModelIndex & index ) const;

    int lastLoaded()const{ return fLastLoaded; }
    QDateTime lastLoadedDate() const { return fLastLoadedDate; }
    int numEntries()const{ return fNumEntries; }
    int numFailedEntries() const{ return fNumFailedEntries; }
    QString getSummary() const;
    bool isStopping() const{ return fStopped; }
    bool isRunning() const;
    size_t threadCount() const;
    QString getThreadSummary() const;
    CSVNLoader *getLoader() const{ return fLoader; }
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool currentDevsOnly() const{ return fCurrentDevsOnly; }
    bool failedAnalysisOnly() const{ return fFailedAnalysisOnly; }

    void setFilter();
    void updateAuthorMap();
    void updateAuthorInfo( bool update );
public slots:
    bool select();
    void slotDataChanged();
    void slotComputeLastLoaded();
    void slotComputeTotalEntries();
    void slotLoadFromSVN();
    void slotRefresh();
    //void slotAnalyzeMissing();
    void slotStopLoading();
    void slotRunNext( int revision, bool aOK );
    //void slotAnalyzeNext( int revisionFinished, bool aOK );
    void slotFinishedComputeTotalEntries();
    void slotSetMaxNumThreads( int value ){ fMaxNumThreads = value; }
    void slotUpdateUI();
    void slotReloadRevision();
    void slotCurrentDevsOnly();
    void slotFailedAnalysisOnly();

signals:
    void sigUpdateValues();
    void sigRunning( bool running );
    void sigStatus( const QString & );
private:
    void computeValues( bool aOK );



    QString fURL;
    int fLastLoaded{ 0 };
    QDateTime fLastLoadedDate;
    int fNumEntries{ 0 };
    int fNumFailedEntries{ 0 };
    bool fStopped{ false };

    std::multimap< int, std::tuple< QString, bool, QDateTime, QDateTime > > fAuthorInfo;
    std::map< QString, std::pair< int, bool > > fAuthorMap;

    std::map< int, std::shared_ptr< SThreadData > > fThreads;
    std::thread fComputeFlag;
    CSVNLoader * fLoader;
    int fMaxNumThreads{ 32 };
    std::map< int, QString > fRevisionsWithMissingAnalysis;
    QTimer fTimer;
    bool fCurrentDevsOnly{ true };
    bool fFailedAnalysisOnly{ false };
};


#endif 
