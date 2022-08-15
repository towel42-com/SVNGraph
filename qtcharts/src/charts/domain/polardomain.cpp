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

#include <private/polardomain_p.h>
#include <private/qabstractaxis_p.h>
#include <QtCore/QtMath>

QT_CHARTS_BEGIN_NAMESPACE

PolarDomain::PolarDomain(QObject *parent)
    : AbstractDomain(parent)
{
}

PolarDomain::~PolarDomain()
{
}

void PolarDomain::setSize(const QSizeF &size)
{
    Q_ASSERT(size.width() == size.height());
    m_radius = size.height() / 2.0;
    m_center = QPointF(m_radius, m_radius);
    AbstractDomain::setSize(size);
}

QPointF PolarDomain::calculateGeometryPoint(const QPointF &point, bool &ok) const
{
    qreal r = 0.0;
    qreal a = toAngularCoordinate(point.x(), ok);
    if (ok)
        r = toRadialCoordinate(point.y(), ok);
    if (ok) {
        return m_center + polarCoordinateToPoint(a, r);
    } else {
        qWarning() << "Logarithm of negative value is undefined. Empty layout returned.";
        return QPointF();
    }
}

QVector<QPointF> PolarDomain::calculateGeometryPoints(const QVector<QPointF> &vector) const
{
    QVector<QPointF> result;
    result.resize(vector.count());
    bool ok;
    qreal r = 0.0;
    qreal a = 0.0;

    for (int i = 0; i < vector.count(); ++i) {
        a = toAngularCoordinate(vector[i].x(), ok);
        if (ok)
            r = toRadialCoordinate(vector[i].y(), ok);
        if (ok) {
            result[i] = m_center + polarCoordinateToPoint(a, r);
        } else {
            qWarning() << "Logarithm of negative value is undefined. Empty layout returned.";
            return QVector<QPointF>();
        }
    }

    return result;
}

QPointF PolarDomain::polarCoordinateToPoint(qreal angularCoordinate, qreal radialCoordinate) const
{
    qreal dx = qSin(angularCoordinate * (M_PI / 180)) * radialCoordinate;
    qreal dy = qCos(angularCoordinate * (M_PI / 180)) * radialCoordinate;

    return QPointF(dx, -dy);
}

//#include "moc_polardomain_p.cpp"

QT_CHARTS_END_NAMESPACE
