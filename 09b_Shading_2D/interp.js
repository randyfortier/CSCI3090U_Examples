var ctx;

var p1 = {x: 50, y: 50};
var p2 = {x: 450, y: 650};
var p3 = {x: 550, y: 200};
var c1 = {r: 1.0, g: 0.0, b: 0.0};
var c2 = {r: 0.0, g: 0.0, b: 1.0};
var c3 = {r: 0.0, g: 1.0, b: 0.0};

var deltaL = 0.002;

function toHex(val) {
  var intVal = Math.round(val * 255);
  var hex = intVal.toString(16);
  if (hex.length < 2) {
    hex = '0' + hex;
  }
  return hex;
}

function toRGB(colour) {
  var rs = toHex(colour.r);
  var gs = toHex(colour.g);
  var bs = toHex(colour.b);
  return '#' + rs + gs + bs;
}

function drawPoint(p, colour) {
  var radius = 1;
  var rgb = toRGB(colour);

  ctx.beginPath();
  ctx.arc(p.x, p.y, radius, 0, 2 * Math.PI, false);
  ctx.fillStyle = rgb;
  ctx.fill();
}

function vMult(vec, scalar) {
  if (vec.x) {
    return {x: scalar * vec.x, y: scalar * vec.y};
  } else {
    return {r: scalar * vec.r, g: scalar * vec.g, b: scalar * vec.b};
  }
}

function vAdd(vec1, vec2, vec3) {
  if (vec1.x) {
    return {x: vec1.x + vec2.x + vec3.x, y: vec1.y + vec2.y + vec3.y};
  } else {
    return {r: vec1.r + vec2.r + vec3.r, g: vec1.g + vec2.g + vec3.g, b: vec1.b + vec2.b + vec3.b};
  }
}

window.onload = function() {
  var surface = document.getElementById("surface");
  ctx = surface.getContext("2d");

  redraw();
};

function interp(l1, l2, l3, vec1, vec2, vec3) {
  return vAdd(vMult(vec1, l1), vMult(vec2, l2), vMult(vec3, l3));
}

function redraw() {
  ctx.clearRect(0, 0, 800, 800);

  for (var lambda1 = 0; lambda1 < 1.0; lambda1 += deltaL) {
    for (var lambda2 = 0; lambda2 < 1.0; lambda2 += deltaL) {
      if ((lambda1 + lambda2) <= 1.0) {
        var lambda3 = 1 - lambda1 - lambda2;

        var position = interp(lambda1, lambda2, lambda3, p1, p2, p3);
        var colour = interp(lambda1, lambda2, lambda3, c1, c2, c3);

        drawPoint(position, colour);
      }
    }
  }

  drawPoint(p1, c1);
  drawPoint(p2, c2);
  drawPoint(p3, c3);
}
