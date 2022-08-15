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
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
QT_END_NAMESPACE

class PenTool;
class BrushTool;
class CustomSlice;

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QPieSeries;
class QPieSlice;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);

public Q_SLOTS:
    void updateChartSettings();
    void updateSerieSettings();
    void updateSliceSettings();
    void handleSliceClicked(QPieSlice *slice);
    void showFontDialog();
    void appendSlice();
    void insertSlice();
    void removeSlice();

private:
    QComboBox *m_themeComboBox;
    QCheckBox *m_aaCheckBox;
    QCheckBox *m_animationsCheckBox;
    QCheckBox *m_legendCheckBox;

    QChartView *m_chartView;
    QPieSeries *m_series;
    CustomSlice *m_slice;

    QDoubleSpinBox *m_hPosition;
    QDoubleSpinBox *m_vPosition;
    QDoubleSpinBox *m_sizeFactor;
    QDoubleSpinBox *m_startAngle;
    QDoubleSpinBox *m_endAngle;
    QDoubleSpinBox *m_holeSize;

    QLineEdit *m_sliceName;
    QDoubleSpinBox *m_sliceValue;
    QCheckBox *m_sliceLabelVisible;
    QDoubleSpinBox *m_sliceLabelArmFactor;
    QCheckBox *m_sliceExploded;
    QDoubleSpinBox *m_sliceExplodedFactor;
    QPushButton *m_brush;
    BrushTool *m_brushTool;
    QPushButton *m_pen;
    PenTool *m_penTool;
    QPushButton *m_font;
    QPushButton *m_labelBrush;
    QComboBox *m_labelPosition;
    BrushTool *m_labelBrushTool;
};

#endif // MAINWIDGET_H
