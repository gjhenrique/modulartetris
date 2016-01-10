//-------------------------------------------------------------------------
// base helper methods
//-------------------------------------------------------------------------

function get(id) {
  return document.getElementById(id);
}

function hide(id) {
  get(id).style.visibility = 'hidden';
}

function show(id) {
  get(id).style.visibility = null;
}

function html(id, html) {
  get(id).innerHTML = html;
}

function timestamp() {
  return new Date().getTime();
}

function random(min, max) {
  return (min + (Math.random() * (max - min)));
}

function randomChoice(choices) {
  return choices[Math.round(random(0, choices.length - 1))];
}

if (!window.requestAnimationFrame) { // http://paulirish.com/2011/requestanimationframe-for-smart-animating/
  window.requestAnimationFrame = window.webkitRequestAnimationFrame ||
    window.mozRequestAnimationFrame ||
    window.oRequestAnimationFrame ||
    window.msRequestAnimationFrame ||
    function(callback, element) {
      window.setTimeout(callback, 1000 / 60);
    };
}

//-------------------------------------------------------------------------
// game constants
//-------------------------------------------------------------------------

var KEY = {
    ESC: 27,
    SPACE: 32,
    LEFT: 37,
    UP: 38,
    RIGHT: 39,
    DOWN: 40
  },
  DIR = {
    UP: 0,
    RIGHT: 1,
    DOWN: 2,
    LEFT: 3,
    MIN: 0,
    MAX: 3
  },
  stats = new Stats(),
  canvas = get('canvas'),
  ctx = canvas.getContext('2d'),
  ucanvas = get('upcoming'),
  uctx = ucanvas.getContext('2d'),
  speed = {
    start: 0.6,
    decrement: 0.005,
    min: 0.1
  }, // how long before piece drops by 1 row (seconds)
  nx = 10, // width of tetris court (in blocks)
  ny = 20, // height of tetris court (in blocks)
  nu = 5,
  board = new Board(nx, ny, Board.BOARD_NORMAL); // width/height of upcoming preview (in blocks)

//-------------------------------------------------------------------------
// game variables (initialized during reset)
//-------------------------------------------------------------------------

var dx, dy, // pixel size of a single tetris block
  actions, // queue of user actions (inputs)
  playing, // true|false - game is in progress
  dt, // time since starting this game
  vscore, // the currently displayed score (it catches up to score in small chunks - like a spinning slot machine)
  step; // how long before current piece drops by 1 row

function run() {
  showStats(); // initialize FPS counter
  addEvents(); // attach keydown and resize events

  var last = now = timestamp();

  function frame() {
    now = timestamp();
    update(Math.min(1, (now - last) / 1000.0)); // using requestAnimationFrame have to be able to handle large delta's caused when it 'hibernates' in a background or non-visible tab
    draw();
    stats.update();
    last = now;
    requestAnimationFrame(frame, canvas);
  }

  resize(); // setup all our sizing information
  reset(); // reset the per-game variables
  frame(); // start the first frame

}

function showStats() {
  stats.domElement.id = 'stats';
  get('menu').appendChild(stats.domElement);
}

function addEvents() {
  document.addEventListener('keydown', keydown, false);
  window.addEventListener('resize', resize, false);
}

function resize(event) {
  canvas.width = canvas.clientWidth; // set canvas logical size equal to its physical size
  canvas.height = canvas.clientHeight; // (ditto)
  ucanvas.width = ucanvas.clientWidth;
  ucanvas.height = ucanvas.clientHeight;
  dx = canvas.width / nx; // pixel size of a single tetris block
  dy = canvas.height / ny; // (ditto)
  invalidate();
  invalidateNext();
}

function keydown(ev) {
  var handled = false;
  if (playing) {
    switch (ev.keyCode) {
      case KEY.LEFT:
        actions.push(DIR.LEFT);
        handled = true;
        break;
      case KEY.RIGHT:
        actions.push(DIR.RIGHT);
        handled = true;
        break;
      case KEY.UP:
        actions.push(DIR.UP);
        handled = true;
        break;
      case KEY.DOWN:
        actions.push(DIR.DOWN);
        handled = true;
        break;
      case KEY.ESC:
        lose();
        handled = true;
        break;
    }
  } else if (ev.keyCode == KEY.SPACE) {
    play();
    handled = true;
  }
  if (handled)
    ev.preventDefault(); // prevent arrow keys from scrolling the page (supported in IE9+ and all other browsers)
}

