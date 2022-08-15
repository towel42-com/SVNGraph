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

#ifndef QCHARTVIEW_H
#define QCHARTVIEW_H

#include <QtCharts/QAbstractAxis>
#include <QtCharts/QAbstractSeries>
#include <QtCharts/QChart>
#include <QtWidgets/QGraphicsView>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QRubberBand;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE

class QChartViewPrivate;

class QT_CHARTS_EXPORT QChartView : public QGraphicsView
{
    Q_OBJECT
    Q_ENUMS(RubberBand)
public:

    enum  RubberBand {
        NoRubberBand = 0x0,
        VerticalRubberBand = 0x1,
        HorizontalRubberBand = 0x2,
        RectangleRubberBand = 0x3
    };

    Q_DECLARE_FLAGS(RubberBands, RubberBand)

    explicit QChartView(QWidget *parent = Q_NULLPTR);
    explicit QChartView(QChart *chart, QWidget *parent = Q_NULLPTR);
    ~QChartView();

    void setRubberBand(const RubberBands &rubberBands);
    RubberBands rubberBand() const;

    QChart *chart() const;
    virtual void setChart(QChart *chart);

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    QScopedPointer<QChartViewPrivate> d_ptr;

private:
    Q_DISABLE_COPY(QChartView)
};

QT_CHARTS_END_NAMESPACE

#endif // QCHARTVIEW_H
