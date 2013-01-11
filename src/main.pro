include(project.pri)
TEMPLATE = app
CONFIG += qt link_prl
TARGET = project_fluid_world
DESTDIR = ../bin
INCLUDEPATH += ../include
LIBS += -L"../lib" -lFWfrontend
SOURCES = main.cpp
INCLUDEPATH += common
