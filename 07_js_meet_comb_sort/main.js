var fs = require("fs");

const DBGOUTPUT = false;
const factor = 1.25;

function get_number_of_iterations(input) {
    var d, jump, numofiter, i, t, swap;

    d = input.replace(/\n+|\r+|\f+/g, '').split(' ');
    if (DBGOUTPUT) console.log("Input:\n", d);

    jump = d.length;
    numofiter = 0;
    swap = true;
    while (jump > 1 || swap) {
        jump = Math.floor(jump / factor);
        swap = false;
        for (i = 0; i + jump < d.length; i++)
            if (d[i] > d[i + jump]) {
                t = d[i];
                d[i] = d[i + jump];
                d[i + jump] = t;
                swap = true;
            }
        if (jump > 1 || swap) numofiter++;
    }
    return numofiter;
} // get_number_of_iterations()

fs.readFileSync(process.argv[2]).toString().split('\n').forEach(function (line) {
    if (line != "") {
        console.log(get_number_of_iterations(line));
    }
});