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
        text: "point labels visible"
        onClicked: series.pointLabelsVisible = !series.pointLabelsVisible;
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
        id: upperButton
        text: "upper series"
        unpressedColor: "#79bd8f"
        onClicked: {
            lineEditor.visible = true;
            color = "#00a388";
            lowerButton.color = "#79bd8f";
            lineEditor.series = series.upperSeries;
        }
    }
    Button {
        id: lowerButton
        text: "lower series"
        unpressedColor: "#79bd8f"
        onClicked: {
            lineEditor.visible = true;
            color = "#00a388";
            upperButton.color = "#79bd8f";
            lineEditor.series = series.lowerSeries;
        }
    }
    LineEditor {
        id: lineEditor
        visible: false
    }

    onSeriesChanged: lineEditor.series = series.upperSeries;
}
