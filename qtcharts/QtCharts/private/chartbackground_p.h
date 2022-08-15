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

//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef CHARTBACKGROUND_H
#define CHARTBACKGROUND_H

#include <QtCharts/QChartGlobal>
#include <QtWidgets/QGraphicsRectItem>

QT_BEGIN_NAMESPACE
class QGraphicsDropShadowEffect;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE

class ChartBackground: public QGraphicsRectItem
{
public:
    ChartBackground(QGraphicsItem *parent = 0);
    ~ChartBackground();

    void setDiameter(qreal diameter);
    qreal diameter() const;
    void setDropShadowEnabled(bool enabled);
    bool isDropShadowEnabled() { return m_dropShadow != 0; }

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    qreal m_diameter;
    QGraphicsDropShadowEffect *m_dropShadow;
};

QT_CHARTS_END_NAMESPACE

#endif /* CHARTBACKGROUND_H */


