var fs = require("fs");

function trans(m) {
    var i, j, r, n;
    r = [];
    n = m.length;
    for (i = 0; i < n; i++)
        for (j = 0, r[i] = []; j < n; j++)
            r[i][j] = m[j][i];
    return r;
} // trans

function sortcols(input) {
    var d, i;

    d = input.replace(/\s*\|\s*/g, "|").replace(/\n+|\r+|\f+/g, '').split('|');

    for (i = 0; i < d.length; i++)
        d[i] = d[i].split(/\s+/);

    d = trans(trans(d).sort(function (a, b) {
        var k;
        for (k = 0; k < a.length; k++)
            if (a[k] - b[k] !== 0)
                return  a[k] - b[k];
        return 0;
    }));

    for (i = 0; i < d.length; i++)
        d[i] = d[i].join(' ');

    return d;

} // sortcols

fs.readFileSync(process.argv[2]).toString().split('\n').forEach(function (line) {
    if (line != "") {
        console.log(sortcols(line).join(" | "));
    }
});