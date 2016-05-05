/**
 * Created by achemichev on 22.01.2016.
 */

var fs = require("fs");

const DBGOUTPUT = true;
const RELEASEOUTPUT = true;

function glue(input) {
    const DELIM = '|';
    var d = input;

    d = d.split(DELIM);
    d = d.slice(1, -1);

    var N = d.length;
    if (DBGOUTPUT) console.log("Number of pieces = ", N);

    var n = d[0].length;
    if (DBGOUTPUT) {
        console.log("Piece length = ", n);
        var check = d.every(function (el) {
            return ( el.length === n )
        });
        if (!check || n <= 1) throw "Piece length check failed...";
        else console.log("Piece length check OK");
    }

    var t = N + n - 1;
    if (DBGOUTPUT) console.log("Solution length = ", t);

    /*
    var leftParts = d.map(function (el) {
        return el.slice(0, -1);
    });

    var rightParts = d.map(function (el) {
        return el.slice(1);
    });

    var bothParts = [];
    Array.prototype.push.apply(bothParts, leftParts);
    Array.prototype.push.apply(bothParts, rightParts);

    if (DBGOUTPUT) console.log("Left parts = ", leftParts);
    if (DBGOUTPUT) console.log("Right parts = ", rightParts);
    if (DBGOUTPUT) console.log("Both parts = ", bothParts);

    var bothParts = [];
    Array.prototype.push.apply(bothParts, leftParts);
    Array.prototype.push.apply(bothParts, rightParts);

    if (DBGOUTPUT) console.log("Left parts = ", leftParts);
    if (DBGOUTPUT) console.log("Right parts = ", rightParts);
    if (DBGOUTPUT) console.log("Both parts = ", bothParts);
    */


    var leftMap = {}, rightMapCounts = {};
    d.forEach(function (el) {
        var left = el.slice(0, -1);
        var last = el.slice(-1);
        if (leftMap[left] === undefined) leftMap[left] = last;
        else leftMap[left] += "" + last;
        var right = el.slice(1);
        if (rightMapCounts[right] === undefined) rightMapCounts[right] = 1;
        else rightMapCounts[right]++;
    });

    if (DBGOUTPUT) console.log("Left map = ", leftMap);
    if (DBGOUTPUT) console.log("Right map counts = ", rightMapCounts);

    /*
    var leftCounts = {}, rightCounts = {}, bothCounts = {};

    [
        {a: leftParts, m: leftCounts},
        {a: rightParts, m: rightCounts},
        {a: bothParts, m: bothCounts}
    ].forEach(function (obj) {
        obj.a.forEach(function (k) {
            if (obj.m[k] === undefined) obj.m[k] = 1;
            else obj.m[k]++;
        });
    });

    if (DBGOUTPUT) console.log("Left counts = ", leftCounts);
    if (DBGOUTPUT) console.log("Right counts = ", rightCounts);
    if (DBGOUTPUT) console.log("Both counts = ", bothCounts);
    */

    /*
    var listByLeft = {}, listByRight = {};
    d.forEach(function (el) {
        var left = el.slice(0, -1);
        var right = el.slice(1);

        if (listByLeft[left] === undefined) listByLeft[left] = [el];
        else listByLeft[left].push(el);

        if (listByRight[right] === undefined) listByRight[right] = [el];
        else listByRight[right].push(el);
    });

    if (DBGOUTPUT) console.log("List by left = ", listByLeft);
    if (DBGOUTPUT) console.log("List by right = ", listByRight);
    */

    var startLeftPiece/*, endRightPiece*/;

    var leftMapKeys = Object.keys(leftMap);
    for (var i = 0; i < leftMapKeys.length; i++) {
        var key = leftMapKeys[i];
        var rlen = rightMapCounts[key] | 0;
        if (leftMap[key].length - rlen > 0)
            startLeftPiece = key;
    }

    /*
    var rightMapCountKeys = Object.keys(rightMapCounts);
    for (var i = 0; i < rightMapCountKeys.length; i++) {
        var key = rightMapCountKeys[i];
        var llen = leftMap[key] ? leftMap[key].length : 0;
        if (rightMapCounts[key] - llen > 0)
            endRightPiece = key;
    }
    */

    /*
     var bothCountsKeys = Object.keys(bothCounts);
     for (var i = 0, key = bothCountsKeys[0]; i < bothCountsKeys.length; key = bothCountsKeys[++i])
     if (bothCounts[key] % 2) {
     (leftCounts[key] | 0) > (rightCounts[key] | 0) ? (startLeftPiece = key) : (endRightPiece = key);
     if (startLeftPiece && endRightPiece) break;
     }
     */

    if (DBGOUTPUT) {
        if (startLeftPiece /*&& endRightPiece*/) {
            console.log("startLeftPiece = ", startLeftPiece);
            //console.log("endRightPiece = ", endRightPiece);
        }
        else {
            //throw "No decision, loop cycle possible...";
            console.log("No decision, loop cycle possible...");
        }
    } // if (DBGOUTPUT)

    return d;
} // glue()

fs.readFileSync(process.argv[2]).toString().split('\n').forEach(
    function (line) {
        if (line != "") {
            if (DBGOUTPUT) console.log(line);
            if (DBGOUTPUT) console.log("Solution:");
            if (RELEASEOUTPUT) console.log(glue(line));
            if (DBGOUTPUT) console.log();
        }
    }
);