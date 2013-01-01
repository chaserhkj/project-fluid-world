include(../project.pri)
TEMPLATE = lib
CONFIG += qt staticlib create_prl
QT += opengl
TARGET = FWfrontend
DESTDIR = ../../lib
SOURCES = displaywidget.cpp \
          projectmainwindow.cpp
HEADERS = displaywidget.h \
          projectmainwindow.h
