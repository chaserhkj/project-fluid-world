TEMPLATE = subdirs

SUBDIRS = frontend \
          main

CONFIG += ordered
          
frontend.file = src/frontend/frontend.pro

main.file = src/main.pro
main.depends = frontend