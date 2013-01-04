include(../project.pri)
TEMPLATE = lib
CONFIG += staticlib
TARGET = FW2D
DESTDIR = ../../lib
SOURCES = cylinder.cpp \
          cylinderProject.cpp \
          cylinderSpotStain.cpp
HEADERS = cylinder.h \
          commontypes.h \
          solver.h
