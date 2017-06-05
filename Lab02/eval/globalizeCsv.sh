#!/bin/bash
for algo in tree naiveBayes knn; do
    csvs=$(find output/$algo -name "*.csv")
    first=1
    for csv in $csvs; do
        if [[ $first == 1 ]]; then
            sed -n '1p' < $csv > output/$algo.csv
            first=0
        fi
        sed -n '2p' < $csv >> output/$algo.csv
    done
done
