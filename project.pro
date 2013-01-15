TEMPLATE = subdirs

SUBDIRS = frontend \
          sudoku \
          sudokugui \
          main \
          2D

2D.file = src/2D/2D.pro
          
frontend.file = src/frontend/frontend.pro
frontend.depends = sudokugui 2D

sudoku.file = src/sudoku/sudoku.pro

sudokugui.file = src/sudokugui/sudokugui.pro
sudokugui.depends = sudoku

main.file = src/main.pro
main.depends = frontend