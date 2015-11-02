%module modular_tetris

%inline %{

#ifdef __cplusplus
extern "C"
{
#endif

#include "model.h"
#include "board.h"
#include "boilerplate.h"

enum Color getColorValue(enum Color **color, int col, int row) {
    return color[col][row];
}
#ifdef __cplusplus
}
#endif
%}

#ifdef SWIGJAVA
%include "enumtypesafe.swg"
%javaconst(1);
%rename Board BoardSwigWrap;
%rename Color ColorSwigWrap;
#endif

%include "../tetris/model.h"
%include "../tetris/board.h"
extern void free_board(struct Board *board);