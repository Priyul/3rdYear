function getInputVector(vectorType) {
  var vectorInput = document.getElementById(vectorType).value;
  var vector = vectorInput.split(" ").map(Number);
  return vector;
}

function getMatrix() {
  var matrixRows = document.getElementById("matrixRows").value;
  var matrixCols = document.getElementById("matrixCols").value;
  var matrixInput = document.getElementById("matrix").value;
  var matrix = matrixInput.split("\n").map(function (row) {
    return row.split(" ").map(Number);
  });
  return matrix;
}

function getVector() {
  var vectorRows = document.getElementById("vectorRows").value;
  var vectorInput = document.getElementById("vector").value;
  var vector = vectorInput.split(" ").map(Number);
  return vector;
}

function multiplyScalar() {
let xString = document.getElementById("x").value;
let a = parseFloat(document.getElementById("scalar").value);




let x = JSON.parse(xString.replace(/'/g, "\""));


let result = [];
for (let i = 0; i < x.length; i++) {
result.push(a * x[i]);
}


document.getElementById("answer").innerHTML = "Result: [" + result + "]";
} //works


function addVectors() {
  let xString = document.getElementById("x").value;
  let yString = document.getElementById("y").value;
  let isColVector = document.getElementById("colVector").checked;


  let x = JSON.parse(xString.replace(/'/g, "\""));
  let y = JSON.parse(yString.replace(/'/g, "\""));

  // Add x and y
  let result = [];
  if (x.length !== y.length) {
    document.getElementById("answer").innerHTML = "Error: Vectors must be of the same length.";
    return;
  }
  for (let i = 0; i < x.length; i++) {
    result.push(x[i] + y[i]);
  }

  // Display the result
  let answerElement = document.getElementById("answer");
  answerElement.innerHTML = "Answer: [";
  if (isColVector) {
    for (let i = 0; i < result.length; i++) {
      let p = document.createElement("p");
      p.innerText = result[i] + ",";
      answerElement.appendChild(p);
    }
    let p = document.createElement("p");
      p.innerText = "]";
      answerElement.appendChild(p);
  } else {
    answerElement.innerText = "Result: [" + result + "]";
  }
}



function parseVector(vectorString) {
  try {

    vectorString = vectorString.replace(/\s/g, "").replace(/\[|\]/g, "");


    let vector = vectorString.split(",").map(parseFloat);


    if (vector.some(isNaN)) {
      throw new Error("Invalid input.");
    }

    return vector;
  } catch (error) {
    document.getElementById("answer").innerHTML = "Error: Please input a vector in the form [x1,x2,...,xn].";
    throw error;
  }
}


function subtractVectors() {

  let xString = document.getElementById("x").value;
  let yString = document.getElementById("y").value;
  let isColumnVector = document.getElementById("colVector").checked;


  let x = JSON.parse(xString.replace(/'/g, "\""));
  let y = JSON.parse(yString.replace(/'/g, "\""));


  if (x.length !== y.length) {
    document.getElementById("answer").innerHTML = "Error: x and y must have the same length";
    return;
  }


  let result = [];
  for (let i = 0; i < x.length; i++) {
    result.push(x[i] - y[i]);
  }


  let resultString = "[";
  if (isColumnVector) {
    for (let i = 0; i < result.length; i++) {
      resultString += result[i] + "," + "<br>";
    }
    resultString += "]";
  } else {
    resultString = "[" + result + "]";
  }
  document.getElementById("answer").innerHTML = "Result: " + resultString;
}

function dotProduct() {
  console.log("hello");
  let xString = document.getElementById("x").value;
  let yString = document.getElementById("y").value;
  

  let x = JSON.parse(xString.replace(/'/g, "\""));
  let y = JSON.parse(yString.replace(/'/g, "\""));

  if (x.length !== y.length) {
    document.getElementById("answer").innerHTML = "Error: x and y must have the same length";
    return;
  }
  

  let result = 0;
  for (let i = 0; i < x.length; i++) {
    result += x[i] * y[i];
  }
  

  let colVector = document.getElementById("colVector").checked;
  if (colVector) {
    let resultString = "";
    for (let i = 0; i < x.length; i++) {
      resultString += result.toFixed(2) + "<br>";
    }
    document.getElementById("answer").innerHTML = "Result:<br>[" + resultString + "]";
  } else {
    document.getElementById("answer").innerHTML = "Result: [" + result.toFixed(2) + "]";
  }
}

function norm() {
  let xString = document.getElementById("x").value;
  let x = JSON.parse(xString.replace(/'/g, "\""));
  

  let norm = 0;
  for (let i = 0; i < x.length; i++) {
    norm += x[i] * x[i];
  }
  norm = Math.sqrt(norm);
  

  document.getElementById("answer").innerHTML = "Result: " + norm.toFixed(4);
}

function multiplyMatrix() {
  let a = parseFloat(document.getElementById("scalar").value);
  let rows = parseInt(document.getElementById("matrixRows").value);
  let cols = parseInt(document.getElementById("matrixCols").value);
  let matrixString = document.getElementById("matrix").value;


  let matrix = matrixString.split("\n").map(row => {
    return JSON.parse(row.replace(/'/g, "\""));
  });

  let result = [];
  for (let i = 0; i < rows; i++) {
    let row = [];
    for (let j = 0; j < cols; j++) {
      row.push(a * matrix[i][j]);
    }
    result.push(row);
  }


  let formattedResult = "";
  for (let i = 0; i < rows; i++) {
    formattedResult += "[";
    for (let j = 0; j < cols; j++) {
      formattedResult += result[i][j];
      if (j !== cols - 1) {
        formattedResult += ",";
      }
    }
    formattedResult += "]<br>";
  }

  document.getElementById("answer").innerHTML = "Result:<br>" + formattedResult;
}


function transposeMatrix() {
  let rows = document.getElementById("matrixRows").value;
  let cols = document.getElementById("matrixCols").value;
  let matrixString = document.getElementById("matrix").value;
  

  let matrix = JSON.parse("[" + matrixString.replace(/\n/g, ",").replace(/\[/g, "[").replace(/,\]/g, "]") + "]");
  
 
  let transpose = [];
  for (let i = 0; i < cols; i++) {
    transpose[i] = [];
    for (let j = 0; j < rows; j++) {
      transpose[i][j] = matrix[j][i];
    }
  }
  
  let formattedTranspose = "";
  for (let i = 0; i < cols; i++) {
    formattedTranspose += "[";
    for (let j = 0; j < rows; j++) {
      formattedTranspose += transpose[i][j];
      if (j < rows - 1) {
        formattedTranspose += ",";
      }
    }
    formattedTranspose += "]<br>";
  }
  

  document.getElementById("answer").innerHTML = "Transpose: <br>" + formattedTranspose;
}



function matrixTimesVector() {
  let Mstring = document.getElementById("matrix").value;
  let Mrows = parseInt(document.getElementById("matrixRows").value);
  let Mcols = parseInt(document.getElementById("matrixCols").value);
  let M = parseMatrix(Mstring, Mrows, Mcols);

  let xString = document.getElementById("x").value;
  let x = JSON.parse(xString.replace(/'/g, "\""));

  if (x.length !== Mcols) {
    document.getElementById("answer").innerHTML = "Error: Invalid dimensions for matrix multiplication";
    return;
  }

  let result = [];
  for (let i = 0; i < Mrows; i++) {
    let rowSum = 0;
    for (let j = 0; j < Mcols; j++) {
      rowSum += M[i][j] * x[j];
    }
    result.push(rowSum);
  }

  if (document.getElementById("colVector").checked) {
    let resultStr = "";
    for (let i = 0; i < result.length; i++) {
      resultStr += "[" + result[i] + "]" + "<br>";
    }
    document.getElementById("answer").innerHTML = "Result: <br>" + resultStr;
  } else {
    document.getElementById("answer").innerHTML = "Result: [" + result + "]";
  }
}

function parseMatrix(matrixString, rows, cols) {
  let matrix = [];
  let rowStrings = matrixString.trim().split("\n");

  if (rowStrings.length !== rows) {
    return null;
  }

  for (let i = 0; i < rows; i++) {
    let row = rowStrings[i].replace(/\[|\]/g, "").split(",").map(Number);

    if (row.length !== cols) {
      return null;
    }

    matrix.push(row);
  }

  return matrix;
}


function determinant() {
  let rows = parseInt(document.getElementById("matrixRows").value);
  let cols = parseInt(document.getElementById("matrixCols").value);
  let matrixString = document.getElementById("matrix").value;
  
  // Parse matrixString to a 2D array of numbers
  let matrix = [];
  let rowsArr = matrixString.split("\n");
  for (let i = 0; i < rowsArr.length; i++) {
    matrix[i] = rowsArr[i].replace("[", "").replace("]", "").split(",");
    for (let j = 0; j < matrix[i].length; j++) {
      matrix[i][j] = parseFloat(matrix[i][j]);
    }
  }
  
  // Check if matrix is square
  if (rows !== cols) {
    document.getElementById("answer").innerHTML = "Error: Determinant is only defined for square matrices.";
    return;
  }
  
  // Recursive function to compute determinant
  function determinantRec(matrix) {
    if (matrix.length === 2) {
      return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    } else {
      let det = 0;
      for (let i = 0; i < matrix.length; i++) {
        let minor = [];
        for (let j = 1; j < matrix.length; j++) {
          minor[j-1] = matrix[j].slice(0, i).concat(matrix[j].slice(i+1));
        }
        let sign = (i % 2 === 0) ? 1 : -1;
        det += sign * matrix[0][i] * determinantRec(minor);
      }
      return det;
    }
  }
  
  // Compute determinant and display the result
  let det = determinantRec(matrix);
  document.getElementById("answer").innerHTML = "Determinant: " + det;
}


function determinantHelper(matrix) {
  let rows = matrix.length;
  let cols = matrix[0].length;

  // Check that the matrix is square
  if (rows !== cols) {
    throw new Error("Matrix must be square");
  }

  // Compute the determinant
  let det = 0;
  if (rows === 1) {
    det = matrix[0][0];
  } else if (rows === 2) {
    det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
  } else {
    for (let i = 0; i < cols; i++) {
      let minor = [];
      for (let j = 1; j < rows; j++) {
        minor.push(matrix[j].filter((_, index) => index !== i));
      }
      det += matrix[0][i] * Math.pow(-1, i) * determinantHelper(minor);
    }
  }

  return det;
}


function multiplyMatrices() {
  const matrixTextArea = document.getElementById("matrix");
  const matrixRows = parseInt(document.getElementById("matrixRows").value);
  const matrixCols = parseInt(document.getElementById("matrixCols").value);
  const matrixGTextArea = document.getElementById("matrixG");
  const matrixGRows = parseInt(document.getElementById("matrixRowsG").value);
  const matrixGCols = parseInt(document.getElementById("matrixColsG").value);
  const matrix = matrixTextArea.value.split("\n");
  const matrixG = matrixGTextArea.value.split("\n");
  const result = [];

  // Convert the matrix strings to nested arrays
  for (let i = 0; i < matrix.length; i++) {
    matrix[i] = matrix[i].replace(/[\[\]]/g, "").split(",");
    for (let j = 0; j < matrix[i].length; j++) {
      matrix[i][j] = parseFloat(matrix[i][j]);
    }
  }

  for (let i = 0; i < matrixG.length; i++) {
    matrixG[i] = matrixG[i].replace(/[\[\]]/g, "").split(",");
    for (let j = 0; j < matrixG[i].length; j++) {
      matrixG[i][j] = parseFloat(matrixG[i][j]);
    }
  }


  if (matrixCols !== matrixGRows) {
    document.getElementById("answer").innerHTML =
      "Error: Invalid dimensions for matrix multiplication";
    return;
  }


  for (let i = 0; i < matrixRows; i++) {
    result.push([]);
    for (let j = 0; j < matrixGCols; j++) {
      let sum = 0;
      for (let k = 0; k < matrixCols; k++) {
        sum += matrix[i][k] * matrixG[k][j];
      }
      result[i][j] = sum;
    }
  }


  let resultString = "";
  for (let i = 0; i < matrixRows; i++) {
    resultString += "[" + result[i].join(",") + "]";
    if (i < matrixRows - 1) {
      resultString += "<br>";
    }
  }
  document.getElementById("answer").innerHTML = resultString;
}


function solveMxV() {
  const matrixTextArea = document.getElementById("matrix");
  const vectorInput = document.getElementById("vector");
  const answerP = document.getElementById("answer");
  
  const matrixStr = matrixTextArea.value.trim();
  const vectorStr = vectorInput.value.trim();

  if (matrixStr === "" || vectorStr === "") {
    answerP.innerHTML = "Please enter both the matrix and the vector.";
    return;
  }

  const matrixRows = matrixStr.split("\n").map(rowStr => rowStr.trim().slice(1, -1).split(",").map(Number));
  const vector = vectorStr.slice(1, -1).split(",").map(Number);

  if (matrixRows.length !== vector.length) {
    answerP.innerHTML = "Error: The dimensions of the matrix and vector are not valid for matrix multiplication.";
    return;
  }

  const matrixCols = matrixRows[0].length;
  const vectorCols = 1;

  for (let i = 1; i < matrixRows.length; i++) {
    if (matrixRows[i].length !== matrixCols) {
      answerP.innerHTML = "Error: The matrix must have the same number of columns for each row.";
      return;
    }
  }

  const matrix = matrixRows.flat();
  const result = [];

  for (let i = 0; i < matrix.length; i += matrixCols) {
    const row = matrix.slice(i, i + matrixCols);
    let sum = 0;
    for (let j = 0; j < matrixCols; j++) {
      sum += row[j] * vector[j];
    }
    result.push(sum);
  }

  answerP.innerHTML = `The solution for Mx = V is: [${result.join(", ")}]`;
}

