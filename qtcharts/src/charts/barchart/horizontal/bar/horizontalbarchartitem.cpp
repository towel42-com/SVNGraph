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

#include <private/horizontalbarchartitem_p.h>
#include <private/qabstractbarseries_p.h>
#include <private/qbarset_p.h>
#include <private/bar_p.h>

QT_CHARTS_BEGIN_NAMESPACE

HorizontalBarChartItem::HorizontalBarChartItem(QAbstractBarSeries *series, QGraphicsItem* item)
    : AbstractBarChartItem(series, item)
{
}

void HorizontalBarChartItem::initializeLayout()
{
    qreal categoryCount = m_series->d_func()->categoryCount();
    qreal setCount = m_series->count();
    qreal barWidth = m_series->d_func()->barWidth();

    m_layout.clear();
    for(int category = 0; category < categoryCount; category++) {
        for (int set = 0; set < setCount; set++) {
            QRectF rect;
            QPointF topLeft;
            QPointF bottomRight;
            if (domain()->type() == AbstractDomain::LogXYDomain || domain()->type() == AbstractDomain::LogXLogYDomain) {
                topLeft = domain()->calculateGeometryPoint(QPointF(domain()->minX(), category - barWidth / 2 + set/setCount * barWidth), m_validData);
                bottomRight = domain()->calculateGeometryPoint(QPointF(domain()->minX(), category - barWidth / 2 + (set + 1)/setCount * barWidth), m_validData);
            } else {
                topLeft = domain()->calculateGeometryPoint(QPointF(0, category - barWidth / 2 + set/setCount * barWidth), m_validData);
                bottomRight = domain()->calculateGeometryPoint(QPointF(0, category - barWidth / 2 + (set + 1)/setCount * barWidth), m_validData);
            }

            if (!m_validData)
                 return;

            rect.setTopLeft(topLeft);
            rect.setBottomRight(bottomRight);
            m_layout.append(rect.normalized());
        }
    }
}

QVector<QRectF> HorizontalBarChartItem::calculateLayout()
{
    QVector<QRectF> layout;

    // Use temporary qreals for accuracy
    qreal categoryCount = m_series->d_func()->categoryCount();
    qreal setCount = m_series->count();
    qreal barWidth = m_series->d_func()->barWidth();

    for(int category = 0; category < categoryCount; category++) {
        for (int set = 0; set < setCount; set++) {
            qreal value = m_series->barSets().at(set)->at(category);
            QRectF rect;
            QPointF topLeft;
            if (domain()->type() == AbstractDomain::LogXYDomain || domain()->type() == AbstractDomain::LogXLogYDomain)
                topLeft = domain()->calculateGeometryPoint(QPointF(domain()->minX(), category - barWidth / 2 + set/setCount * barWidth), m_validData);
            else
                topLeft = domain()->calculateGeometryPoint(QPointF(0, category - barWidth / 2 + set/setCount * barWidth), m_validData);

            QPointF bottomRight = domain()->calculateGeometryPoint(QPointF(value, category - barWidth / 2 + (set + 1)/setCount * barWidth), m_validData);

            rect.setTopLeft(topLeft);
            rect.setBottomRight(bottomRight);
            layout.append(rect.normalized());
        }
    }
    return layout;
}

//#include "moc_horizontalbarchartitem_p.cpp"

QT_CHARTS_END_NAMESPACE
