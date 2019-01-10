var surface;
var controlPoints = [];
var ctx;
var dragging = false;
var selectedPoint = -1;
var dragX = 0;
var dragY = 0;

window.onload = function() {
  surface = document.getElementById("surface");
  surface.onmousedown = handleMouseDown;
  surface.onmouseup = handleMouseUp;
  surface.onmousemove = handleMouseMove;
  ctx = surface.getContext("2d");
};

function drawCircle(ctx, centreX, centreY, radius, colour) {
  ctx.beginPath();
  ctx.arc(centreX, centreY, radius, 0, 2 * Math.PI, false);
  ctx.strokeStyle = colour;
  ctx.stroke();
}

function getMousePos(event) {
    var bounds = surface.getBoundingClientRect();
    return {
      x: event.clientX - bounds.left,
      y: event.clientY - bounds.top
    };
}

function redraw() {
  ctx.clearRect(0, 0, 800, 800);

  var radius = 5;
  var colour = '#A00000';
  for (var i = 0; i < controlPoints.length; i++) {
    drawCircle(ctx, controlPoints[i].x, controlPoints[i].y, radius, colour);
  }

  ctx.beginPath();
  if (controlPoints.length >= 4) {
    // draw a cubic bezier
    var colour = '#000000';
    var deltaT = 0.05;

    var firstPoint = true;
    for (var first = 0; (first + 3) < controlPoints.length; first += 3) {
      for (var t = 0.0; t < 1.001; t += deltaT) {
        var p0 = controlPoints[first];
        var p1 = controlPoints[first+1];
        var p2 = controlPoints[first+2];
        var p3 = controlPoints[first+3];
        var oneMinusT = 1.0 - t;
        x = Math.pow(oneMinusT, 3) * p0.x + 3 * t * Math.pow(oneMinusT, 2) * p1.x + 3 * Math.pow(t, 2) * oneMinusT * p2.x + Math.pow(t, 3) * p3.x;
        y = Math.pow(oneMinusT, 3) * p0.y + 3 * t * Math.pow(oneMinusT, 2) * p1.y + 3 * Math.pow(t, 2) * oneMinusT * p2.y + Math.pow(t, 3) * p3.y;

        if (firstPoint) {
          ctx.moveTo(x, y);
          firstPoint = false;
        } else {
          ctx.lineTo(x, y);
          ctx.strokeStyle = colour;
          ctx.stroke();
        }
      }
    }
  }
}

function distance(x1, y1, x2, y2) {
  var dx = x1 - x2;
  var dy = y1 - y2;
  return Math.sqrt(dx * dx + dy * dy);
}

function handleMouseDown(event) {
  var mousePos = getMousePos(event);
  for (var i = 0; i < controlPoints.length; i++) {
    if (distance(mousePos.x, mousePos.y, controlPoints[i].x, controlPoints[i].y) < 10) {
      selectedPoint = i;
      dragX = mousePos.x - controlPoints[i].x;
      dragY = mousePos.y - controlPoints[i].y;
      dragging = true;
      return;
    }
  }
  if (selectedPoint >= 0) {
    selectedPoint = -1;
  }
}
function handleMouseMove(event) {
  if (dragging) {
    var mousePos = getMousePos(event);

    if (selectedPoint >= 0) {
      controlPoints[selectedPoint].x = mousePos.x;
      controlPoints[selectedPoint].y = mousePos.y;
      redraw();
    }
  }
}

function handleMouseUp(event) {
  if (dragging) {
    dragging = false;
  } else {
    var mousePos = getMousePos(event);

    // add control points
    controlPoints.push(mousePos);

    redraw();
  }
}
