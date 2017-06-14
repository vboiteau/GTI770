'use strict';
const fs = require('fs');
const path = require('path');
const Spawn = require('child_process').spawn;
const WEKA_JAR = '/Applications/Weka.app/Contents/Java/weka.jar';
var algos = [
    {
        name: 'tree',
        variables: [ 
            {
                name: 'pruned',
                possibilities: [
                    {
                        command: '-C 0.25',
                        value: true
                    },
                    {
                        command: '-U',
                        value: false
                    }
                ]
            }
        ],
        commons: '-M 2',
        classifier: 'trees.J48'
    }
];

function getArffs(startDir){
   let files = fs.readdirSync(startDir);
    let arffs = files.reduce((arffs, file) => {
        let filePath = path.join(startDir, file),
            stat = fs.lstatSync(filePath);
        if (stat.isDirectory()) {
            arffs = [...arffs, ...getArffs(filePath)];
        }
        if(path.parse(file).ext === '.arff'){
            arffs.push(filePath);
        }
        return arffs;
    },[]);

    return arffs;
}

let arffs = getArffs(__dirname);

const trains = arffs.filter(arff => /train/.test(arff)), 
    valid = arffs.filter(arff => /valid/.test(arff))[0];

let promises = algos.reduce((promises, algo) => {
    return [...promises, ...testVariable(algo, algo.variables, [])];
}, []);

Promise.all(promises).then(results => {
    results.map((result, i) => {
        let sections = result.output.split(/===\s.*\s===\n/).filter(char => char);
        let headers = ['general', ...result.output.match(/===\s.*\s===\n/g).map(ln => ln.match(/===\s(.*)\s===\n/)[1])];
        if (i === 0) {
            console.log(sections.length, headers.length);
        }
        return result;
    });
}).catch(err =>  console.error(err));

function getPromise({algo, newChosenVariables=[]}, train) {
    return new Promise((resolve, reject) => {
        let spawn = Spawn('java', [
            '-cp',
            WEKA_JAR,
            `weka.classifiers.${algo.classifier}`,
            ...newChosenVariables.reduce((string, variable) => {
                return `${string} ${variable.command}`;
            },'').split(' '),
            ...algo.commons.split(' '),
            '-t',
            train,
            '-T',
            valid
        ]);
        let output = '';

        spawn.stdout.on('data', data => output+=data.toString());

        spawn.stderr.on('data', err => reject(err.toString()))

        spawn.on('close', code => {
            if (!code) {
                resolve({algo, vars: newChosenVariables, output});
            }
        });
    });
}

function parsePossibility({algo, currentVariable={}, leftVariables=[], chosenVariables=[]}, possibility) {
    let newChosenVariables = [...chosenVariables, {
        name: currentVariable.name,
        command: possibility.command,
        value: possibility.value
    }];
    let promises = [];
    if (leftVariables.length) {
        console.dir(object);
         promises = [...promises, ...testVariable(algo, leftVariables, newChosenVariables)];
    } else {
        promises = [...promises, ...trains.map(getPromise.bind(null, {algo, newChosenVariables}))];
    }
    return promises;
}

function testVariable(algo, leftVariables, chosenVariables) {
    let currentVariable = {};
    [currentVariable, ...leftVariables] = algo.variables;
    let promises = currentVariable.possibilities
        .map(parsePossibility.bind(null,{
            algo,
            currentVariable,
            leftVariables,
            chosenVariables
        }))
        .reduce((promises, cursor) => {
            return [...promises, ...cursor];
        }, []);
    return promises;
}
