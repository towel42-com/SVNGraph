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

#ifndef QCATEGORYAXIS_P_H
#define QCATEGORYAXIS_P_H

#include <QtCharts/QCategoryAxis>
#include <private/qvalueaxis_p.h>

QT_CHARTS_BEGIN_NAMESPACE

typedef QPair<qreal, qreal> Range;

class QCategoryAxisPrivate : public QValueAxisPrivate
{
    Q_OBJECT

public:
    QCategoryAxisPrivate(QCategoryAxis *q);
    ~QCategoryAxisPrivate();

    void initializeGraphics(QGraphicsItem* parent);
    int ticksCount() const;

private:
    QMap<QString , Range> m_categoriesMap;
    QStringList m_categories;
    qreal m_categoryMinimum;
    QCategoryAxis::AxisLabelsPosition m_labelsPosition;

private:
    Q_DECLARE_PUBLIC(QCategoryAxis)
};

QT_CHARTS_END_NAMESPACE

#endif // QCATEGORYAXIS_P_H
