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

#ifndef MODEL_H
#define MODEL_H

#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QPointF>
#include <QtCore/QTime>
#include <stdlib.h>

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;


class Model
{
private:
    Model() {}

public:
    static DataTable generateRandomData(int listCount, int valueMax, int valueCount)
    {
        DataTable dataTable;

        // set seed for random stuff
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

        // generate random data
        for (int i(0); i < listCount; i++) {
            DataList dataList;
            qreal yValue(0.1);
            for (int j(0); j < valueCount; j++) {
                yValue = yValue + (qreal)(qrand() % valueMax) / (qreal) valueCount;
                QPointF value(
                    (j + (qreal) qrand() / (qreal) RAND_MAX)
                    * ((qreal) valueMax / (qreal) valueCount), yValue);
                QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
                dataList << Data(value, label);
            }
            dataTable << dataList;
        }
        return dataTable;
    }
};

#endif
