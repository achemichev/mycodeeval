var fs = require("fs");

var DEBUGOUTPUT = true;

function transpose_matrix(m) {
    var i, j, r, nrows, ncols;
    r = [];
    nrows = m[0].length;
    ncols = m.length;
    for (i = 0; i < nrows; i++)
        for (j = 0, r[i] = []; j < ncols; j++)
            r[i][j] = m[j][i];
    return r;
} // trans

function sort_matrix(input) {
    var d, i;

    d = input;
    if (DEBUGOUTPUT) console.log("Input:\n" + d);

    d = d.replace(/\s*\|\s*/g, "|").replace(/\n+|\r+|\f+/g, '');
    d = d.split('|');

    for (i = 0; i < d.length; i++)
        d[i] = d[i].split(/\s+/);

    if (DEBUGOUTPUT) console.log("Splits:");
    if (DEBUGOUTPUT) console.log(d);

    d = transpose_matrix(d);

    if (DEBUGOUTPUT) console.log("Transposed:");
    if (DEBUGOUTPUT) console.log(d);

    d.sort(function (a, b) {
        var k;
        for (k = 0; k < a.length; k++)
            if (a[k] - b[k] !== 0)
                return a[k] - b[k];
        return 0;
    });

    if (DEBUGOUTPUT) console.log("Sorted:");
    if (DEBUGOUTPUT) console.log(d);

    d = transpose_matrix(d);

    if (DEBUGOUTPUT) console.log("Transposed back:");
    if (DEBUGOUTPUT) console.log(d);

    for (i = 0; i < d.length; i++)
        d[i] = d[i].join(' ');

    if (DEBUGOUTPUT) console.log("Pre-joined result:");
    if (DEBUGOUTPUT) console.log(d);

    return d;

} // sortcols

fs.readFileSync(process.argv[2]).toString().split('\n').forEach(function (line) {
    if (line != "") {
        console.log(sort_matrix(line).join(" | "));
    }
});