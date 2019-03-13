var radius = 100;
var n = 3;
var polygons = [];
var xOffset = 300;
var yOffset = 200;

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
  if (n >= 20) {
      clearInterval(id);
  } else {
    n++;
    redraw();
  }
}

function redraw() {
   ctx.lineWidth = 1;
   ctx.clearRect(0, 0, 600, 400);

   // create a polygon with n sides
   polygonPoints = [];
   angleStep = 6.28319 / n;

   for (i = 0; i < n; i++) {
      angle = angleStep * i;

      point = {
         x: radius * Math.cos(angle) + xOffset,
         y: radius * Math.sin(angle) + yOffset
      };

      polygonPoints.push(point);
   }

   polygons.push(polygonPoints);

   // draw the polygons
   pointColour = '#707070';
   lineColour = '#505050';
   for (poly = 3; poly <= n; poly++) {
      if (poly == n) {
         pointColour = '#0000FF';
         lineColour = '#5050FF';
      }

      var points = polygons[poly - 3];
      console.log(poly);
      console.log(polygons);
      console.log(points);
      for (i = 0; i < n; i++) {
         drawPoint(points[i], pointColour);

         if (i > 0) {
           drawLine(points[i-1], points[i], lineColour);
         }
      }
      drawLine(points[n-1], points[0], lineColour);
   }
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
