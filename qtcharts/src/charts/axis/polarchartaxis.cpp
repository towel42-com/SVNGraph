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

#include <private/polarchartaxis_p.h>
#include <private/qabstractaxis_p.h>
#include <private/chartpresenter_p.h>
#include <QtCharts/QValueAxis>

QT_CHARTS_BEGIN_NAMESPACE

PolarChartAxis::PolarChartAxis(QAbstractAxis *axis, QGraphicsItem *item, bool intervalAxis)
    : ChartAxisElement(axis, item, intervalAxis)
{
}

PolarChartAxis::~PolarChartAxis()
{

}

void PolarChartAxis::setGeometry(const QRectF &axis, const QRectF &grid)
{
    Q_UNUSED(grid);
    setAxisGeometry(axis);

    if (isEmpty()) {
        prepareGeometryChange();
        return;
    }

    QVector<qreal> layout = calculateLayout();
    updateLayout(layout);
}

QRectF PolarChartAxis::gridGeometry() const
{
    return QRectF();
}

void PolarChartAxis::updateLayout(QVector<qreal> &layout)
{
    int diff = ChartAxisElement::layout().size() - layout.size();

    if (animation()) {
        switch (presenter()->state()) {
        case ChartPresenter::ZoomInState:
        case ChartPresenter::ZoomOutState:
        case ChartPresenter::ScrollUpState:
        case ChartPresenter::ScrollLeftState:
        case ChartPresenter::ScrollDownState:
        case ChartPresenter::ScrollRightState:
        case ChartPresenter::ShowState:
            animation()->setAnimationType(AxisAnimation::DefaultAnimation);
            break;
        }
        // Update to "old" geometry before starting animation to avoid incorrectly sized
        // axes lingering in wrong position compared to series plot before animation can kick in.
        // Note that the position mismatch still exists even with this update, but it will be
        // far less ugly.
        updateGeometry();
    }

    if (diff > 0)
        deleteItems(diff);
    else if (diff < 0)
        createItems(-diff);

    updateMinorTickItems();

    if (animation()) {
        animation()->setValues(ChartAxisElement::layout(), layout);
        presenter()->startAnimation(animation());
    } else {
        setLayout(layout);
        updateGeometry();
    }
}

bool PolarChartAxis::isEmpty()
{
    return !axisGeometry().isValid() || qFuzzyIsNull(min() - max());
}

void PolarChartAxis::deleteItems(int count)
{
    QList<QGraphicsItem *> gridLines = gridItems();
    QList<QGraphicsItem *> labels = labelItems();
    QList<QGraphicsItem *> shades = shadeItems();
    QList<QGraphicsItem *> axis = arrowItems();

    for (int i = 0; i < count; ++i) {
        if (gridItems().size() == 1 || (((gridLines.size() + 1) % 2) && gridLines.size() > 0))
            delete(shades.takeLast());
        delete(gridLines.takeLast());
        delete(labels.takeLast());
        delete(axis.takeLast());
    }
}

void PolarChartAxis::handleShadesBrushChanged(const QBrush &brush)
{
    foreach (QGraphicsItem *item, shadeItems())
        static_cast<QGraphicsPathItem *>(item)->setBrush(brush);
}

void PolarChartAxis::handleShadesPenChanged(const QPen &pen)
{
    foreach (QGraphicsItem *item, shadeItems())
        static_cast<QGraphicsPathItem *>(item)->setPen(pen);
}

//#include "moc_polarchartaxis_p.cpp"

QT_CHARTS_END_NAMESPACE
