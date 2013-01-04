TEMPLATE = subdirs

SUBDIRS = frontend \
          sudoku \
          sudokugui \
          main

          
frontend.file = src/frontend/frontend.pro
frontend.depends = sudokugui

sudoku.file = src/sudoku/sudoku.pro

sudokugui.file = src/sudokugui/sudokugui.pro
sudokugui.depends = sudoku

main.file = src/main.pro
main.depends = frontend