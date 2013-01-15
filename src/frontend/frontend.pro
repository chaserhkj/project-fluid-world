include(../project.pri)
TEMPLATE = lib
CONFIG += qt staticlib create_prl
QT += opengl
TARGET = FWfrontend
DESTDIR = ../../lib
LIBS += -L"../../lib" -lFW2D
INCLUDEPATH += ../common ../../include
SOURCES = displaywidget.cpp \
          projectmainwindow.cpp \
          controlwidget.cpp \
          calthread.cpp \
          inputwidget.cpp
HEADERS = displaywidget.h \
          projectmainwindow.h \
          controlwidget.h \
          calthread.h \
          inputwidget.h
sudoku {
    CONFIG += link_prl
    DEFINES += SUDOKU_ENABLED
    LIBS += -L"../../lib" -lFWsudokuGui
}
