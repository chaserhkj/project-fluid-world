include(../project.pri)
TEMPLATE = lib
CONFIG += staticlib create_prl link_prl
TARGET = FW2D
DESTDIR = ../../lib
INCLUDEPATH = ../common
LIBS += -lumfpack
QMAKE_CXXFLAGS += -fopenmp
SOURCES = cylinder.cpp \
          cylinderProject.cpp \
          cylinderSpotStain.cpp \
          cylinderDataVariant.cpp
HEADERS = cylinder.h \
          commontypes.h \
          solver.h
