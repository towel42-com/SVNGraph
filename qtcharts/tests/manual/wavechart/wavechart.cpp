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

#include "wavechart.h"
#include <cmath>

QT_CHARTS_USE_NAMESPACE

#define PI 3.14159265358979
static const int numPoints =100;

WaveChart::WaveChart(QChart* chart, QWidget* parent) :
    QChartView(chart, parent),
    m_series(new QLineSeries()),
    m_wave(0),
    m_step(2 * PI / numPoints)
{
    QPen blue(Qt::blue);
    blue.setWidth(3);
    m_series->setPen(blue);

    chart->legend()->setVisible(false);

    QTime now = QTime::currentTime();
    qsrand((uint) now.msec());

    int fluctuate = 100;

    for (qreal x = 0; x <= 2 * PI; x += m_step) {
        m_series->append(x, fabs(sin(x) * fluctuate));
    }

    chart->addSeries(m_series);
    chart->createDefaultAxes();

    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer.setInterval(5000);
    m_timer.start();
}

void WaveChart::update()
{

    int fluctuate;
    const QList<QPointF>& points = m_series->points();
    for (qreal i = 0, x = 0; x <= 2 * PI; x += m_step, i++) {
        fluctuate = qrand() % 100;
        m_series->replace(x,points[i].y(),x,fabs(sin(x) * fluctuate));

    }

}
