var canvas;
var ctx;
var xFactor, yFactor;
var usePerlin = true;

function updateScale() {
  var xFactorSlider = document.getElementById('xFactor');
  var yFactorSlider = document.getElementById('yFactor');

  xFactor = xFactorSlider.value;
  yFactor = yFactorSlider.value;
}

window.onload = function() {
  canvas = document.getElementById("surface");
  ctx = surface.getContext("2d");

  var xFactorSlider = document.getElementById('xFactor');
  xFactorSlider.oninput = function() {
    updateScale();
    redraw();
  };

  var yFactorSlider = document.getElementById('yFactor');
  yFactorSlider.oninput = function() {
    updateScale();
    redraw();
  };

  var perlinCheckbox = document.getElementById('perlinNoise');
  perlinCheckbox.onchange = function() {
    usePerlin = perlinCheckbox.checked;
    redraw();
  }

  updateScale();
  redraw();
};

function redraw() {
  var image = ctx.createImageData(canvas.width, canvas.height);
  var data = image.data;

  for (var x = 0; x < canvas.width; x++) {
    for (var y = 0; y < canvas.height; y++) {
      if (usePerlin) {
        var value = (noise.perlin2(x / xFactor, y / yFactor) + 1) / 2; // scale between 0 and 1
      } else {
        var value = (Math.random() * 2) - 1;
      }
      value *= 256;

      var cell = (x + y * canvas.width) * 4;
      data[cell + 0] = value;
      data[cell + 1] = value;
      data[cell + 2] = value;
      data[cell + 3] = 255;
    }
  }

  ctx.putImageData(image, 0, 0);
}
