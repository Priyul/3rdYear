const canvas = document.getElementById('webglCanvas');
const gl = canvas.getContext('webgl2');

if (!gl) {
  alert('WebGL 2 is not supported by your browser!');
}
const baseVertices = [
  -0.5, -0.5, // bottom left
  0.5, -0.5, // bottom right
  0.5, 0.3, // top right
  -0.5, 0.3 // top left
];

const roofVertices = [
  -0.5, 0.3, // bottom left
  0.5, 0.3, // bottom right
  0.0, 0.7 // top
];

const doorVertices = [
  -0.15, -0.5, // bottom left
  0.15, -0.5, // bottom right
  0.15, 0.0, // top right
  -0.15, 0.0 // top left
];

const baseColor = [1.0, 0.0, 0.0, 1.0]; // Red
const roofColor = [0.0, 0.0, 1.0, 1.0]; // Blue
const doorColor = [0.0, 1.0, 0.0, 1.0]; // Green


function createShader(gl, type, source) {
  const shader = gl.createShader(type);
  gl.shaderSource(shader, source);
  gl.compileShader(shader);
  return shader;
}

const vertexShaderSource = `
  attribute vec2 a_position;
  void main() {
    gl_Position = vec4(a_position, 0.0, 1.0);
  }
`;

const fragmentShaderSource = `
  precision mediump float;
  uniform vec4 u_color;
  void main() {
    gl_FragColor = u_color;
  }
`;

const vertexShader = createShader(gl, gl.VERTEX_SHADER, vertexShaderSource);
const fragmentShader = createShader(gl, gl.FRAGMENT_SHADER, fragmentShaderSource);

const program = gl.createProgram();
gl.attachShader(program, vertexShader);
gl.attachShader(program, fragmentShader);
gl.linkProgram(program);

const baseBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, baseBuffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(baseVertices), gl.STATIC_DRAW);

const roofBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, roofBuffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(roofVertices), gl.STATIC_DRAW);

const doorBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, doorBuffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(doorVertices), gl.STATIC_DRAW);

function drawPart(gl, buffer, vertices, color) {
  const positionLocation = gl.getAttribLocation(program, 'a_position');
  const colorLocation = gl.getUniformLocation(program, 'u_color');

  gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
  gl.enableVertexAttribArray(positionLocation);
  gl.vertexAttribPointer(positionLocation, 2, gl.FLOAT, false, 0, 0);

  gl.uniform4fv(colorLocation, color);

  gl.drawArrays(gl.TRIANGLE_FAN, 0, vertices.length / 2);
}

gl.clearColor(0.8, 0.9, 1.0, 1.0); // Set a background color (sky blue)
gl.clear(gl.COLOR_BUFFER_BIT);

gl.viewport(0, 0, canvas.width, canvas.height);

gl.useProgram(program);

drawPart(gl, baseBuffer, baseVertices, baseColor); // Draw base
drawPart(gl, roofBuffer, roofVertices, roofColor); // Draw roof
drawPart(gl, doorBuffer, doorVertices, doorColor); // Draw door


