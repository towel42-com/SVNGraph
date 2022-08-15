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
#include <QtCharts/QPieSeries>

class DonutChart: public Chart
{
public:
    QString name() { return QObject::tr("DonutChart"); }
    QString category()  { return QObject::tr("PieSeries"); }
    QString subCategory() { return QString::null; }

    QChart *createChart(const DataTable &table)
    {
        QChart *chart = new QChart();
        chart->setTitle("Donut chart");
        for (int i = 0, j = table.count(); i < table.count(); i++, j--) {
            QPieSeries *series = new QPieSeries(chart);
            foreach (Data data, table[i]) {
                QPieSlice *slice = series->append(data.second, data.first.y());
                if (data == table[i].first())
                    slice->setLabelVisible();
            }
            series->setPieSize(j / (qreal) table.count());
            if (j > 1)
                series->setHoleSize((j - 1) / (qreal) table.count() + 0.1);
            series->setHorizontalPosition(0.5);
            series->setVerticalPosition(0.5);
            chart->addSeries(series);
        }
        return chart;
    }
};

DECLARE_CHART(DonutChart)
