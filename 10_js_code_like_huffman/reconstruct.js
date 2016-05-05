var fs = require("fs");

function reconstruct(input) {
    var d,
        nmax,
        result;
    d = input.replace(/\n+|\r+|\f+/g, '').replace(/;$/, '').split('; ');
    d.forEach(function (el, index) {
        d[index] = el.split(": ");
        var n = parseInt(d[index][1], 2);
        d[index].push(n);
    });

    nmax = d.reduce(function (prev, cur) {
        return cur[2] > prev ? cur[2] : prev
    }, 0);

    d.forEach(function (el, index) {
        d[index].push(nmax - el[2] + 1);
    });

    console.log(d);
}

fs.readFileSync(process.argv[2]).toString().split('\n').forEach(function (line) {
    if (line != "") {
        reconstruct(line);

    }
});