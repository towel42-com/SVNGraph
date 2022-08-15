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

#ifndef QAREASERIES_H
#define QAREASERIES_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QAbstractSeries>
#include <QtGui/QPen>
#include <QtGui/QBrush>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QAreaSeriesPrivate;

class QT_CHARTS_EXPORT QAreaSeries : public QAbstractSeries
{
    Q_OBJECT
    Q_PROPERTY(QLineSeries *upperSeries READ upperSeries)
    Q_PROPERTY(QLineSeries *lowerSeries READ lowerSeries)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(QString pointLabelsFormat READ pointLabelsFormat WRITE setPointLabelsFormat NOTIFY pointLabelsFormatChanged)
    Q_PROPERTY(bool pointLabelsVisible READ pointLabelsVisible WRITE setPointLabelsVisible NOTIFY pointLabelsVisibilityChanged)
    Q_PROPERTY(QFont pointLabelsFont READ pointLabelsFont WRITE setPointLabelsFont NOTIFY pointLabelsFontChanged)
    Q_PROPERTY(QColor pointLabelsColor READ pointLabelsColor WRITE setPointLabelsColor NOTIFY pointLabelsColorChanged)
    Q_PROPERTY(bool pointLabelsClipping READ pointLabelsClipping WRITE setPointLabelsClipping NOTIFY pointLabelsClippingChanged)

public:
    explicit QAreaSeries(QObject *parent = Q_NULLPTR);
    explicit QAreaSeries(QLineSeries *upperSeries, QLineSeries *lowerSeries = Q_NULLPTR);
    ~QAreaSeries();

public:
    QAbstractSeries::SeriesType type() const;

    void setUpperSeries(QLineSeries *series);
    QLineSeries *upperSeries() const;
    void setLowerSeries(QLineSeries *series);
    QLineSeries *lowerSeries() const;

    void setPen(const QPen &pen);
    QPen pen() const;

    void setBrush(const QBrush &brush);
    QBrush brush() const;

    void setColor(const QColor &color);
    QColor color() const;

    void setBorderColor(const QColor &color);
    QColor borderColor() const;

    void setPointsVisible(bool visible = true);
    bool pointsVisible() const;

    void setPointLabelsFormat(const QString &format);
    QString pointLabelsFormat() const;

    void setPointLabelsVisible(bool visible = true);
    bool pointLabelsVisible() const;

    void setPointLabelsFont(const QFont &font);
    QFont pointLabelsFont() const;

    void setPointLabelsColor(const QColor &color);
    QColor pointLabelsColor() const;

    void setPointLabelsClipping(bool enabled = true);
    bool pointLabelsClipping() const;

Q_SIGNALS:
    void clicked(const QPointF &point);
    void hovered(const QPointF &point, bool state);
    void pressed(const QPointF &point);
    void released(const QPointF &point);
    void doubleClicked(const QPointF &point);
    void selected();
    void colorChanged(QColor color);
    void borderColorChanged(QColor color);
    void pointLabelsFormatChanged(const QString &format);
    void pointLabelsVisibilityChanged(bool visible);
    void pointLabelsFontChanged(const QFont &font);
    void pointLabelsColorChanged(const QColor &color);
    void pointLabelsClippingChanged(bool clipping);

private:
    Q_DECLARE_PRIVATE(QAreaSeries)
    Q_DISABLE_COPY(QAreaSeries)
    friend class AreaLegendMarker;
    friend class AreaChartItem;
    friend class QAreaLegendMarkerPrivate;
};

QT_CHARTS_END_NAMESPACE

#endif // QAREASERIES_H
