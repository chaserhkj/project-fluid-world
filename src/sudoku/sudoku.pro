include(../project.pri)
TEMPLATE = lib
CONFIG += staticlib create_prl
TARGET = FWsudoku
DESTDIR = ../../lib
SOURCES = backend.c
HEADERS = backend.h
