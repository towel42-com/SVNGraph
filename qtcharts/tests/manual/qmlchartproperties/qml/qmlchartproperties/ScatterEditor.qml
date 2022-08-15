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

import QtQuick 2.0

Flow {
    id: flow
    spacing: 5
    flow: Flow.TopToBottom
    property variant series

    Button {
        text: "visible"
        onClicked: series.visible = !series.visible;
    }
    Button {
        text: "opacity +"
        onClicked: series.opacity += 0.1;
    }
    Button {
        text: "opacity -"
        onClicked: series.opacity -= 0.1;
    }
    Button {
        text: "color"
        onClicked: series.color = main.nextColor();
    }
    Button {
        text: "borderColor"
        onClicked: series.borderColor = main.nextColor();
    }
    Button {
        text: series != undefined ? "borderWidth + (" + series.borderWidth + ")" : ""
        onClicked: series.borderWidth += 0.5;
    }
    Button {
        text: series != undefined ? "borderWidth - (" + series.borderWidth + ")" : ""
        onClicked: series.borderWidth -= 0.5;
    }
    Button {
        text: "markerSize +"
        onClicked: series.markerSize += 1.0;
    }
    Button {
        text: "markerSize -"
        onClicked: series.markerSize -= 1.0;
    }
    Button {
        text: "markerShape"
        onClicked: series.markerShape = ((series.markerShape + 1) % 2);
    }
    Button {
        text: "point labels visible"
        onClicked: series.pointLabelsVisible = !series.pointLabelsVisible;
    }
    Button {
        text: "point labels clipping"
        onClicked: series.pointLabelsClipping = !series.pointLabelsClipping;
    }
    Button {
        text: "point labels format"
        onClicked: {
            if (series.pointLabelsFormat === "@xPoint, @yPoint")
                series.pointLabelsFormat = "(@xPoint)"
            else
                series.pointLabelsFormat = "@xPoint, @yPoint"
        }
    }
    Button {
        text: "point labels font"
        onClicked: {
            if (series.pointLabelsFont.family === "Times")
                series.pointLabelsFont.family = "Courier";
            else
                series.pointLabelsFont.family = "Times";
        }
    }
    Button {
        text: "point labels color"
        onClicked: series.pointLabelsColor = main.nextColor();
    }
    Button {
        text: "point labels clipping"
        onClicked: series.pointLabelsClipping = !series.pointLabelsClipping;
    }
    Button {
        text: "append point"
        onClicked: series.append(series.count - 1, series.count - 1);
    }
    Button {
        text: "replace point"
        onClicked: {
            var xyPoint = series.at(series.count - 1);
            series.replace(series.count - 1, xyPoint.x, xyPoint.y + 0.1);
        }
    }
    Button {
        text: "remove point"
        onClicked: series.remove(series.count - 1);
    }
    Button {
        text: "remove points"
        onClicked: {
            var count = 3;
            if (series.count < 3)
                count = series.count
            var index = series.count - count;
            series.removePoints(index, count);
        }
    }
    Button {
        text: "insert point"
        onClicked: series.insert(series.count - 2, series.count - 2, series.count - 2);
    }
    Button {
        text: "clear"
        onClicked: series.clear();
    }
}
