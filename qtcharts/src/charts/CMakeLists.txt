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
project( QtCharts )

include(include.cmake)
include(${QT_CHARTS_DIR}/Project.cmake)

add_library(QtCharts 
	${qtproject_SRCS} 
	${qtproject_QRC_SRCS} 
    ${qtproject_MOC_SRCS}
	${qtproject_UIS_H} 
	${qtproject_H} 
	${project_H}
	) 
set_target_properties( QtCharts PROPERTIES FOLDER 3rdParty )

