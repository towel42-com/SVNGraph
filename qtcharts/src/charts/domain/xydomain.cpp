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

#include <private/xydomain_p.h>
#include <private/qabstractaxis_p.h>
#include <QtCore/QtMath>

QT_CHARTS_BEGIN_NAMESPACE

XYDomain::XYDomain(QObject *parent)
    : AbstractDomain(parent),
    fAllowXZoom( true ),
    fAllowYZoom( true )
{
}

XYDomain::~XYDomain()
{
}

void XYDomain::setRange(qreal minX, qreal maxX, qreal minY, qreal maxY)
{
    bool axisXChanged = false;
    bool axisYChanged = false;

    if ( ( fAllowXZoom || ((m_minX == 0) && ( m_maxX == 0)) ) && ( !qFuzzyCompare(m_minX, minX) || !qFuzzyCompare(m_maxX, maxX))) {
        m_minX = minX;
        m_maxX = maxX;
        axisXChanged = true;
        if(!m_signalsBlocked)
            emit rangeHorizontalChanged(m_minX, m_maxX);
    }

    if( ( fAllowYZoom || ( ( m_minY == 0 ) && ( m_maxY == 0 ) ) ) && ( !qFuzzyCompare( m_minY, minY ) || !qFuzzyCompare( m_maxY, maxY ) ) ) {
        m_minY = minY;
        m_maxY = maxY;
        axisYChanged = true;
        if(!m_signalsBlocked)
            emit rangeVerticalChanged(m_minY, m_maxY);
    }

    if (axisXChanged || axisYChanged)
        emit updated();
}


void XYDomain::zoomIn(const QRectF &rect)
{
    storeZoomReset();
    qreal dx = spanX() / m_size.width();
    qreal dy = spanY() / m_size.height();

    qreal maxX = m_maxX;
    qreal minX = m_minX;
    qreal minY = m_minY;
    qreal maxY = m_maxY;

    maxX = minX + dx * rect.right();
    minX = minX + dx * rect.left();
    minY = maxY - dy * rect.bottom();
    maxY = maxY - dy * rect.top();

    if ((maxX - minX) == spanX()) {
        minX = m_minX;
        maxX = m_maxX;
    }
    if ((maxY - minY) == spanY()) {
        minY = m_minY;
        maxY = m_maxY;
    }

    setRange(minX, maxX, minY, maxY);
}

void XYDomain::zoomOut(const QRectF &rect)
{
    storeZoomReset();
    qreal dx = spanX() / rect.width();
    qreal dy = spanY() / rect.height();

    qreal maxX = m_maxX;
    qreal minX = m_minX;
    qreal minY = m_minY;
    qreal maxY = m_maxY;

    minX = maxX - dx * rect.right();
    maxX = minX + dx * m_size.width();
    maxY = minY + dy * rect.bottom();
    minY = maxY - dy * m_size.height();

    if ((maxX - minX) == spanX()) {
        minX = m_minX;
        maxX = m_maxX;
    }
    if ((maxY - minY) == spanY()) {
        minY = m_minY;
        maxY = m_maxY;
    }

    setRange(minX, maxX, minY, maxY);
}

void XYDomain::move(qreal dx, qreal dy)
{
    qreal x = spanX() / m_size.width();
    qreal y = spanY() / m_size.height();

    qreal maxX = m_maxX;
    qreal minX = m_minX;
    qreal minY = m_minY;
    qreal maxY = m_maxY;

    if (dx != 0) {
        minX = minX + x * dx;
        maxX = maxX + x * dx;
    }
    if (dy != 0) {
        minY = minY + y * dy;
        maxY = maxY + y * dy;
    }
    setRange(minX, maxX, minY, maxY);
}

QPointF XYDomain::calculateGeometryPoint(const QPointF &point, bool &ok) const
{
    const qreal deltaX = m_size.width() / (m_maxX - m_minX);
    const qreal deltaY = m_size.height() / (m_maxY - m_minY);
    qreal x = (point.x() - m_minX) * deltaX;
    qreal y = (point.y() - m_minY) * -deltaY + m_size.height();
    ok = true;
    return QPointF(x, y);
}

QVector<QPointF> XYDomain::calculateGeometryPoints(const QVector<QPointF> &vector) const
{
    const qreal deltaX = m_size.width() / (m_maxX - m_minX);
    const qreal deltaY = m_size.height() / (m_maxY - m_minY);

    QVector<QPointF> result;
    result.resize(vector.count());

    for (int i = 0; i < vector.count(); ++i) {
        qreal x = (vector[i].x() - m_minX) * deltaX;
        qreal y = (vector[i].y() - m_minY) * -deltaY + m_size.height();
        result[i].setX(x);
        result[i].setY(y);
    }
    return result;
}

QPointF XYDomain::calculateDomainPoint(const QPointF &point) const
{
    const qreal deltaX = m_size.width() / (m_maxX - m_minX);
    const qreal deltaY = m_size.height() / (m_maxY - m_minY);
    qreal x = point.x() / deltaX + m_minX;
    qreal y = (point.y() - m_size.height()) / (-deltaY) + m_minY;
    return QPointF(x, y);
}

// operators

bool QT_CHARTS_AUTOTEST_EXPORT operator== (const XYDomain &domain1, const XYDomain &domain2)
{
    return (qFuzzyCompare(domain1.m_maxX, domain2.m_maxX)
            && qFuzzyCompare(domain1.m_maxY, domain2.m_maxY)
            && qFuzzyCompare(domain1.m_minX, domain2.m_minX)
            && qFuzzyCompare(domain1.m_minY, domain2.m_minY));
}


bool QT_CHARTS_AUTOTEST_EXPORT operator!= (const XYDomain &domain1, const XYDomain &domain2)
{
    return !(domain1 == domain2);
}


QDebug QT_CHARTS_AUTOTEST_EXPORT operator<<(QDebug dbg, const XYDomain &domain)
{
#ifdef QT_NO_TEXTSTREAM
    Q_UNUSED(domain)
#else
    dbg.nospace() << "AbstractDomain(" << domain.m_minX << ',' << domain.m_maxX << ',' << domain.m_minY << ',' << domain.m_maxY << ')' << domain.m_size;
#endif
    return dbg.maybeSpace();
}

//#include "moc_xydomain_p.cpp"

QT_CHARTS_END_NAMESPACE
