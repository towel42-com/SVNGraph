#include "ChartWindow.h"
#include "ui_ChartWindow.h"  
#include "AutoWaitCursor.h"

#include "SVNModel.h"

#include <QTimer> 
#include <QLineSeries>
#include <map>
#include <QDebug>

CChartWindow::CChartWindow( std::shared_ptr< CSVNModel > model, QWidget *parent)
    : QMainWindow(parent),
    fImpl( new Ui::CChartWindow ),
    fModel( model )
{
    fImpl->setupUi(this);
    setWindowIcon( QIcon( ":/resources/svn.png" ) );
    QTimer::singleShot( 0, this, SLOT( slotInit() ) );
    setAttribute( Qt::WA_DeleteOnClose, true );
    fImpl->chartView->setRenderHint( QPainter::Antialiasing );
}

CChartWindow::~CChartWindow()
{
}

void CChartWindow::slotInit()
{
    NUIUtils::CAutoWaitCursor awc;
    qDebug() << "About to Fetch";
    while( fModel->canFetchMore() )
        fModel->fetchMore();
    qDebug() << "Fully Fetched()";

    int rowCount = fModel->rowCount();
    std::map< int, std::pair< QString, int > > revisions; // revision -> User -> total
    std::map< QString, QLineSeries * > authors;

    for( int ii = rowCount -1 ; ii >= 0; ii-- )
    {
        int revision = fModel->index( ii, eRevision ).data().toInt();
        QString author = fModel->index( ii, eAuthor ).data().toString();
        auto pos = authors.find( author );
        if ( pos == authors.end() )
        {
            auto ls = new QLineSeries;
            ls->setName( author );
            authors[ author ] = ls;
        }
        int numIns = fModel->index( ii, eNumInserted ).data().toInt();
        int numDel = fModel->index( ii, eNumDeleted ).data().toInt();
        int numMod = fModel->index( ii, eNumModified ).data().toInt();
        int tot = numIns + numDel + numMod;

        revisions[ revision ] = std::make_pair( author, tot );
    }
    qDebug() << "Fully Analyzed";

    std::map< QString, int > totSoFar;
    for( auto && currRev : revisions )
    {
        auto rev = currRev.first;
        auto currAuthor = currRev.second.first;
        auto currTot = currRev.second.second;
        totSoFar[ currAuthor ] += currTot;
        for( auto && author : authors )
        {
            auto && authorName = author.first;
            auto && ls = author.second;

            ls->append( rev, totSoFar[ authorName ] );
        }
    }
    qDebug() << "Fully Series Added";

    QChart * chart = new QChart();
    fImpl->chartView->setChart( chart );

    for( auto && author : authors )
    {    
        chart->addSeries( author.second );
    }
    qDebug() << "All Series Added to Chart";
    chart->createDefaultAxes();
    qDebug() << "Create Default Axes";
    chart->legend()->setAlignment( Qt::AlignRight );
    chart->legend()->show();
}
