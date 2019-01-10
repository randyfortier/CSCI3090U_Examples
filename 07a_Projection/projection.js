var screenWidth = 400;
var screenHeight = 400;

var eulerAngles = {x: 0, y: 0, z: 0};

var transMatrix = null;
var rotMatrix = null;
var scalingMatrix = null;
var modelMatrix = null;

var mvpMatrix;

var orthographicProjection = false;

var ctx;

var polygonPoints = [
  // front
  {x:  0.0,  y:  0.5, z: 0.5, w: 1.0}, // 0
  {x: -0.5,  y:  0.0, z: 0.5, w: 1.0}, // 1
  {x: -0.5,  y: -0.5, z: 0.5, w: 1.0}, // 2
  {x:  0.5,  y: -0.5, z: 0.5, w: 1.0}, // 3
  {x:  0.5,  y:  0.0, z: 0.5, w: 1.0}, // 4
  {x:  0.0,  y:  0.5, z: 0.5, w: 1.0}, // 5 (same as 0)

  // back
  {x:  0.0,  y:  0.5, z: -0.5, w: 1.0}, // 6
  {x: -0.5,  y:  0.0, z: -0.5, w: 1.0}, // 7
  {x: -0.5,  y: -0.5, z: -0.5, w: 1.0}, // 8
  {x:  0.5,  y: -0.5, z: -0.5, w: 1.0}, // 9
  {x:  0.5,  y:  0.0, z: -0.5, w: 1.0}, // 10
  {x:  0.0,  y:  0.5, z: -0.5, w: 1.0}, // 11 (same as 6)

  // right side
  {x:  0.5,  y:  0.0, z: -0.5, w: 1.0}, // 12 (same as 10)
  {x:  0.5,  y:  0.0, z:  0.5, w: 1.0}, // 13 (same as 4)
  {x:  0.5,  y: -0.5, z:  0.5, w: 1.0}, // 14 (same as 3)
  {x:  0.5,  y: -0.5, z: -0.5, w: 1.0}, // 15 (same as 9)

  // left side
  {x: -0.5,  y: -0.5, z: -0.5, w: 1.0}, // 16 (same as 8)
  {x: -0.5,  y:  0.0, z: -0.5, w: 1.0}, // 17 (same as 7)
  {x: -0.5,  y:  0.0, z:  0.5, w: 1.0}, // 19 (same as 1)
  {x: -0.5,  y: -0.5, z:  0.5, w: 1.0}, // 18 (same as 2)
  {x: -0.5,  y: -0.5, z: -0.5, w: 1.0}  // 19 (same as 8)
];

var pointColour = '#0000FF';

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
  var screenMatrix = [
    screenWidth/2,  0.0,             0.0, (screenWidth-1)/2,
    0.0,            screenHeight/2,  0.0, (screenWidth-1)/2,
    0.0,            0.0,             1.0, 0.0,
    0.0,            0.0,             0.0, 1.0
  ];
  return mvMult(screenMatrix, pt);
  /*
  var x = ((pt.x + 1.0) / 2.0) * screenWidth;
  var y = (1.0 - ((pt.y + 1.0) / 2.0)) * screenHeight;

  return {x: x, y: y};
  */
}

function scaleMatrix(scaleFactor) {
  return [
    scaleFactor, 0.0,         0.0,         0.0,
    0.0,         scaleFactor, 0.0,         0.0,
    0.0,         0.0,         scaleFactor, 0.0,
    0.0,         0.0,         0.0,         1.0
  ];
}

function nonUniformScaleMatrix(xScale, yScale, zScale) {
  return [
    xScale, 0.0,    0.0,    0.0,
    0.0,    yScale, 0.0,    0.0,
    0.0,    0.0,    zScale, 0.0,
    0.0,    0.0,    0.0,    1.0
  ];
}

