!include( ../tests.pri ) {
    error( "Couldn't find the tests.pri file!" )
}

QT += testlib widgets

!contains(TARGET, ^tst_.*):TARGET = $$join(TARGET,,"tst_")

INCLUDEPATH += ../inc
HEADERS += ../inc/tst_definitions.h

#Define for unit tests
CONFIG(debug, debug|release) {
    DEFINES += BUILD_PRIVATE_UNIT_TESTS
}
