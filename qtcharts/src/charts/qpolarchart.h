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

#ifndef QPOLARCHART_H
#define QPOLARCHART_H

#include <QtCharts/QChart>

QT_CHARTS_BEGIN_NAMESPACE

class QAbstractSeries;
class QAbstractAxis;

class QT_CHARTS_EXPORT QPolarChart : public QChart
{
    Q_OBJECT
    Q_ENUMS(PolarOrientation)
    Q_FLAGS(PolarOrientations)

public:
    enum PolarOrientation {
        PolarOrientationRadial = 0x1,
        PolarOrientationAngular = 0x2
    };
    Q_DECLARE_FLAGS(PolarOrientations, PolarOrientation)

public:
    explicit QPolarChart(QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags wFlags = Qt::WindowFlags());
    ~QPolarChart();

    void addAxis(QAbstractAxis *axis, PolarOrientation polarOrientation);

    QList<QAbstractAxis*> axes(PolarOrientations polarOrientation = PolarOrientations(PolarOrientationRadial | PolarOrientationAngular), QAbstractSeries *series = Q_NULLPTR) const;

    static PolarOrientation axisPolarOrientation(QAbstractAxis *axis);

private:
    Q_DISABLE_COPY(QPolarChart)
};

QT_CHARTS_END_NAMESPACE

#endif // QCHART_H
