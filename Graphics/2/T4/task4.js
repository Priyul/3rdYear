const canvas = document.getElementById('webglCanvas');
const gl = canvas.getContext('webgl2');

if (!gl) {
    console.error('WebGL 2 is not supported by your browser!');
    document.body.innerHTML = 'This example requires WebGL 2. Please try a different browser.';
  }
  
const leftWindowColor = [0.0, 0.0, 1.0, 1.0]; // Blue
const rightWindowColor = [0.0, 0.0, 1.0, 1.0]; // Blue
const leftWallColor = [1.0, 1.0, 0.0, 1.0]; // Yellow
const rightWallColor = [0.0, 1.0, 1.0, 1.0]; // Cyan
const frontWallColor = [1.0, 0.0, 1.0, 1.0]; // Magenta
const backWallColor = [1.0, 0.0, 0.0, 1.0]; // Red

  

const frontWallVertices = [
  -0.5, -0.5, 0.0,
  0.5, -0.5, 0.0,
  0.5, 0.3, 0.0,
  -0.5, 0.3, 0.0
];


const roof1Vertices = [ //front
  -0.5, 0.3, 0.0,
  0.5, 0.3, 0.0,
  0.0, 0.7, -0.5
];


const backWallVertices = [
  -0.5, -0.5, -1.0,
  0.5, -0.5, -1.0,
  0.5, 0.3, -1.0,
  -0.5, 0.3, -1.0
];


const roof4Vertices = [ //back
  -0.5, 0.3, -1.0,
  0.5, 0.3, -1.0,
  0.0, 0.7, -0.5
];

const leftWallVertices = [
  -0.5, -0.5, 0.0,
  -0.5, 0.3, 0.0,
  -0.5, 0.3, -1.0,
  -0.5, -0.5, -1.0
];

const leftWindowVertices = [
  -0.5-0.01, -0.1-0.01, -0.6-0.01,
  -0.5-0.01, -0.1-0.01, -0.4-0.01,
  -0.5-0.01, 0.05-0.01, -0.4-0.01,
  -0.5-0.01, 0.05-0.01, -0.6-0.01
];


const roof2Vertices = [ //left
-0.5, 0.3, 0.0,
-0.5, 0.3, -1.0, // updated top vertex to align with left wall
0.0, 0.7, -0.5 // updated top vertex to be above left wall vertices
];

const rightWallVertices = [
  0.5, -0.5, 0.0,
  0.5, 0.3, 0.0,
  0.5, 0.3, -1.0,
  0.5, -0.5, -1.0
];


const rightWindowVertices = [
  0.5+0.01, -0.1+0.01, -0.6+0.01,
  0.5+0.01, -0.1+0.01, -0.45+0.01,
  0.5+0.01, 0.05+0.01, -0.45+0.01,
  0.5+0.01, 0.05+0.01, -0.6+0.01
];

const roof3Vertices = [ //right
0.5, 0.3, 0.0,
0.5, 0.3, -1.0, // updated top vertex to align with right wall
0.0, 0.7, -0.5 // updated top vertex to be above right wall vertices
];

  
  const doorVertices = [
    -0.15, -0.5, 0.01,
    0.15, -0.5, 0.01,
    0.15, 0.0, 0.01,
    -0.15, 0.0, 0.01
  ];

  const baseVertices = [
    -0.5, -0.5, 0.0,
    0.5, -0.5, 0.0,
    0.5, -0.5, -1.0,
    -0.5, -0.5, -1.0
  ];


  
  const baseColor = [0.0, 0.0, 1.0, 1.0]; // BLUE
  const roof1Color = [0.0, 0.0, 1.0, 1.0]; // front - BLUE
  const roof2Color = [0.0, 1.0, 0.0, 1.0]; // left - green
  const roof3Color = [0.0, 1.0, 0.0, 1.0]; // right - green
  const roof4Color = [0.0, 0.0, 1.0, 1.0]; // back - blue
  const doorColor = [0.0, 1.0, 0.0, 1.0]; // Green

  const vertexShaderSource = `
  attribute vec4 a_position;
  uniform vec4 u_color;
  uniform mat4 u_mvpMatrix;
  varying vec4 v_color;
  void main() {
    gl_Position = u_mvpMatrix * a_position;
    v_color = u_color;
  }
`;


const fragmentShaderSource = `
  precision mediump float;
  varying vec4 v_color;
  void main() {
    gl_FragColor = v_color;
  }
`;

const vertexShader = createShader(gl, gl.VERTEX_SHADER, vertexShaderSource);
const fragmentShader = createShader(gl, gl.FRAGMENT_SHADER, fragmentShaderSource);



  function createVAO(gl, buffer, program) {
    const vao = gl.createVertexArray();
    gl.bindVertexArray(vao);
  
    const positionLocation = gl.getAttribLocation(program, 'a_position');
    gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
    gl.enableVertexAttribArray(positionLocation);
    gl.vertexAttribPointer(positionLocation, 3, gl.FLOAT, false, 0, 0);
  
    gl.bindVertexArray(null);
    return vao;
  }

  const program = gl.createProgram();
  gl.attachShader(program, vertexShader);
  gl.attachShader(program, fragmentShader);
  gl.linkProgram(program);

function createShader(gl, type, source) {
  const shader = gl.createShader(type);
  gl.shaderSource(shader, source);
  gl.compileShader(shader);
  return shader;
}

