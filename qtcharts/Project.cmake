#
# (c) Copyright 2004 - 2017 Blue Pearl Software Inc.
# All rights reserved.
#
# This source code belongs to Blue Pearl Software Inc.
# It is considered trade secret and confidential, and is not to be used
# by parties who have not received written authorization
# from Blue Pearl Software Inc.
#
# Only authorized users are allowed to use, copy and modify
# this software provided that the above copyright notice
# remains in all copies of this software.
#
#
#
SET( QT_USE_QTGUI TRUE )


AddQtIncludes( Gui )
AddQtIncludes( Core )
AddQtIncludes( Widgets )
QT5_WRAP_CPP(qtproject_MOC_SRCS ${qtproject_H})
QT5_WRAP_UI(qtproject_UIS_H ${qtproject_UIS})

include(${CMAKE_SOURCE_DIR}/Project.cmake)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/UI)
include_directories(${GMOCK_SOURCE_DIR}/include)
include_directories(${GOOGLETEST_SOURCE_DIR}/include)

include_directories(${GOOGLETEST_SOURCE_DIR}/include)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(${QT_CHARTS_DIR})
include_directories(${QT_CHARTS_DIR}/QtCharts)
include_directories(${QT_CHARTS_DIR}/src/charts)
include_directories(${QT_CHARTS_DIR}/src/charts/animations)
include_directories(${QT_CHARTS_DIR}/src/charts/areachart)
include_directories(${QT_CHARTS_DIR}/src/charts/axis/valueaxis)
include_directories(${QT_CHARTS_DIR}/src/charts/axis/logvalueaxis)
include_directories(${QT_CHARTS_DIR}/src/charts/axis/barcategoryaxis)
include_directories(${QT_CHARTS_DIR}/src/charts/axis/categoryaxis)
include_directories(${QT_CHARTS_DIR}/src/charts/axis/datetimeaxis)
include_directories(${QT_CHARTS_DIR}/src/charts/axis)
include_directories(${QT_CHARTS_DIR}/src/charts/barchart)
include_directories(${QT_CHARTS_DIR}/src/charts/barchart/vertical/stacked)
include_directories(${QT_CHARTS_DIR}/src/charts/barchart/vertical/percent)
include_directories(${QT_CHARTS_DIR}/src/charts/boxplotchart)
include_directories(${QT_CHARTS_DIR}/src/charts/domain)
include_directories(${QT_CHARTS_DIR}/src/charts/layout)
include_directories(${QT_CHARTS_DIR}/src/charts/legend)
include_directories(${QT_CHARTS_DIR}/src/charts/linechart)
include_directories(${QT_CHARTS_DIR}/src/charts/piechart)
include_directories(${QT_CHARTS_DIR}/src/charts/scatterchart)
include_directories(${QT_CHARTS_DIR}/src/charts/splinechart)
include_directories(${QT_CHARTS_DIR}/src/charts/themes)
include_directories(${QT_CHARTS_DIR}/src/charts/xychart)

source_group("Generated Files" FILES ${qtproject_UIS_H} ${qtproject_QRC_SRCS} )
source_group("MOC Source Files" FILES ${qtproject_MOC_SRCS} )
source_group("Resource Files" FILES ${qtproject_QRC} ${qtproject_QRC_SOURCES} )
source_group("Designer Files" FILES ${qtproject_UIS} )
source_group("Header Files" FILES ${qtproject_H} )

add_definitions( 
	-DQT_CHARTS_LIBRARY
	-DQT_CHARTS_STATIC_LIB
)

