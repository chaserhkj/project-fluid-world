include(../project.pri)
TEMPLATE = lib
CONFIG += qt staticlib create_prl
QT += opengl
TARGET = FWfrontend
DESTDIR = ../../lib
INCLUDEPATH += ../common
SOURCES = displaywidget.cpp \
          projectmainwindow.cpp \
          controlwidget.cpp \
          inputwidget.cpp
HEADERS = displaywidget.h \
          projectmainwindow.h \
          controlwidget.h \
          inputwidget.h
sudoku {
    CONFIG += link_prl
    DEFINES += SUDOKU_ENABLED
    INCLUDEPATH += ../../include
    LIBS += -L"../../lib" -lFWsudokuGui
}
