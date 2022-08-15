#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <memory>
class QSplashScreen;
class CSVNModel;
namespace Ui{ class CMainWindow; }
class CMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

    void contextMenuEvent(QContextMenuEvent *event);

public slots:
    void slotValuesChanged();
    void slotModelRunning( bool modelRunning );
    void slotStatus( const QString & status );
    void slotDeleteRows();
    void slotViewGraph();
    void slotEditCurrentUsers();
private:
    void initDatabase();
    std::shared_ptr< CSVNModel > fModel;
    std::unique_ptr< Ui::CMainWindow > fImpl;
};

#endif // ORDERPROCESSOR_H
