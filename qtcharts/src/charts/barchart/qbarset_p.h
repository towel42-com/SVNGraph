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

#ifndef QBARSET_P_H
#define QBARSET_P_H

#include <QtCharts/QBarSet>
#include <QtCore/QMap>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QFont>

QT_CHARTS_BEGIN_NAMESPACE

class QBarSetPrivate : public QObject
{
    Q_OBJECT

public:
    QBarSetPrivate(const QString label, QBarSet *parent);
    ~QBarSetPrivate();

    void append(QPointF value);
    void append(QList<QPointF> values);
    void append(QList<qreal> values);

    void insert(const int index, const qreal value);
    void insert(const int index, const QPointF value);
    int remove(const int index, const int count);

    void replace(const int index, const qreal value);
    void replace(const int index, const QPointF value);

    qreal pos(const int index);
    qreal value(const int index);

Q_SIGNALS:
    void restructuredBars();
    void updatedBars();
    void updatedLayout();

public:
    QBarSet * const q_ptr;
    QString m_label;
    QList<QPointF> m_values;
    QPen m_pen;
    QBrush m_brush;
    QBrush m_labelBrush;
    QFont m_labelFont;

    friend class QBarSet;
};

QT_CHARTS_END_NAMESPACE

#endif // QBARSETPRIVATE_P_H
