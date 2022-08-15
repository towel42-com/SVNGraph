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

Item {
    id: chartViewSelector
    width: parent.width
    height: parent.height
    signal seriesAdded(string seriesName, color seriesColor)

    function highlightSeries(seriesName) {
        if (seriesName == "") {
            if (state != "")
                state = "";

            for (var i = 0; i < chartViewStacked.count; i++)
                chartViewStacked.series(i).opacity = 1.0;
        } else {
            var targetOpacity = 0.1;
            for (var j = 0; j < chartViewStacked.count; j++) {
                if (chartViewStacked.series(j).name != seriesName)
                    chartViewStacked.series(j).opacity = 0.25;
                else if (state == "highlight")
                    chartViewSelected.selectedSeries = chartViewStacked.series(j);
            }
        }
    }

    function selectSeries(seriesName) {
        for (var i = 0; i < chartViewStacked.count; i++) {
            if (chartViewStacked.series(i).name == seriesName) {
                chartViewSelected.selectedSeries = chartViewStacked.series(i);
                if (chartViewSelector.state == "")
                    chartViewSelector.state = "highlighted";
                else
                    chartViewSelector.state = "";
            }
        }
    }

    ChartViewStacked {
        id: chartViewStacked
        anchors.left: parent.left
        anchors.leftMargin: 0
        width: parent.width
        height: parent.height
        onSeriesAdded: chartViewSelector.seriesAdded(series.name, series.color);
    }

    ChartViewHighlighted {
        id: chartViewSelected
        anchors.left: chartViewStacked.right
        width: parent.width
        height: parent.height

        opacity: 0.0
        onClicked: {
            chartViewSelector.state = "";
        }
    }

    states: State {
        name: "highlighted"
        PropertyChanges {
            target: chartViewSelected
            opacity: 1.0
        }
        PropertyChanges {
            target: chartViewStacked
            anchors.leftMargin: -chartViewStacked.width
            opacity: 0.0
        }
    }

    transitions: Transition {
        PropertyAnimation {
            properties: "width, height, opacity, anchors.leftMargin"
            duration: 400
        }
    }
}