function rotationMatrix(eulerAngles) {
  var xRot = [
    1.0,                     0.0,                          0.0, 0.0,
    0.0, Math.cos(eulerAngles.x), -1 * Math.sin(eulerAngles.x), 0.0,
    0.0, Math.sin(eulerAngles.x), Math.cos(eulerAngles.x),      0.0,
    0.0,                     0.0,                          0.0, 1.0
  ];
  var yRot = [
         Math.cos(eulerAngles.y), 0.0, Math.sin(eulerAngles.y), 0.0,
                             0.0, 1.0,                     0.0, 0.0,
    -1 * Math.sin(eulerAngles.y), 0.0, Math.cos(eulerAngles.y), 0.0,
                             0.0, 0.0,                     0.0, 1.0
  ];
  var zRot = [
    Math.cos(eulerAngles.z), -1 * Math.sin(eulerAngles.z), 0.0, 0.0,
    Math.sin(eulerAngles.z), Math.cos(eulerAngles.z),      0.0, 0.0,
    0.0,                     0.0,                          1.0, 0.0,
    0.0,                     0.0,                          0.0, 1.0
  ];

  return mmMult(xRot, mmMult(yRot, zRot));
}

function translationMatrix(dx, dy, dz) {
  return [
    1.0, 0.0, 0.0, dx,
    0.0, 1.0, 0.0, dy,
    0.0, 0.0, 1.0, dz,
    0.0, 0.0, 0.0, 1.0
  ];
}

function vScale(vec, scalar) {
  return {x: vec.x * scalar, y: vec.y * scalar, z: vec.z * scalar};
}

function normalize(vec) {
  var length = Math.sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

  return {x: vec.x / length, y: vec.y / length, z: vec.z / length};
}

function vSub(vec1, vec2) {
  return {x: vec1.x - vec2.x, y: vec1.y - vec2.y, z: vec1.z - vec2.z};
}

