function Board(width, height, type) {
  var board = this;

  var colors = ['no-color', 'gray', 'cyan', 'blue', 'orange', 'yellow', 'green', 'purple', 'red'];

  var init = function() {
    board.boardPtr = Module.ccall('create_board_string', 'number', ['number', 'number', 'string'], [width, height, type]);

    // Exposing public APIs
    var apiMethods = [{
      name: 'move_to_bottom',
      fnName: 'moveBottom'
    }, {
      name: 'move_to_left',
      fnName: 'moveLeft'
    }, {
      name: 'move_to_right',
      fnName: 'moveRight'
    }, {
      name: 'rotate_clockwise',
      fnName: 'rotate'
    }, {
      name: 'next_move',
      returns: 'bool',
      fnName: 'nextMove'
    }, {
      name: 'free_board',
      fnName: 'free'
    }];

    for (var i = 0; i < apiMethods.length; i++) {
      (function() {
        var apiMethod = apiMethods[i];
        var returns = apiMethod.returns || null;

        var fn = Module.cwrap(apiMethod.name, returns, ['number']);
        board[apiMethod.fnName] = function() {
          var result = fn(board.boardPtr);
          heapToObject();
          return (returns) ? result : undefined;
        };
      })();
    };

    heapToObject();
  };

  var heapToObject = function() {
    board.height = Module.getValue(board.boardPtr, 'i32');
    board.width = Module.getValue(board.boardPtr + 4, 'i32');

    var nextBlockPtr = Module.getValue(board.boardPtr + 24, 'i32');
    board.nextBlock = buildBlock(nextBlockPtr);

    var boardValuesPtr = Module.getValue(board.boardPtr + 32, 'i32');
    board.boardValues = board.buildMatrix(boardValuesPtr, board.width, board.height);

    board.isGameOver = Module.getValue(board.boardPtr + 36, 'i32');
    board.score = Module.getValue(board.boardPtr + 40, 'i32');
    board.rows = Module.getValue(board.boardPtr + 44, 'i32');
  };

  var buildBlock = function(blockPtr) {
    var colSize = Module.getValue(blockPtr, 'i32');
    var rowSize = Module.getValue(blockPtr + 4, 'i32');
    var color = Module.getValue(blockPtr + 8, 'i32');
    var nextBlockPtr = Module.getValue(blockPtr + 12, 'i32');
    return {
      values: board.buildMatrix(nextBlockPtr, colSize, rowSize),
      color: color,
      // We know that all the blocks are a square matrix
      size: colSize
    };
  };

  this.intToColor = function(colorInt) {
    return colors[colorInt % 9];
  };

  this.hasValue = function(colorInt) {
    return colorInt !== 0;
  };

  init();
};

Board.prototype.buildMatrix = function(ptr, col, row) {
  var arr = new Array(row);
  for (var i = 0; i < row; i++) {
    var rowPtr = Module.getValue(ptr + i * 4, 'i32');
    arr[i] = new Array(col);
    for (var j = 0; j < col; j++) {
      arr[i][j] = Module.getValue(rowPtr + j * 4, 'i32');
    }
  }
  return arr;
};
