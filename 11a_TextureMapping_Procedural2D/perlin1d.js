var canvas;
var ctx;
var xSpacing = 20;
var xFactor;
var usePerlin = true;
var scroll = false;
var id;
var points = [];
var xOffset = 0;
var xRate = 0.01;

function updateScale() {
  var xFactorSlider = document.getElementById('xFactor');

  xFactor = xFactorSlider.value;
}

window.onload = function() {
  canvas = document.getElementById("surface");
  ctx = surface.getContext("2d");

  var xFactorSlider = document.getElementById('xFactor');
  xFactorSlider.oninput = function() {
    updateScale();
    redraw();
  };

  var perlinCheckbox = document.getElementById('perlinNoise');
  perlinCheckbox.onchange = function() {
    usePerlin = perlinCheckbox.checked;
    redraw();
  }

  var scrollCheckbox = document.getElementById('scroll');
  scrollCheckbox.onchange = function() {
    scroll = scrollCheckbox.checked;
    if (!scroll) {
      clearInterval(id);
    } else {
      id = setInterval(redraw, 10);
    }
  }

  updateScale();
  redraw();
};

function drawCircle(ctx, centreX, centreY, radius, colour) {
  ctx.beginPath();
  ctx.arc(centreX, centreY, radius, 0, 2 * Math.PI, false);
  ctx.strokeStyle = colour;
  ctx.stroke();
}

function redraw() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  var firstPoint = true;
  points = [];

  ctx.beginPath();
  for (var x = 0; x < canvas.width; x += xSpacing) {
    if (usePerlin) {
      var y = (noise.perlin2(x / xFactor + xOffset, 0.5) + 1) / 2; // scale between 0 and 1
    } else {
      var y = Math.random();
    }
    y *= canvas.height;

    var currentPoint = {x: x, y: y};
    points.push(currentPoint);

    var lineColour = '#A00000';

    if (firstPoint) {
      firstPoint = false;
      ctx.moveTo(x, y);
    } else {
      ctx.lineTo(x, y);
      ctx.strokeStyle = lineColour;
      ctx.stroke();
    }
  }

  var radius = 5;
  var pointColour = '#0000A0';
  for (var i = 0; i < points.length; i++) {
    drawCircle(ctx, points[i].x, points[i].y, radius, pointColour);
  }

  xOffset += xRate;
}
