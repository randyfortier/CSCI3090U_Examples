var canvas;
var ctx;
var xFactor, yFactor;
var usePerlin = true;
var zOffset = 0;
var zChange = 0.01;

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
  id = setInterval(redraw, 10);
};

function redraw() {
  var image = ctx.createImageData(canvas.width, canvas.height);
  var data = image.data;

/*
  var value = (noise.perlin3(x / xFactor, y / yFactor) + 1, zOffset) / 2; // scale between 0 and 1
  console.log('perlin3(0.3, 0.3, 0.3): ' + noise.perlin3(0.3, 0.3, 0.3));
  console.log('perlin3(0.29, 0.3, 0.3): ' + noise.perlin3(0.29, 0.3, 0.3));
  console.log('perlin3(0.3, 0.29, 0.3): ' + noise.perlin3(0.3, 0.29, 0.3));
  console.log('perlin3(0.3, 0.3, 0.29): ' + noise.perlin3(0.3, 0.3, 0.29));
  console.log('perlin3(0.31, 0.3, 0.3): ' + noise.perlin3(0.31, 0.3, 0.3));
  console.log('perlin3(0.3, 0.31, 0.3): ' + noise.perlin3(0.3, 0.31, 0.3));
  console.log('perlin3(0.3, 0.3, 0.31): ' + noise.perlin3(0.3, 0.3, 0.31));
*/
  var offset = 1;
  for (var x = 0; x < canvas.width; x += offset) {
    for (var y = 0; y < canvas.height; y += offset) {
      if (usePerlin) {
        var value = (noise.perlin3(x / xFactor, y / yFactor, zOffset) + 1) / 2; // scale between 0 and 1
      } else {
        var value = Math.random();
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

  zOffset += zChange;
}
