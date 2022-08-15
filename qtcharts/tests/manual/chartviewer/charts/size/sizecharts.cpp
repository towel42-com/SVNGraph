/******************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Charts module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include "charts.h"
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

class SizeChart: public Chart
{
public:
    QString name() { return QObject::tr("PlotArea"); }
    QString category()  { return QObject::tr("Size"); }
    QString subCategory() { return QString::null; }

    QChart *createChart(const DataTable &table)
    {

        QChart *chart = new QChart();
        QString name("Series");
        int nameIndex = 0;
        foreach (DataList list, table) {
            QLineSeries *series = new QLineSeries(chart);
            foreach (Data data, list)
                series->append(data.first);
            series->setName(name + QString::number(nameIndex));
            nameIndex++;
            chart->addSeries(series);
        }
       chart->setContentsMargins(0,0,0,0);
       chart->setMargins(QMargins(0,0,0,0));
       return chart;
    }
};

class SizeChart2: public SizeChart
{
public:
    QString name() { return QObject::tr("PA + T"); }

    QChart *createChart(const DataTable &table)
    {
        QChart *chart = SizeChart::createChart(table);
        chart->setTitle(name());
        return chart;
    }
};

class SizeChart3: public SizeChart2
{
public:
    QString name() { return QObject::tr("PA+T+CM10"); }

    QChart *createChart(const DataTable &table)
    {
        QChart *chart = SizeChart2::createChart(table);
        chart->setContentsMargins(10,10,10,10);

        return chart;
    }
};

class SizeChart4: public SizeChart3
{
public:
    QString name() { return QObject::tr("PA+T+CM10+M25"); }

    QChart *createChart(const DataTable &table)
    {
        QChart *chart = SizeChart3::createChart(table);
        chart->setMargins(QMargins(30,30,30,30));
        return chart;
    }
};

class SizeChart5: public SizeChart4
{
public:
    QString name() { return QObject::tr("PA+T+CM10+M25+AX"); }

    QChart *createChart(const DataTable &table)
    {
        QChart *chart = SizeChart4::createChart(table);
        chart->createDefaultAxes();
        chart->axisY()->hide();
        return chart;
    }
};

class SizeChart6: public SizeChart4
{
public:
    QString name() { return QObject::tr("PA+T+CM10+M25+AY"); }

    QChart *createChart(const DataTable &table)
    {
        QChart *chart = SizeChart4::createChart(table);
        chart->createDefaultAxes();
        chart->axisX()->hide();
        return chart;
    }
};

class SizeChart7: public SizeChart4
{
public:
    QString name() { return QObject::tr("PA+T+CM10+M25+AX+AY"); }

    QChart *createChart(const DataTable &table)
    {
        QChart *chart = SizeChart4::createChart(table);
        chart->createDefaultAxes();
        return chart;
    }
};

DECLARE_CHART(SizeChart)
DECLARE_CHART(SizeChart2)
DECLARE_CHART(SizeChart3)
DECLARE_CHART(SizeChart4)
DECLARE_CHART(SizeChart5)
DECLARE_CHART(SizeChart6)
DECLARE_CHART(SizeChart7)
