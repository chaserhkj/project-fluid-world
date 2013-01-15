include(../project.pri)
TEMPLATE = lib
CONFIG += staticlib
TARGET = FW2D
DESTDIR = ../../lib
INCLUDEPATH = ../common
QMAKE_CXXFLAGS += -fopenmp
SOURCES = cylinder.cpp \
          cylinderProject.cpp \
          cylinderSpotStain.cpp \
          cylinderDataVariant.cpp
HEADERS = cylinder.h \
          commontypes.h \
          solver.h
