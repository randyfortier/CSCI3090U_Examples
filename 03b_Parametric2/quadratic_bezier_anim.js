var p0 = {x: 50, y: 350};
var p1 = {x: 300, y: 50};
var p2 = {x: 550, y: 350};
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
    if (t > 1.0) {
        clearInterval(id);
    } else {
      t += deltaT;
      redraw();
    }
}

function redraw() {
    ctx.lineWidth = 4;
    // draw the lines between the control points
    drawLine(p0, p1, '#000000');
    drawLine(p1, p2, '#000000');

    // draw the gray in-between lines
    ctx.lineWidth = 1;
    for (var currentT = deltaT; currentT < t; currentT += deltaT) {
      // find the points on the lines between the control points
      var i01 = interpolatePoint(vSub(p1, p0), p0, currentT);
      var i12 = interpolatePoint(vSub(p2, p1), p1, currentT);
      drawLine(i01, i12, '#5050A0');
    }

    // draw the most recent line
    var c1 = interpolatePoint(vSub(p1, p0), p0, t);
    var c2 = interpolatePoint(vSub(p2, p1), p1, t);
    drawLine(c1, c2, '#0000FF');

    // draw all of the previous curve points
    for (var i = 0; i < curvePoints.length; i++) {
      drawPoint(curvePoints[i], '#A05050');
    }

    // draw the curve point for this line
    var curvePoint = interpolatePoint(vSub(c2, c1), c1, t);
    drawPoint(curvePoint, '#FF0000');

    // add the current curve point to the list
    curvePoints.push(curvePoint);
}

window.onload = function() {
  surface = document.getElementById("surface");
  ctx = surface.getContext("2d");

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
