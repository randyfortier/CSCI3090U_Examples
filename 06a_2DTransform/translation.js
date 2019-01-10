var screenWidth = 400;
var screenHeight = 400;

var modelMatrix;

var ctx;

var polygonPoints = [
  {x: -0.5,  y:  0.0, z: 1.0},
  {x: -0.5,  y: -0.5, z: 1.0},
  {x:  0.5,  y: -0.5, z: 1.0},
  {x:  0.5,  y:  0.0, z: 1.0},
  {x:  0.0,  y:  0.5, z: 1.0},
  {x: -0.5,  y:  0.0, z: 1.0},
  {x:  0.5,  y:  0.0, z: 1.0},
];

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

function toScreenCoords(pt) {
  var x = ((pt.x + 1.0) / 2.0) * screenWidth;
  var y = ((1.0 - pt.y) / 2.0) * screenHeight;

  return {x: x, y: y};
}

function scaleMatrix(scaleFactor) {
  return [
    scaleFactor, 0.0,
    0.0, scaleFactor,
  ];
}

function rotationMatrix(rotationAngle) {
  return [
    Math.cos(rotationAngle), -1 * Math.sin(rotationAngle),
    Math.sin(rotationAngle), Math.cos(rotationAngle)
  ];
}

function translationMatrix(dx, dy) {
  return [
    1.0, 0.0, dx,
    0.0, 1.0, dy,
    0.0, 0.0, 1.0
  ];
}

function mvMult(matrix, vector) {
  // Note:  This has been updated to access homogeneous coordinates (required for translation)
  var x = matrix[0] * vector.x + matrix[1] * vector.y + matrix[2] * vector.z;
  var y = matrix[3] * vector.x + matrix[4] * vector.y + matrix[5] * vector.z;
  var z = matrix[6] * vector.x + matrix[7] * vector.y + matrix[8] * vector.z;
  return {x: x, y: y, z: z};
}

function updateTranslation() {
  var xTranslationSlider = document.getElementById('xTranslation');
  var xTranslation = xTranslationSlider.value / 50.0;

  var xTranslationField = document.getElementById('xTranslationValue');
  xTranslationField.value = '' + xTranslation;

  var yTranslationSlider = document.getElementById('yTranslation');
  var yTranslation = yTranslationSlider.value / 50.0;

  var yTranslationField = document.getElementById('yTranslationValue');
  yTranslationField.value = '' + yTranslation;

  modelMatrix = translationMatrix(xTranslation, yTranslation);

  redraw();
}

function applyTransform(point) {
  // Note:  This has been updated to access homogeneous coordinates (required for translation)

  // step 1:  apply the model matrix to transform in 2D
  var transformedPoint = mvMult(modelMatrix, point);

  // step 2:  apply the model to screen coordinate system conversion
  return toScreenCoords(transformedPoint);
}

function redraw() {
  ctx.lineWidth = 1;
  ctx.clearRect(0, 0, 600, 400);

  for (var i = 0; i < polygonPoints.length; i++) {
      drawPoint(applyTransform(polygonPoints[i]), '#0000FF');

      if (i > 0) {
        drawLine(applyTransform(polygonPoints[i-1]), applyTransform(polygonPoints[i]), '#5050FF');
      }
  }
}

window.onload = function() {
  var surface = document.getElementById('surface');
  ctx = surface.getContext('2d');

  var xTranslation = document.getElementById('xTranslation');
  xTranslation.oninput = function() {
    updateTranslation();
  };

  var yTranslation = document.getElementById('yTranslation');
  yTranslation.oninput = function() {
    updateTranslation();
  };

  updateTranslation();
};
