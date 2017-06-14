#!/bin/bash
source ./config.sh
TRAIN_FILE="./arff/Char_UpperLower52.train.arff"
if [ -z $1 ]; then
    echo "First parameter should be the train file, will use default $TRAIN_FILE"
else
    TRAIN_FILE=$1
fi

TEST_FILE="./arff/Char_UpperLower52.test.arff"
if [ -z $2 ]; then
    echo "Second parameter should be the test file, will use default $TEST_FILE"
else
    TEST_FILE=$2
fi

OUTPUT_NAME="test"
if [ -z $3 ]; then
    echo "Third parameter should be the output name without extension, will use default $OUTPUT_NAME."
else
    OUTPUT_NAME=$3
fi


classifier="functions.MultilayerPerceptron"

arguments="-N 1000 -H \"108,80,52\" -t $TRAIN_FILE -T $TEST_FILE -d $OUTPUT_NAME.model"

eval "java -cp $WEKA_JAR weka.classifiers.${classifier} $arguments > $OUTPUT_NAME.txt"