function setRows(n) {
  step = Math.max(speed.min, speed.start - (speed.decrement * board.rows));
  invalidateRows();
}

function clearRows() {
  setRows(0);
}

function clearActions() {
  actions = [];
}

function play() {
  hide('start');
  console.log('freeing');
  board.free();
  reset();
  playing = true;
}

function lose() {
  show('start');
  setVisualScore();
  playing = false;
}

function setVisualScore(n) {
  vscore = n || board.score;
  invalidateScore();
}

function reset() {
  board = new Board(nx, ny, Board.BOARD_NORMAL); // width/height of upcoming preview (in blocks)
  dt = 0;
  clearActions();
  clearRows(0);
  setVisualScore(0);
}

function update(idt) {
  if (playing) {
    if (vscore < board.score)
      setVisualScore(vscore + 1);
    handle(actions.shift());
    dt = dt + idt;
    if (dt > step) {
      dt = dt - step;
      if (board.nextMove() === 0) {
        invalidateNext();
      }
      invalidate();
      // We don't know when to invalidate rows
      setRows(board.rows);
    }
  }
}

function handle(action) {
  switch (action) {
    case DIR.LEFT:
      board.moveLeft();
      invalidate();
      break;
    case DIR.RIGHT:
      board.moveRight();
      invalidate();
      break;
    case DIR.UP:
      board.rotate();
      invalidate();
      break;
    case DIR.DOWN:
      board.moveBottom();
      invalidateNext();
      invalidate();
      break;
  }
}

var invalid = {};

function invalidate() {
  invalid.court = true;
  if(board.isGameOver !== 0) {
    lose();
  }
}

function invalidateNext() {
  invalid.next = true;
}

function invalidateScore() {
  invalid.score = true;
}

function invalidateRows() {
  invalid.rows = true;
}

function draw() {
  ctx.save();
  ctx.lineWidth = 1;
  ctx.translate(0.5, 0.5); // for crisp 1px black lines
  drawCourt();
  drawNext();
  drawScore();
  drawRows();
  ctx.restore();
}

function drawCourt() {
  if (invalid.court) {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    drawMatrix(ctx, board.boardValues, board.height, board.width, 0);
    ctx.strokeRect(0, 0, nx * dx - 1, ny * dy - 1); // court boundary
    invalid.court = false;
  }
}

function drawNext() {
  if (invalid.next) {
    var padding = (nu - board.nextBlock.size) / 2 ; // half-arsed attempt at centering next piece display
    uctx.save();
    uctx.translate(0.5, 0.5);
    uctx.clearRect(0, 0, nu * dx, nu * dy);
    drawMatrix(uctx, board.nextBlock.values, board.nextBlock.size, board.nextBlock.size, padding);
    uctx.strokeStyle = 'black';
    uctx.strokeRect(0, 0, nu * dx - 1, nu * dy - 1);
    uctx.restore();
    invalid.next = false;
  }
}

function drawScore() {
  if (invalid.score) {
    html('score', ("00000" + Math.floor(vscore)).slice(-5));
    invalid.score = false;
  }
}

function drawRows() {
  if (invalid.rows) {
    html('rows', board.rows);
    invalid.rows = false;
  }
}

function drawMatrix(ctx, matrix, height, width,padding) {
  for (var i = 0; i < height; i++) {
    for (var j = 0; j < width; j++) {
      var colorInt = matrix[i][j];
      if (board.hasValue(colorInt)) {
        drawBlock(ctx, j + padding, i + padding, board.intToColor(colorInt));
      }
    }
  }
}

function drawBlock(ctx, x, y, color) {
  ctx.fillStyle = color;
  ctx.fillRect(x * dx, y * dy, dx, dy);
  ctx.strokeRect(x * dx, y * dy, dx, dy);
}

//-------------------------------------------------------------------------
// FINALLY, lets run the game
run();
