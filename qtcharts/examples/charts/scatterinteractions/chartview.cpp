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

#include "chartview.h"
#include <QtCore/QtMath>
#include <QtCore/QDebug>

QT_CHARTS_USE_NAMESPACE

ChartView::ChartView(QWidget *parent)
    : QChartView(new QChart(), parent),
      m_scatter(0),
      m_scatter2(0)
{
    setRenderHint(QPainter::Antialiasing);

    chart()->setTitle("Click to interact with scatter points");

    m_scatter = new QScatterSeries();
    m_scatter->setName("scatter1");
    for (qreal x(0.5); x <= 4.0; x += 0.5) {
        for (qreal y(0.5); y <= 4.0; y += 0.5) {
            *m_scatter << QPointF(x, y);
        }
    }
    m_scatter2 = new QScatterSeries();
    m_scatter2->setName("scatter2");

    chart()->addSeries(m_scatter2);
    chart()->addSeries(m_scatter);
    chart()->createDefaultAxes();
    chart()->axisX()->setRange(0, 4.5);
    chart()->axisY()->setRange(0, 4.5);

    connect(m_scatter, SIGNAL(clicked(QPointF)), this, SLOT(handleClickedPoint(QPointF)));
}

ChartView::~ChartView()
{
}

void ChartView::handleClickedPoint(const QPointF &point)
{
    QPointF clickedPoint = point;
    // Find the closest point from series 1
    QPointF closest(INT_MAX, INT_MAX);
    qreal distance(INT_MAX);
    foreach (QPointF currentPoint, m_scatter->points()) {
        qreal currentDistance = qSqrt((currentPoint.x() - clickedPoint.x())
                                      * (currentPoint.x() - clickedPoint.x())
                                      + (currentPoint.y() - clickedPoint.y())
                                      * (currentPoint.y() - clickedPoint.y()));
        if (currentDistance < distance) {
            distance = currentDistance;
            closest = currentPoint;
        }
    }

    // Remove the closes point from series 1 and append it to series 2
    m_scatter->remove(closest);
    m_scatter2->append(closest);
}
