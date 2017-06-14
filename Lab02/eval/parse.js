'use strict';
const fs = require('fs');
let [,,path] = process.argv;
let [,algo, ...args] = path.split('/');
let filename = args.pop();
let [,cut=100,noise=0] = filename.match(/spambase-train([\d]*)?-?(\d*)?/);
fs.readFile(path, {encoding: 'utf8'}, (err, content) => {
    let [start, end] = content.split("=== Error on test data ===\n");
    let [summary, details] = end.split("=== Detailed Accuracy By Class ===\n");
    summary = summary.match(/(\d+\.?\d*)/g).map(n => Number(n));
    let [, success,, error] = summary;
    let total = summary.pop();
    summary = {cut, noise, success, error, total};
    if (path.indexOf('tree') !== -1) {
       let [leaveCount, treeSize] = start.split('\n').filter(ln => /^(Number\sof\sLeaves|Size\sof\sthe\stree).*/.test(ln)).map(ln => Number(ln.match(/(\d+)/g)[0])); 
        Object.assign(summary, {leaveCount, treeSize});
    }
    switch (algo) {
        case 'tree':
            let pruned = args[0]==='pruned';
            Object.assign(summary, {pruned});
            break;
        case 'naiveBayes':
            Object.assign(summary, {representation:args[0]});
            break;
        case 'knn':
            Object.assign(summary, {distancePonderation:args[0],  k: Number(args[1])});
            break;
    }
    details = details.split("\n").filter(line => line.match(/^Weighted\sAvg\./))[0];
    let [tpRate, fpRate, precision, recall, fMeasure, MCC, ROCArea, PRCArea] = details.match(/(\d+\.?\d*)/g).map(n => Number(n));
    Object.assign(summary, {tpRate, fpRate, precision, recall, fMeasure, MCC, ROCArea, PRCArea});
    let csv = `${Object.keys(summary).join(',')}\n${Object.keys(summary).map(key => summary[key]).join(',')}\n`;
    fs.writeFile(path.replace('txt','csv'), csv, {}, (err) => {
        if (err) {
           console.error(err); 
        }
    });
});
