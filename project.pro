TEMPLATE = subdirs

SUBDIRS = frontend \
          sudoku \
          main

CONFIG += ordered
          
frontend.file = src/frontend/frontend.pro

sudoku.file = src/sudoku/sudoku.pro

main.file = src/main.pro
main.depends = frontend