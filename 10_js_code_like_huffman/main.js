var fs = require("fs");

const DBGOUTPUT = true;

function tree_recur(t, nid, r, c) {
    if (t[nid].left >= 0 && t[nid].right >= 0) {
        tree_recur(t, t[nid].left, r, c + "" + "0");
        tree_recur(t, t[nid].right, r, c + "" + "1");
    }
    else r.push(t[nid].ch + ": " + c);
} // tree_recur()

function huffman(input) {
    var d,
        freq,
        tree,
        n,
        nfree,
        newindex,
        npriomax,
        result,
        nodeid,
        code;

    d = input.replace(/\n+|\r+|\f+/g, '').split('');
    if (DBGOUTPUT) console.log("Input:\n", d);

    freq = [];
    d.forEach(
        function (el) {
            if (freq[el] === undefined) freq[el] = 1;
            else freq[el]++;
        });
    if (DBGOUTPUT) console.log("freq:\n", freq);

    tree = [];
    newindex = 0;
    Object.keys(freq).forEach(
        function (el) {
            tree.push({id: newindex++, ch: el, pr: freq[el], p: -1, left: -1, right: -1});
        });
    if (DBGOUTPUT) console.log("tree:\n", tree);

    npriomax = tree.reduce(function (sum, cur) {
        return sum + cur.pr
    }, 0);
    if (DBGOUTPUT) console.log("npriomax:\n", npriomax);

    nfree = n = tree.length;
    if (DBGOUTPUT) console.log("nfree=n:\n", nfree);

    while (nfree > 1) {

        if (tree[10] && tree[10].id === 10)
            var iii = 0;

        tree.sort(
            function (a, b) {
                var apr,
                    bpr;
                apr = a.pr;
                bpr = b.pr;
                if (a.p >= 0) apr = npriomax;
                if (b.p >= 0) bpr = npriomax;
                if (apr === bpr)
                    if (a.ch.length === b.ch.length) return a.ch > b.ch ? 1 : -1;
                    else return a.ch.length < b.ch.length ? 1 : -1;
                return apr - bpr;
            }
        );
        if (DBGOUTPUT) console.log("tree sorted:\n", tree);

        tree[0].p = tree.length;
        tree[1].p = tree.length;

        tree.push({
            id: tree.length,
            ch: (tree[0].ch + "" + tree[1].ch),//.split('').sort().join(''),
            pr: tree[0].pr + tree[1].pr,
            p: -1,
            left: tree[0].id,
            right: tree[1].id
        });

        if (DBGOUTPUT) console.log("new tree:\n", tree);
        nfree--;
    }

    if (DBGOUTPUT) console.log("tree[(2*n-1)-1]:\n", tree[(2 * n - 1) - 1]);

    tree.sort(function (a, b) {
        return a.id - b.id;
    });
    if (DBGOUTPUT) console.log("tree final resort:\n", tree);

    result = [];
    nodeid = (2 * n - 1) - 1;
    code = "";
    tree_recur(tree, nodeid, result, code);
    result = result.sort().join("; ") + ";";
    if (DBGOUTPUT) console.log("result:\n", result);
    if (DBGOUTPUT) console.log("RELEASE OUTPUT:");
    console.log(result);

} // huffman()

fs.readFileSync(process.argv[2]).toString().split('\n').forEach(function (line) {
    if (line != "") {
        huffman(line);
        if (DBGOUTPUT) console.log();
    }
});

