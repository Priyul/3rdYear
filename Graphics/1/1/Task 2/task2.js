function dotProduct(a, b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

function parseVector(input) {
    return input.split(',').map(parseFloat);
}

document.getElementById('input-form').addEventListener('submit', (event) => {
    event.preventDefault();

    const p = parseVector(document.getElementById('p').value);
    const d = parseVector(document.getElementById('d').value);
    const u = parseVector(document.getElementById('u').value);
    const e = parseVector(document.getElementById('e').value);
    const configuration = document.getElementById('configuration').value;

    const pe = [e[0] - p[0], e[1] - p[1], e[2] - p[2]];

    const dotProductPE = dotProduct(pe, d);
    let dotProductPU = dotProduct(pe, u);

    if (configuration === 'left-handed') {
        dotProductPU = -dotProductPU;
    }

    let result = '';

    if (dotProductPE > 0) {
        result += 'enemy is in front of p; ';
    } else {
        result += 'enemy is behind p; ';
    }

    if (dotProductPU > 0) {
        result += 'enemy is to the right of p.';
    } else {
        result += 'enemy is to the left of p.';
    }

    document.getElementById('result').innerText = result;
});
