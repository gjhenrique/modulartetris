%module modular_tetris

%inline %{
#include "model.h"
#include "board.h"
#include "matrix_file.h"

enum Color getColorValue(enum Color **color, int col, int row) {
    return color[col][row];
}
%}

%include "enumtypesafe.swg"
%javaconst(1);
%rename Board BoardSwigWrap;
%rename Color ColorSwigWrap;

%include "../tetris/matrix_file.h"
%include "../tetris/model.h"
%include "../tetris/board.h"
