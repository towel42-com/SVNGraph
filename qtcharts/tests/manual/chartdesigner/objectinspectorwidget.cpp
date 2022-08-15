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

#include "objectinspectorwidget.h"
#include <QtGui/QIcon>
#include <QtGui/QPainter>
#include <QtGui/QPixmapCache>

InspectorWidget::InspectorWidget()
{

}

InspectorWidget::~InspectorWidget()
{

}

QIcon InspectorWidget::getColorIcon(const QColor& color,const QSize &size) const
{

    QString key = QString("COLOR_ICON=%1:%2x%3").arg(color.name()).arg(size.width()).arg(size.height());

    QPixmap pixmap(size);

    if (!QPixmapCache::find(key, &pixmap)) {
        pixmap.fill(Qt::white);
        QPainter painter(&pixmap);
        painter.fillRect(2, 2, 46, 46, color);
        QPixmapCache::insert(key, pixmap);
    }

    return QIcon(pixmap);
}