function vDot(vec1, vec2) {
  return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

function vCross(vec1, vec2) {
  var x = vec1.y * vec2.z - vec1.z * vec2.y;
  var y = vec1.z * vec2.x - vec1.x * vec2.z;
  var z = vec1.x * vec2.y - vec1.y * vec2.x;
  return {x: x, y: y, z: z};
}

function toStr(matrix) {
  var output = '';

  output += '\t'+matrix[0];
  output += '\t'+matrix[1];
  output += '\t'+matrix[2];
  output += '\t'+matrix[3];
  output += '\n';

  output += '\t'+matrix[4];
  output += '\t'+matrix[5];
  output += '\t'+matrix[6];
  output += '\t'+matrix[7];
  output += '\n';

  output += '\t'+matrix[8];
  output += '\t'+matrix[9];
  output += '\t'+matrix[10];
  output += '\t'+matrix[11];
  output += '\n';

  output += '\t'+matrix[12];
  output += '\t'+matrix[13];
  output += '\t'+matrix[14];
  output += '\t'+matrix[15];
  output += '\n';
  return output;
}

function viewMatrix(vertFov, aspectRatio, n, f, r, l, t, b, eyePos, look, up) {
  var horizFov = aspectRatio * vertFov;

  // calculate the orthonormal basis vectors
  var w = normalize(vScale(look, -1));
  var u = vCross(up, w);
  var v = vCross(w, u);

  // scaling
  var s1 = nonUniformScaleMatrix(2.0 / (r - l), 2.0 / (t - b), 2.0 / (n - f));
  var xt = (l + r) / 2.0;
  var yt = (b + t) / -2.0;
  var zt = (n + f) / -2.0;
  var s2 = [
    1, 0, 0, xt,
    0, 1, 0, yt,
    0, 0, 1, zt,
    0, 0, 0, 1
  ];
  var s = mmMult(s1, s2);

  // orientation
  var r = [
    u.x, u.y, u.z, 0,
    v.x, v.y, v.z, 0,
    w.x, w.y, w.z, 0,
    0,   0,   0,   1
  ];

  // translation
  var t = translationMatrix(-1 * eyePos.x, -1 * eyePos.y, -1 * eyePos.z);

  return mmMult(s, mmMult(r, t));
}

function mvMult(matrix, vector) {
  var x = matrix[0] * vector.x + matrix[1] * vector.y + matrix[2] * vector.z + matrix[3] * vector.w;
  var y = matrix[4] * vector.x + matrix[5] * vector.y + matrix[6] * vector.z + matrix[7] * vector.w;
  var z = matrix[8] * vector.x + matrix[9] * vector.y + matrix[10] * vector.z + matrix[11] * vector.w;
  var w = matrix[12] * vector.x + matrix[13] * vector.y + matrix[14] * vector.z + matrix[15] * vector.w;
  var newVector = {x: x, y: y, z: z, w: w};
  return newVector;
}

function ind(x, y) {
  return y * 4 + x;
}

function mmMult(m1, m2) {
  var result = identity();
  for (var i = 0; i < 4; i++) {
    for (var j = 0; j < 4; j++) {
      result[ind(j, i)] = m1[ind(0, i)] * m2[ind(j, 0)] +
                          m1[ind(1, i)] * m2[ind(j, 1)] +
                          m1[ind(2, i)] * m2[ind(j, 2)] +
                          m1[ind(3, i)] * m2[ind(j, 3)];
    }
  }
  return result;
}

function identity() {
  return [
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
  ];
}

function pointToStr(pt) {
  return '('+pt.x+', '+pt.y+', '+pt.z+', '+pt.w+')';
}

function homogeneousDivide(pt) {
  var newVector = {x: pt.x / pt.w, y: pt.y / pt.w, z: pt.z / pt.w, w: 1};
  return newVector;
}

function applyTransform(point) {
  // step 1:  apply the model matrix to transform in 3D
  var transformedPoint = mvMult(mvpMatrix, point);

  // step 2:  homogeneous divide
  transformedPoint = homogeneousDivide(transformedPoint);

  // step 3:  apply the model to screen coordinate system conversion
  var screenCoords = toScreenCoords(transformedPoint);

  return screenCoords;
}

function redraw() {
  ctx.lineWidth = 1;
  ctx.clearRect(0, 0, 600, 400);

  for (var i = 0; i < polygonPoints.length; i++) {
    var pt = applyTransform(polygonPoints[i]);
    drawPoint(pt, pointColour);

    if (i > 0) {
      drawLine(applyTransform(polygonPoints[i-1]), pt, '#5050FF');
    }
  }
}

function updateModelMatrix() {
  modelMatrix = mmMult(transMatrix, mmMult(rotMatrix, scalingMatrix));
}

var orthographicProjection = false;

function orthoMatrix(l, r, t, b, n, f) {
  var s = nonUniformScaleMatrix(2/(r-l), -2/(t-b), 2/(n-f));
  var t = translationMatrix((l+r)/-2, (b+t)/-2, (n+f)/-2);
  return mmMult(t, s);
}

function perspectiveMatrix(n, f) {
  return [
    1, 0, 0,       0,
    0, 1, 0,       0,
    0, 0, (n+f)/n, -f,
    0, 0, 1/(10*n), 0
  ];
}

function updateMVPMatrix() {
  updateModelMatrix();

  var eyePos = {x: 0, y: 0, z: 1};
  var look = {x: 0, y: 0, z: -1};
  var up = {x: 0, y: 1, z: 0};
  var near = 0.1;
  var far = 10;
  var fov = 45;
  var top = 1;
  var bottom = -1;
  var left = -1;
  var right = 1;
  var aspectRatio = screenWidth/screenHeight;

  var view = viewMatrix(fov, aspectRatio, near, far, right, left, top, bottom, eyePos, look, up);

  if (orthographicProjection) {
    var projection = orthoMatrix(left, right, top, bottom, near, far);
    mvpMatrix = mmMult(projection, mmMult(view, modelMatrix));
  } else {
    var projection = orthoMatrix(left, right, top, bottom, near, far);
    var perspective = perspectiveMatrix(near, far);

    mvpMatrix = mmMult(projection, mmMult(perspective, mmMult(view, modelMatrix)));
  }
}

window.onload = function() {
  var surface = document.getElementById('surface');
  ctx = surface.getContext('2d');

  transMatrix = translationMatrix(0, 0, 0);
  rotMatrix = rotationMatrix({x: 0, y: 0, z: 0});
  scalingMatrix = scaleMatrix(1);

  var xTranslationSlider = document.getElementById('xTranslationSlider');
  xTranslationSlider.oninput = function() {
    updateTranslation();
    updateMVPMatrix();
    redraw();
  };

  var yTranslationSlider = document.getElementById('yTranslationSlider');
  yTranslationSlider.oninput = function() {
    updateTranslation();
    updateMVPMatrix();
    redraw();
  };

  var zTranslationSlider = document.getElementById('zTranslationSlider');
  zTranslationSlider.oninput = function() {
    updateTranslation();
    updateMVPMatrix();
    redraw();
  };

  var xRotationSlider = document.getElementById('xRotationSlider');
  xRotationSlider.oninput = function() {
    updateRotation();
    updateMVPMatrix();
    redraw();
  };

  var yRotationSlider = document.getElementById('yRotationSlider');
  yRotationSlider.oninput = function() {
    updateRotation();
    updateMVPMatrix();
    redraw();
  };

  var zRotationSlider = document.getElementById('zRotationSlider');
  zRotationSlider.oninput = function() {
    updateRotation();
    updateMVPMatrix();
    redraw();
  };

  var xScaleSlider = document.getElementById('xScaleSlider');
  xScaleSlider.oninput = function() {
    updateScale();
    updateMVPMatrix();
    redraw();
  };

  var yScaleSlider = document.getElementById('yScaleSlider');
  yScaleSlider.oninput = function() {
    updateScale();
    updateMVPMatrix();
    redraw();
  };

  var zScaleSlider = document.getElementById('zScaleSlider');
  zScaleSlider.oninput = function() {
    updateScale();
    updateMVPMatrix();
    redraw();
  };

  var ortho = document.getElementById('ortho');
  ortho.onchange = function() {
    updateOrthographic();
  };

  updateOrthographic();
  updateTranslation();
  updateRotation();
  updateScale();
  updateMVPMatrix();
  redraw();
};

function updateOrthographic() {
  var ortho = document.getElementById('ortho');
  orthographicProjection = ortho.checked;
  updateMVPMatrix();
  redraw();
}

function updateTranslation() {
  var xTranslationSlider = document.getElementById('xTranslationSlider');
  var xTranslation = xTranslationSlider.value / 50.0;

  var xTranslationField = document.getElementById('xTranslationValue');
  xTranslationField.value = '' + xTranslation;

  var yTranslationSlider = document.getElementById('yTranslationSlider');
  var yTranslation = yTranslationSlider.value / 50.0;

  var yTranslationField = document.getElementById('yTranslationValue');
  yTranslationField.value = '' + yTranslation;

  var zTranslationSlider = document.getElementById('zTranslationSlider');
  var zTranslation = zTranslationSlider.value / 10.0;

  var zTranslationField = document.getElementById('zTranslationValue');
  zTranslationField.value = '' + zTranslation;

  transMatrix = translationMatrix(xTranslation, yTranslation, zTranslation);
}

function updateRotation() {
  var xRotationSlider = document.getElementById('xRotationSlider');
  eulerAngles.x = xRotationSlider.value / 10.0;

  var xRotationValue = document.getElementById('xRotationValue');
  xRotationValue.value = '' + eulerAngles.x;

  var yRotationSlider = document.getElementById('yRotationSlider');
  eulerAngles.y = yRotationSlider.value / 10.0;

  var yRotationValue = document.getElementById('yRotationValue');
  yRotationValue.value = '' + eulerAngles.y;

  var zRotationSlider = document.getElementById('zRotationSlider');
  eulerAngles.z = zRotationSlider.value / 10.0;

  var zRotationValue = document.getElementById('zRotationValue');
  zRotationValue.value = '' + eulerAngles.z;

  rotMatrix = rotationMatrix(eulerAngles);
}

function updateScale() {
  var xScaleSlider = document.getElementById('xScaleSlider');
  var xScale = xScaleSlider.value / 10.0;

  var xScaleValue = document.getElementById('xScaleValue');
  xScaleValue.value = '' + xScale;

  var yScaleSlider = document.getElementById('yScaleSlider');
  var yScale = yScaleSlider.value / 10.0;

  var yScaleValue = document.getElementById('yScaleValue');
  yScaleValue.value = '' + yScale;

  var zScaleSlider = document.getElementById('zScaleSlider');
  var zScale = zScaleSlider.value / 10.0;

  var zScaleValue = document.getElementById('zScaleValue');
  zScaleValue.value = '' + zScale;

  scalingMatrix = nonUniformScaleMatrix(xScale, yScale, zScale);
}
