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

#ifndef QVALUEAXIS_H
#define QVALUEAXIS_H

#include <QtCharts/QAbstractAxis>

QT_CHARTS_BEGIN_NAMESPACE

class QValueAxisPrivate;

class QT_CHARTS_EXPORT QValueAxis : public QAbstractAxis
{
    Q_OBJECT
    Q_PROPERTY(int tickCount READ tickCount WRITE setTickCount NOTIFY tickCountChanged)
    Q_PROPERTY(qreal min READ min WRITE setMin NOTIFY minChanged)
    Q_PROPERTY(qreal max READ max WRITE setMax NOTIFY maxChanged)
    Q_PROPERTY(QString labelFormat READ labelFormat WRITE setLabelFormat NOTIFY labelFormatChanged)
    Q_PROPERTY(int minorTickCount READ minorTickCount WRITE setMinorTickCount NOTIFY minorTickCountChanged)

public:
    explicit QValueAxis(QObject *parent = Q_NULLPTR);
    ~QValueAxis();

protected:
    QValueAxis(QValueAxisPrivate &d, QObject *parent = Q_NULLPTR);

public:
    AxisType type() const;

    //range handling
    void setMin(qreal min);
    qreal min() const;
    void setMax(qreal max);
    qreal max() const;
    void setRange(qreal min, qreal max);

    //ticks handling
    void setTickCount(int count);
    int tickCount() const;
    void setMinorTickCount(int count);
    int minorTickCount() const;

    void setLabelFormat(const QString &format);
    QString labelFormat() const;

public Q_SLOTS:
    void applyNiceNumbers();

Q_SIGNALS:
    void minChanged(qreal min);
    void maxChanged(qreal max);
    void rangeChanged(qreal min, qreal max);
    void tickCountChanged(int tickCount);
    void minorTickCountChanged(int tickCount);
    void labelFormatChanged(const QString &format);

private:
    Q_DECLARE_PRIVATE(QValueAxis)
    Q_DISABLE_COPY(QValueAxis)
};

QT_CHARTS_END_NAMESPACE

#endif // QVALUEAXIS_H
