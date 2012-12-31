include(../demo.pro)
HEADERS = $$join(HEADERS, " ../" , ../)
SOURCES = $$join(SOURCES, " ../" , ../)
TARGET = demo
QMAKE_LFLAGS += -static-libgcc -static-libstdc++
CONFIG -= debug debug_and_release
CONFIG += release window
