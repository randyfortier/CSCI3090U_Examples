var p0 = {x: 100, y: 350};
var d = {x: 400, y: -300};

var deltaT = 0.05;
var t = 0.0;

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

function redraw() {
  ctx.clearRect(0, 0, 600, 400);

  // draw the gray in-between lines
  var prevPt = null;
  for (var currentT = 0.0; currentT < t; currentT += deltaT) {
    // find the points on the lines between the control points
    var pt = interpolatePoint(d, p0, currentT);
    if (prevPt) {
      drawLine(prevPt, pt, '#5050A0');
    }

    drawPoint(pt, '#A05050');

    prevPt = pt;
  }

  // draw the most recent line
  var newPt = interpolatePoint(d, p0, t);
  drawLine(prevPt, newPt, '#0000FF');

  // draw the curve point for this line
  drawPoint(newPt, '#FF0000');
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
