include(../project.pri)
TEMPLATE = lib
CONFIG += staticlib create_prl link_prl qt
TARGET = FWsudokuGui
DESTDIR = ../../lib
INCLUDEPATH += ../../include
LIBS += -L"../../lib" -lFWsudoku
SOURCES = sudokuwidget.cpp \
          sudokumodel.cpp \
          combodelegate.cpp
HEADERS = sudokuwidget.h \
          sudokumodel.h \
          combodelegate.h
show_about {
    DEFINES += SUDOKU_SHOW_ABOUT
}