include(../project.pri)
TEMPLATE = lib
CONFIG += qt staticlib create_prl link_prl
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
    DEFINES += SUDOKU_ENABLED
    LIBS += -L"../../lib" -lFWsudokuGui
}
opengl {
    QT += opengl
    DEFINES += OPENGL_ENABLED
}