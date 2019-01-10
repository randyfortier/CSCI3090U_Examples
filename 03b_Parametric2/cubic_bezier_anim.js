var p0 = {x: 50, y: 50};
var p1 = {x: 200, y: 300};
var p2 = {x: 350, y: 20};
var p3 = {x: 550, y: 350};
var deltaT = 0.05;
var t = 0.0;
var curvePoints = [];

var surface;
var ctx;
var id;

function drawLine(p1, p2, colour) {
  ctx.beginPath();
  ctx.moveTo(p1.x, p1.y);
  ctx.lineTo(p2.x, p2.y);
  ctx.strokeStyle = colour;
  ctx.stroke();
}

function drawPoint(p, colour) {
  var radius = 3;

  ctx.beginPath();
  ctx.arc(p.x, p.y, radius, 0, 2 * Math.PI, false);
  ctx.fillStyle = colour;
  ctx.fill();
}

function vMult(vec, scalar) {
  return {x: scalar * vec.x, y: scalar * vec.y};
}

function vAdd(vec1, vec2) {
  return {x: vec1.x + vec2.x, y: vec1.y + vec2.y};
}

function vSub(vec1, vec2) {
  return {x: vec1.x - vec2.x, y: vec1.y - vec2.y};
}

function interpolatePoint(direction, startingPoint, amount) {
  return vAdd(startingPoint, vMult(direction, amount));
}

var animating = false;

function updateSpeed() {
  if (animating) {
    var speedSlider = document.getElementById('speed');
    var speed = 10000 / speedSlider.value;

    clearInterval(id);
    id = setInterval(drawFrame, speed);
  }
}

function stopAnimation() {
  animating = false;

  clearInterval(id);
}

function startAnimation() {
  var speedSlider = document.getElementById('speed');

  if (speedSlider.value == 0) {
    stopAnimation();
  } else {
    animating = true;
    var speed = 10000 / speedSlider.value;

    clearInterval(id);
    id = setInterval(drawFrame, speed);
  }
}

function drawFrame() {
  if (t > 0.999999) {
      clearInterval(id);
  } else {
    t += deltaT;
    redraw();
  }
}

// for testing purposes only
function calculateCubic(p0, p1, p2, p3, t) {
  var oneMinusT = 1.0 - t;
  var x = Math.pow(oneMinusT, 3) * p0.x + 3 * t * Math.pow(oneMinusT, 2) * p1.x + 3 * Math.pow(t, 2) * oneMinusT * p2.x + Math.pow(t, 3) * p3.x;
  var y = Math.pow(oneMinusT, 3) * p0.y + 3 * t * Math.pow(oneMinusT, 2) * p1.y + 3 * Math.pow(t, 2) * oneMinusT * p2.y + Math.pow(t, 3) * p3.y;
  return {x: x, y: y};
}

function redraw() {
  ctx.lineWidth = 1;
  ctx.clearRect(0, 0, 600, 400);

  // draw the most recent line
  c1 = interpolatePoint(vSub(p1, p0), p0, t);
  c2 = interpolatePoint(vSub(p2, p1), p1, t);
  c3 = interpolatePoint(vSub(p3, p2), p2, t);
  drawPoint(c1, '#0000FF');
  drawPoint(c2, '#0000FF');
  drawPoint(c3, '#0000FF');
  drawLine(c1, c2, '#5050FF');
  drawLine(c2, c3, '#5050FF');

  // draw the join line
  var j1 = interpolatePoint(vSub(c2, c1), c1, t);
  var j2 = interpolatePoint(vSub(c3, c2), c2, t);
  drawPoint(j1, '#00FF00');
  drawPoint(j2, '#00FF00');

  // find the correct point on the join line
  var curvePoint = interpolatePoint(vSub(j2, j1), j1, t);
  drawLine(j1, j2, '#A0FFA0');

  // draw all of the previous curve points
  for (var i = 0; i < curvePoints.length; i++) {
    drawPoint(curvePoints[i], '#A05050');
  }

  // draw the current curve point
  drawPoint(curvePoint, '#FF0000');

  // add the current curve point to the list
  curvePoints.push(curvePoint);

  // draw the lines between the control points
  ctx.lineWidth = 4;
  drawLine(p0, p1, '#000000');
  drawLine(p1, p2, '#000000');
  drawLine(p2, p3, '#000000');
}

window.onload = function() {
  surface = document.getElementById('surface');
  ctx = surface.getContext('2d');

  var speedSlider = document.getElementById('speed');
  speedSlider.oninput = function() {
    updateSpeed();
  };

  stopAnimation();
  var stopGoButton = document.getElementById('stopgo');
  stopGoButton.onclick = function() {
    if (stopGoButton.textContent === 'Stop') {
      stopAnimation();
      stopGoButton.textContent = 'Go';
    } else {
      startAnimation();
      stopGoButton.textContent = 'Stop';
    }
  };
};
