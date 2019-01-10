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

function vAdd(vec1, vec2) {
  return {x: vec1.x + vec2.x, y: vec1.y + vec2.y};
}

function vSub(vec1, vec2) {
  return {x: vec1.x - vec2.x, y: vec1.y - vec2.y};
}

function drawLine(p1, p2, colour) {
  ctx.beginPath();
  ctx.moveTo(p1.x, p1.y);
  ctx.lineTo(p2.x, p2.y);
  ctx.strokeStyle = colour;
  ctx.stroke();
}

function redraw() {
  ctx.clearRect(0, 0, 800, 800);

  var radius = 5;
  var colour = '#A00000';
  for (var i = 0; i < controlPoints.length; i++) {
    drawCircle(ctx, controlPoints[i].x, controlPoints[i].y, radius, colour);
  }

  if (controlPoints.length >= 4) {

    // draw lines between each pair of control points
    for (var first = 0; (first + 1) < controlPoints.length; first += 2) {
      drawLine(controlPoints[first+0], controlPoints[first+1], '#0000A0');
    }

    ctx.beginPath();

    // draw a cubic bezier
    var colour = '#000000';
    var deltaT = 0.05;

    var firstPoint = true;
    for (var first = 0; (first + 3) < controlPoints.length; first += 2) {
      for (var t = 0.0; t < 1.001; t += deltaT) {
        var p1 = controlPoints[first];
        var v1 = vSub(controlPoints[first+1], p1);
        var p2 = controlPoints[first+2];
        var v2 = vSub(controlPoints[first+3], p2);

        var p1_coeff = 1 - 3 * Math.pow(t, 2) + 2 * Math.pow(t, 3);
        var p2_coeff = 3 * Math.pow(t, 2) - 2 * Math.pow(t, 3);
        var v1_coeff = t - 2 * Math.pow(t, 2) + Math.pow(t, 3);
        var v2_coeff = -1 * Math.pow(t, 2) + Math.pow(t, 3);

        var x = p1_coeff * p1.x + p2_coeff * p2.x + v1_coeff * v1.x + v2_coeff * v2.x;
        var y = p1_coeff * p1.y + p2_coeff * p2.y + v1_coeff * v1.y + v2_coeff * v2.y;

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

function createOpposingPoint(otherPoint, centrePoint) {
  var direction = vSub(centrePoint, otherPoint);
  return vAdd(direction, centrePoint);
}

function handleMouseUp(event) {
  if (dragging) {
    dragging = false;
  } else {
    var mousePos = getMousePos(event);

    // do we need to add an automatic control point?
    /*
    if ((controlPoints.length > 4) && ((controlPoints.length % 4) == 0)) {
      var last = controlPoints.length - 1;
      controlPoints.push(createOpposingPoint(controlPoints[last], controlPoints[last-1]));
    }
    */

    // add control points
    controlPoints.push(mousePos);

    redraw();
  }
}
