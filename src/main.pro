include(project.pri)
TEMPLATE = app
CONFIG += qt link_prl
TARGET = project_fluid_world
DESTDIR = ../bin
INCLUDEPATH += .
LIBS += -L"../lib" -lfrontend
SOURCES = main.cpp