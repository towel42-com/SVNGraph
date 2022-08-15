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

#ifndef ABSTRACTCHARTLAYOUT_H
#define ABSTRACTCHARTLAYOUT_H

#include <QtWidgets/QGraphicsLayout>
#include <QtCore/QMargins>
#include <QtCharts/QChartGlobal>

QT_CHARTS_BEGIN_NAMESPACE

class ChartTitle;
class ChartAxisElement;
class ChartPresenter;
class QLegend;
class ChartBackground;

class AbstractChartLayout : public QGraphicsLayout
{
public:
    AbstractChartLayout(ChartPresenter *presenter);
    virtual ~AbstractChartLayout();

    virtual void setMargins(const QMargins &margins);
    virtual QMargins margins() const;
    virtual void setGeometry(const QRectF &rect);

protected:
    virtual QRectF calculateBackgroundGeometry(const QRectF &geometry, ChartBackground *background) const;
    virtual QRectF calculateBackgroundMinimum(const QRectF &minimum) const;
    virtual QRectF calculateContentGeometry(const QRectF &geometry) const;
    virtual QRectF calculateContentMinimum(const QRectF &minimum) const;
    virtual QRectF calculateTitleGeometry(const QRectF &geometry, ChartTitle *title) const;
    virtual QRectF calculateTitleMinimum(const QRectF &minimum, ChartTitle *title) const;
    virtual QRectF calculateLegendGeometry(const QRectF &geometry, QLegend *legend) const;
    virtual QRectF calculateLegendMinimum(const QRectF &minimum, QLegend *legend) const;

    virtual QRectF calculateAxisGeometry(const QRectF &geometry, const QList<ChartAxisElement *>& axes) const = 0;
    virtual QRectF calculateAxisMinimum(const QRectF &minimum, const QList<ChartAxisElement *>& axes) const = 0;

    // from QGraphicsLayout
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    int count() const { return 0; }
    QGraphicsLayoutItem *itemAt(int) const { return 0; };
    void removeAt(int) {};

    ChartPresenter *m_presenter;
    QMargins m_margins;
    QRectF m_minChartRect;
    QRectF m_minAxisRect;
};

QT_CHARTS_END_NAMESPACE

#endif // ABSTRACTCHARTLAYOUT_H
