#!/bin/bash
source ./config.sh

HiddenLayer=80
if [ -z $1 ]; then
    echo "First parameter should be hidden layers, will use default $HiddenLayer"
else
    HiddenLayer=$1
fi

Cycle=1000
if [ -z $2 ]; then
    echo "Second parameter should be cycle, will use default $Cycle" 
else
    Cycle=$2
fi

TRAIN_FILE="./arff/Char_UpperLower52.train.arff"

TEST_FILE="./arff/Char_UpperLower52.val.arff"

OUTPUT_NAME="test_H${HiddenLayer}_N${Cycle}"
if [ -z $3 ]; then
    echo "Second parameter should be the output name without extension, will use default $OUTPUT_NAME."
else
    OUTPUT_NAME=$3
fi


classifier="functions.MultilayerPerceptron"

arguments="-L 0.3 -M 0.2 -S 0 -E 20 -N $Cycle -H $HiddenLayer -t $TRAIN_FILE -T $TEST_FILE -d output/$OUTPUT_NAME.model"
eval "java -cp $WEKA_JAR weka.classifiers.${classifier} $arguments > output/$OUTPUT_NAME.txt"
