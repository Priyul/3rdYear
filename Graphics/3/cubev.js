"use strict";

var canvas;
var gl;
var program;

var numVertices  = 36;

var axis = 0;
var xAxis = 0;
var yAxis =1;
var zAxis = 2;
var theta = [ 0, 0, 0 ];
var thetaLoc;

function vec3(x, y, z) {
    return [x, y, z];
}

function vec4(x, y, z, w) {
    return [x, y, z, w];
}


    var vertices = [
        vec3( -0.5, -0.5,  0.5 ),
        vec3( -0.5,  0.5,  0.5 ),
        vec3(  0.5,  0.5,  0.5 ),
        vec3(  0.5, -0.5,  0.5 ),
        vec3( -0.5, -0.5, -0.5 ),
        vec3( -0.5,  0.5, -0.5 ),
        vec3(  0.5,  0.5, -0.5 ),
        vec3(  0.5, -0.5, -0.5 )
    ];

    var vertexColors = [
        vec4( 0.0, 0.0, 0.0, 1.0 ),  // black
        vec4( 1.0, 0.0, 0.0, 1.0 ),  // red
        vec4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
        vec4( 0.0, 1.0, 0.0, 1.0 ),  // green
        vec4( 0.0, 0.0, 1.0, 1.0 ),  // blue
        vec4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
        vec4( 1.0, 1.0, 1.0, 1.0 ),  // white
        vec4( 0.0, 1.0, 1.0, 1.0 )   // cyan
    ];

var indices = [
    1, 0, 3,
    3, 2, 1,
    2, 3, 7,
    7, 6, 2,
    3, 0, 4,
    4, 7, 3,
    6, 5, 1,
    1, 2, 6,
    4, 5, 6,
    6, 7, 4,
    5, 4, 0,
    0, 1, 5
];

function mult(a, b) {
    var result = [];
    for (var i = 0; i < a.length; i++) {
        result[i] = [];
        for (var j = 0; j < b[0].length; j++) {
            var sum = 0;
            for (var k = 0; k < a[0].length; k++) {
                sum += a[i][k] * b[k][j];
            }
            result[i][j] = sum;
        }
    }
    return result;
}


function flatten(a) {
    return a.reduce((acc, val) => acc.concat(val), []);
}

function scalem(x, y, z) {
    return [
        [x, 0, 0, 0],
        [0, y, 0, 0],
        [0, 0, z, 0],
        [0, 0, 0, 1]
    ];
}

function rotate(angle, axis) {
    var s = Math.sin(angle);
    var c = Math.cos(angle);
    if (axis[0] == 1) { // rotation around x-axis
        return [
            [1, 0, 0, 0],
            [0, c, -s, 0],
            [0, s, c, 0],
            [0, 0, 0, 1]
        ];
    } else if (axis[1] == 1) { // rotation around y-axis
        return [
            [c, 0, s, 0],
            [0, 1, 0, 0],
            [-s, 0, c, 0],
            [0, 0, 0, 1]
        ];
    } else { // rotation around z-axis
        return [
            [c, -s, 0, 0],
            [s, c, 0, 0],
            [0, 0, 1, 0],
            [0, 0, 0, 1]
        ];
    }
}

function translate(x, y, z) {
    return [
        [1, 0, 0, x],
        [0, 1, 0, y],
        [0, 0, 1, z],
        [0, 0, 0, 1]
    ];
}


var scaleMatrix, rotationMatrix, translationMatrix;
var scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0;
var transX = 0.0, transY = 0.0, transZ = 0.0;
var delta = 0.05;

window.onload = function init()
{
    var vertexShaderText = `
        attribute vec4 vPosition;
        attribute vec4 vColor;
        varying vec4 fColor;
        uniform mat4 transform;

        void main() {
            gl_Position = transform * vPosition;
            fColor = vColor;
        }
        `;

    var fragmentShaderText = `
    precision mediump float;
    varying vec4 fColor;

    void main() {
        gl_FragColor = fColor;
    }
`;


    canvas = document.getElementById( "gl-canvas" );
    gl = WebGLUtils.setupWebGL( canvas );
    if ( !gl ) { alert( "WebGL isn't available" ); }

    program = gl.createProgram();

    var vertexShader = gl.createShader(gl.VERTEX_SHADER);
    var fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(vertexShader, vertexShaderText);
    gl.shaderSource(fragmentShader, fragmentShaderText);

    gl.compileShader(vertexShader);
    gl.compileShader(fragmentShader);

    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);

    gl.linkProgram(program);

    gl.useProgram(program);

    var vertexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(flatten(vertices)), gl.STATIC_DRAW);

    var vPosition = gl.getAttribLocation(program, "vPosition");
    gl.vertexAttribPointer(vPosition, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(vPosition);

    var colorBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(flatten(vertexColors)), gl.STATIC_DRAW);

    var vColor = gl.getAttribLocation(program, "vColor");
    gl.vertexAttribPointer(vColor, 4, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(vColor);

    var indexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint8Array(indices), gl.STATIC_DRAW);

    scaleMatrix = scalem(scaleX, scaleY, scaleZ);
    rotationMatrix = mult(rotate(theta[xAxis], [1, 0, 0]), mult(rotate(theta[yAxis], [0, 1, 0]), rotate(theta[zAxis], [0, 0, 1])));
    translationMatrix = translate(transX, transY, transZ);

    window.onkeydown = function(event) {
    var key = event.key.toUpperCase();
    switch(key){
        case 'W': scaleX += delta; break; // scale cube on x-axis
        case 'S': scaleX -= delta; break; // scale cube on x-axis
        case 'A': scaleY += delta; break; // scale cube on y-axis
        case 'D': scaleY -= delta; break; // scale cube on y-axis
        case 'Q': scaleZ += delta; break; // scale cube on z-axis
        case 'E': scaleZ -= delta; break; // scale cube on z-axis

        case 'R': theta[xAxis] += delta; break; // rotate cube around x-axis
        case 'T': theta[yAxis] += delta; break; // rotate cube around y-axis
        case 'Y': theta[zAxis] += delta; break; // rotate cube around z-axis

        case '+': delta += 0.01; break; // increase delta
        case '-': delta -= 0.01; break; // decrease delta

        case ' ': // reset all transformations and deltas
            scaleX = scaleY = scaleZ = 1.0;
            transX = transY = transZ = 0.0;
            theta = [0, 0, 0];
            delta = 0.05;
            break;

        case 'ARROWUP': transY += delta; break; // translate cube on y-axis
        case 'ARROWDOWN': transY -= delta; break; // translate cube on y-axis
        case 'ARROWLEFT': transX -= delta; break; // translate cube on x-axis
        case 'ARROWRIGHT': transX += delta; break; // translate cube on x-axis
    }
    scaleMatrix = scalem(scaleX, scaleY, scaleZ);
    rotationMatrix = mult(rotate(theta[xAxis], [1, 0, 0]), mult(rotate(theta[yAxis], [0, 1, 0]), rotate(theta[zAxis], [0, 0, 1])));
    translationMatrix = translate(transX, transY, transZ);
};


    render();
};

function render()
{
    gl.clear( gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    var transformMatrix = mult(translationMatrix, mult(rotationMatrix, scaleMatrix));

    var transformLoc = gl.getUniformLocation(program, "transform");
    gl.uniformMatrix4fv(transformLoc, false, flatten(transformMatrix));

    gl.drawElements( gl.TRIANGLES, numVertices, gl.UNSIGNED_BYTE, 0 );

    requestAnimFrame( render );
}
