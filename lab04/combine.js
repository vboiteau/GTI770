const fs = require('fs');
const { parse, stringify} = require('csv');

if (process.argv.length !== 5) {
   console.error('should have three arguments first distribution to parse, second distribution to write and combined distribution to print');
}

const isDistributionLine = line => line.split(',').length === 29;

const processLine = ([, expected,,,...distribution]) => {
    return [
    expected,
    ...distribution.map(classDistribution => Number(classDistribution.replace(/[^\d^.]/g,'')))
    ];
};

let maximumValue = 0;

const processCSV = csv => {
    maximumValue = csv
        .split('\n')
        .filter(isDistributionLine)
        .length;

    return csv
        .split('\n')
        .filter(isDistributionLine)
        .splice(1)
        .map(line => line.split(','))
        .map(processLine);
};


let firstCSV = '';
let secondCSV = '';
const first = [];
const second = [];
const genres = [
    "BIG_BAND",
    "BLUES_CONTEMPORARY",
    "COUNTRY_TRADITIONAL",
    "DANCE",
    "ELECTRONICA",
    "EXPERIMENTAL",
    "FOLK_INTERNATIONAL",
    "GOSPEL",
    "GRUNGE_EMO",
    "HIP_HOP_RAP",
    "JAZZ_CLASSIC",
    "METAL_ALTERNATIVE",
    "METAL_DEATH",
    "METAL_HEAVY",
    "POP_CONTEMPORARY",
    "POP_INDIE",
    "POP_LATIN",
    "PUNK",
    "REGGAE",
    "RNB_SOUL",
    "ROCK_ALTERNATIVE",
    "ROCK_COLLEGE",
    "ROCK_CONTEMPORARY",
    "ROCK_HARD",
    "ROCK_NEO_PSYCHEDELI"
];


const combineLine = (operator, firstDistribution, secondDistribution) => firstDistribution
    .map((firstClassDistribution, i) => {
        const secondClassDistribution = secondDistribution[i];
        switch (operator) {
            case 'SUM':
                return firstClassDistribution + secondClassDistribution
            case 'MAX': 
            default:
                return Math.max(firstClassDistribution, secondClassDistribution);
                
        }
    });

const doubleCombine = ([firstParsed, secondParsed]) => ['SUM', 'MAX']
    .map(operator => firstParsed.map(([expected,...firstDistribution], i) => {
        const [,...secondDistribution] = secondParsed[i];
        return [expected, ...combineLine(operator, firstDistribution, secondDistribution)]
    }).map(([expected, ...distribution]) => {
        const genreIndex = distribution.indexOf(Math.max(...distribution));
        return [
            expected,
            `${genreIndex + 1}:${genres[genreIndex]}`,
            ...distribution
        ];
    }).filter(([expected, found]) => expected === found).length);

Promise
    .all([
        new Promise((resolve, reject) => {
            fs.createReadStream(`${__dirname}/${process.argv[2]}`)
                .on('data', chunk => {
                    firstCSV += chunk.toString();
                })
                .on('error', e => reject(e))
                .on('end', () => resolve(processCSV(firstCSV, first)))
        }),
        new Promise((resolve, reject) => {
            fs.createReadStream(`${__dirname}/${process.argv[3]}`)
                .on('data', chunk => {
                    secondCSV += chunk.toString();
                }).on('error', e => {
                    reject(e);
                }).on('end', () => resolve(processCSV(secondCSV)))
        })
    ])
    .then(doubleCombine)
    .then(([sumDistribution, maxDistribution]) => {
        console.log(`Line Count\n${maximumValue}\nVote\tCount\t%\nSUM\t${sumDistribution}\t${sumDistribution/maximumValue}\nMAX\t${maxDistribution}\t${maxDistribution/maximumValue}\n\n`);
    }).catch(err => console.error(err));
