/**
 * Created by achemichev on 22.01.2016.
 */

var fs = require("fs");

function glue(input) {

    const DELIM = '|';

    var d = input,
        N, // num of pieces
        n, // piece length
        t, // solution string length
        leftMap = {}, leftMapKeys, rightMapCounts = {}, startLeftPiece, i, key;

    function clone(obj) {
        return JSON.parse(JSON.stringify(obj));
    }

    function glueRecur(solution, map) {

        var recurKey, mapClone, newSolution, bestNewSolution, k, charsPossible, char2Add;

        if (solution.length === t) return solution;
        recurKey = solution.slice(1 - n);
        charsPossible = map[recurKey];
        if (charsPossible === undefined) return solution;

        for (k = 0; k < charsPossible.length; k++) {
            char2Add = charsPossible[k];
            mapClone = clone(map);
            mapClone[recurKey] = charsPossible.slice(0, k) + charsPossible.slice(k + 1);
            newSolution = glueRecur(solution + "" + char2Add, mapClone);
            if (bestNewSolution === undefined || ( newSolution && bestNewSolution.length < newSolution.length ))
                bestNewSolution = newSolution;
            if (bestNewSolution && bestNewSolution.length === t) break;
        } // for

        return bestNewSolution;
    } // glueRecur()

    d = d.split(DELIM);
    d = d.slice(1, -1);
    N = d.length;
    n = d[0].length;
    t = N + n - 1;

    d.forEach(function (el) {
        var left = el.slice(0, -1);
        var last = el.slice(-1);
        if (leftMap[left] === undefined) leftMap[left] = last;
        else leftMap[left] += "" + last;
        var right = el.slice(1);
        if (rightMapCounts[right] === undefined) rightMapCounts[right] = 1;
        else rightMapCounts[right]++;
    });

    leftMapKeys = Object.keys(leftMap);
    for (i = 0, key = leftMapKeys[0]; i < leftMapKeys.length; key = leftMapKeys[++i])
        if (leftMap[key].length - (rightMapCounts[key] | 0) > 0)
            startLeftPiece = key;

    if (startLeftPiece)
        return glueRecur(startLeftPiece, leftMap);

} // glue()

fs.readFileSync(process.argv[2]).toString().split('\n').forEach(
    function (line) {
        if (line != "") console.log(glue(line));
    }
);