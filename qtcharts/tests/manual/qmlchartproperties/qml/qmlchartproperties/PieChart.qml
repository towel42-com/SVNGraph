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
import QtCharts 2.0

ChartView {
    id: chart
    title: "pie series"
    animationOptions: ChartView.SeriesAnimations

    property variant series: pieSeries

    PieSeries {
        id: pieSeries
        name: "pie"
        PieSlice { label: "slice1"; value: 11;
            onValueChanged:         console.log("slice.onValueChanged: " + value);
            onLabelVisibleChanged:  console.log("slice.onLabelVisibleChanged: " + labelVisible);
            onPenChanged:           console.log("slice.onPenChanged: " + pen);
            onBorderColorChanged:   console.log("slice.onBorderColorChanged: " + borderColor);
            onBorderWidthChanged:   console.log("slice.onBorderWidthChanged: " + borderWidth);
            onBrushChanged:         console.log("slice.onBrushChanged: " + brush);
            onColorChanged:         console.log("slice.onColorChanged: " + color);
            onLabelColorChanged:    console.log("slice.onLabelColorChanged: " + labelColor);
            onLabelBrushChanged:    console.log("slice.onLabelBrushChanged: " + labelBrush);
            onLabelFontChanged:     console.log("slice.onLabelFontChanged: " + labelFont);
            onPercentageChanged:    console.log("slice.onPercentageChanged: " + percentage);
            onStartAngleChanged:    console.log("slice.onStartAngleChanged: " + startAngle);
            onAngleSpanChanged:     console.log("slice.onAngleSpanChanged: " + angleSpan);
            onClicked:              console.log("slice.onClicked: " + label);
            onHovered:              console.log("slice.onHovered: " + state);
            onPressed:              console.log("slice.onPressed: " + label);
            onReleased:             console.log("slice.onReleased: " + label);
            onDoubleClicked:        console.log("slice.onDoubleClicked: " + label);
        }
        PieSlice { label: "slice2"; value: 22 }
        PieSlice { label: "slice3"; value: 33 }
        PieSlice { label: "slice4"; value: 44 }

        onNameChanged:              console.log("pieSeries.onNameChanged: " + name);
        onVisibleChanged:           console.log("pieSeries.onVisibleChanged: " + series.visible);
        onOpacityChanged:           console.log("pieSeries.onOpacityChanged: " + opacity);
        onClicked:                  console.log("pieSeries.onClicked: " + slice.label);
        onHovered:                  console.log("pieSeries.onHovered: " + slice.label);
        onAdded:                    console.log("pieSeries.onAdded: " + slices);
        onSliceAdded:               console.log("pieSeries.onSliceAdded: " + slice.label);
        onRemoved:                  console.log("pieSeries.onRemoved: " + slices);
        onSliceRemoved:             console.log("pieSeries.onSliceRemoved: " + slice.label);
        onCountChanged:             console.log("pieSeries.onCountChanged: " + count);
        onSumChanged:               console.log("pieSeries.onSumChanged: " + sum);
        onPressed:                  console.log("pieSeries.onPressed: " + slice.label);
        onReleased:                 console.log("pieSeries.onReleased: " + slice.label);
        onDoubleClicked:            console.log("pieSeries.onDoubleClicked: " + slice.label);
    }
}
