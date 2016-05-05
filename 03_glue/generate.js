"use strict"

var fs = require("fs");

function shred(f, input) {

    var d, N, n, t, i, p, q;

    /*
     *  n = 10; // piece length
     *  N = 600// num of pieces
     *  t = N + n - 1; // solution string length
     */

    d = input;

    if(d.slice(-1)==='\n'||d.slice(-1)==='\r') d = d.slice(0,-1);
    if(d.slice(-1)==='\n'||d.slice(-1)==='\r') d = d.slice(0,-1);

    t = d.length;

    N = 0;
    n = 0;

    for (p = 120; p <= 975; p++)
        for (q = 28; q >= 8; q--)
            if (t === (p + q - 1)) {
                N = p;
                n = q;
            }

    console.log(d.slice(0, 10) + "...");

    if (N > 0 && n > 0)
        console.log("N = ", N, ", n = ", n, ", t = N+n-1 = ", t);

    if (t !== d.length)throw "Unexpected input..."

    fs.appendFileSync(f, '|');
    for (i = 0; i < N; i++) fs.appendFileSync(f, d.slice(i, i + n) + '|');
    fs.appendFileSync(f, '\n');

} // shred()

var outf = process.argv[2].replace("_init\.", '\.');
fs.writeFileSync(outf, '');

fs.readFileSync(process.argv[2]).toString().split('\n').forEach(
    function (line) {
        if (line != "") shred(outf, line);
    }
);