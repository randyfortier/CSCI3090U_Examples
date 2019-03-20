const MAX_POLYGON_SIZE = 100;

var radius = 200;
var n = 2;
var polygons = [];
var xOffset = 300;
var yOffset = 300;

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

function updateN(n) {
   var nElement = document.getElementById('n');
   nElement.value = n;
}

function updatePi(pi) {
   var piElement = document.getElementById('pi');
   piElement.value = pi;
}

function drawFrame() {
   if (n >= MAX_POLYGON_SIZE) {
      clearInterval(id);
   } else {
      n++;
      updateN(n);
      redraw();
   }
}

function drawPolygon(points, pointColour, lineColour) {
   for (i = 0; i < points.length; i++) {
      drawPoint(points[i], pointColour);

      if (i > 0) {
         drawLine(points[i - 1], points[i], lineColour);
      }
   }
   drawLine(points[points.length - 1], points[0], lineColour);
}

function lineLength(p1, p2) {
   var dx = p1.x - p2.x;
   var dy = p1.y - p2.y;

   return Math.sqrt(dx*dx + dy*dy) / (2 * radius);
}

function redraw() {
   ctx.lineWidth = 1;
   ctx.clearRect(0, 0, 600, 600);

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
   pointColour = '#e0e0e0';
   lineColour = '#e0e0e0';
   for (poly = 3; poly <= n; poly++) {
      points = polygons[poly - 3];

      if (poly == n) {
         pointColour = '#0000FF';
         lineColour = '#5050FF';

         updatePi(poly * lineLength(points[0], points[1]));
      }

      drawPolygon(points, pointColour, lineColour);
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
