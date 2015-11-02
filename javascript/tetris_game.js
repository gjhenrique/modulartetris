var modular_tetris = require('./build/Release/modular-tetris');

var board = modular_tetris.create_board_file(10, 10, "../default_blocks");

for(var i = 0; i < 30; i++) {
    modular_tetris.next_move(board);
}

for(var i = 0; i < board.height; i++) {
    for (var j = 0; j < board.width; j++) {
        var color = modular_tetris.getColorValue(board.visited, i, j);
        process.stdout.write(color + "\t");
    }
    process.stdout.write("\n");
}
