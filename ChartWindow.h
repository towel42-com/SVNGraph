#ifndef CCHARTWINDOW_H
#define CCHARTWINDOW_H

#include <QMainWindow>
#include <memory>
class CSVNModel;
namespace Ui{ class CChartWindow; }
class CChartWindow : public QMainWindow
{
    Q_OBJECT
public:
    CChartWindow( std::shared_ptr< CSVNModel > model, QWidget *parent = 0);
    ~CChartWindow();

public slots:
    void slotInit();
private:
    std::shared_ptr< CSVNModel > fModel;
    std::unique_ptr< Ui::CChartWindow > fImpl;
};

#endif // ORDERPROCESSOR_H