// const program = gl.createProgram();
gl.attachShader(program, vertexShader);
gl.attachShader(program, fragmentShader);
gl.linkProgram(program);

const leftWallBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, leftWallBuffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(leftWallVertices), gl.STATIC_DRAW);

const rightWallBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, rightWallBuffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(rightWallVertices), gl.STATIC_DRAW);

const frontWallBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, frontWallBuffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(frontWallVertices), gl.STATIC_DRAW);

const backWallBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, backWallBuffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(backWallVertices), gl.STATIC_DRAW);

const roof1Buffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, roof1Buffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(roof1Vertices), gl.STATIC_DRAW);

const roof2Buffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, roof2Buffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(roof2Vertices), gl.STATIC_DRAW);

const roof3Buffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, roof3Buffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(roof3Vertices), gl.STATIC_DRAW);

const roof4Buffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, roof4Buffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(roof4Vertices), gl.STATIC_DRAW);

const doorBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, doorBuffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(doorVertices), gl.STATIC_DRAW);

const leftWindowBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, leftWindowBuffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(leftWindowVertices), gl.STATIC_DRAW);

const rightWindowBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, rightWindowBuffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(rightWindowVertices), gl.STATIC_DRAW);

const baseBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, baseBuffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(baseVertices), gl.STATIC_DRAW);


// const leftWallVAO = createVAO(gl, leftWallBuffer, program);
// const rightWallVAO = createVAO(gl, rightWallBuffer, program);
// const frontWallVAO = createVAO(gl, frontWallBuffer, program);
// const backWallVAO = createVAO(gl, backWallBuffer, program);
// const roofVAO = createVAO(gl, roofBuffer, program);
// const doorVAO = createVAO(gl, doorBuffer, program);
// const leftWindowVAO = createVAO(gl, leftWindowBuffer, program);
// const rightWindowVAO = createVAO(gl, rightWindowBuffer, program);

function drawPart(gl, buffer, vertices, color) {
    const positionLocation = gl.getAttribLocation(program, 'a_position');
    const colorLocation = gl.getUniformLocation(program, 'u_color');
    const wireframe = document.getElementById('wireframe').checked;
  
    gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
    gl.enableVertexAttribArray(positionLocation);
    gl.vertexAttribPointer(positionLocation, 3, gl.FLOAT, false, 0, 0);
  
    gl.uniform4fv(colorLocation, color);
  
    gl.useProgram(program);
  
    if (wireframe) {
      for (let i = 0; i < vertices.length / 3 - 2; i += 1) {
        gl.drawArrays(gl.LINE_LOOP, i, 3);
      }
    } else {
      gl.drawArrays(gl.TRIANGLE_FAN, 0, vertices.length / 3);
    }
  }


  function drawScene(time) {
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    gl.viewport(0, 0, gl.drawingBufferWidth, gl.drawingBufferHeight);
  
    const aspect = canvas.width / canvas.height;
    const projectionMatrix = perspective(45, aspect, 1, 100);
    const viewMatrix = lookAt(vec3(0, 0, 5), vec3(0, 0, 0), vec3(0, 1, 0));
    const modelMatrix = mat4();
  
    const rotationAxis = document.querySelector('input[name="rotation"]:checked').value;
  
    let rotationMatrix;
    if (rotationAxis === "x") {
      rotationMatrix = rotateX(time / 5);
    } else if (rotationAxis === "y") {
      rotationMatrix = rotateY(time / 5);
    } else {
      rotationMatrix = rotateZ(time / 5);
    }
  
    const finalModelMatrix = mult(modelMatrix, rotationMatrix);
    const mvpMatrix = mult(projectionMatrix, mult(viewMatrix, finalModelMatrix));
  
    gl.uniformMatrix4fv(gl.getUniformLocation(program, 'u_mvpMatrix'), false, flatten(mvpMatrix));
  
    // Replace the gl.drawArrays calls with drawPart calls
    drawPart(gl, frontWallBuffer, frontWallVertices, frontWallColor);
    drawPart(gl, backWallBuffer, backWallVertices, backWallColor);
    drawPart(gl, leftWallBuffer, leftWallVertices, leftWallColor);
    drawPart(gl, rightWallBuffer, rightWallVertices, rightWallColor);
    drawPart(gl, roof1Buffer, roof1Vertices, roof1Color);
    drawPart(gl, roof2Buffer, roof2Vertices, roof2Color);
    drawPart(gl, roof3Buffer, roof3Vertices, roof3Color);
    drawPart(gl, roof4Buffer, roof4Vertices, roof4Color);
    drawPart(gl, doorBuffer, doorVertices, doorColor);
    drawPart(gl, leftWindowBuffer, leftWindowVertices, leftWindowColor);
    drawPart(gl, rightWindowBuffer, rightWindowVertices, rightWindowColor);
    drawPart(gl, baseBuffer, baseVertices, baseColor);
  
    requestAnimationFrame(drawScene);
  }


// Enable the depth test before clearing the buffer
gl.enable(gl.DEPTH_TEST);

// gl.clearColor(0.8, 0.9, 1.0, 1.0); // Set a background color (sky blue)
gl.clear(gl.COLOR_BUFFER_BIT);

gl.viewport(0, 0, canvas.width, canvas.height);

gl.useProgram(program);

requestAnimationFrame(drawScene);