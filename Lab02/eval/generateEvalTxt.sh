#!/bin/bash
trainFiles=$(ls -1 *.arff | grep "train" | sed -e 's/\..*$//')
validFile=spambase-valid.arff
for folder in output/tree/pruned output/tree/unpruned output/naiveBayes/normal output/naiveBayes/discretisation output/naiveBayes/kernel output/knn/noWeight output/knn/I output/knn/F; do
    mkdir -p $folder
done
ks="1 3 5 7 9"
for k in $ks; do
    mkdir -p output/knn/noWeight/$k 
    mkdir -p output/knn/I/$k 
    mkdir -p output/knn/F/$k 
done

wekaJar=/Applications/Weka.app/Contents/Java/weka.jar

for file in $trainFiles; do
    echo $file
    java -cp $wekaJar weka.classifiers.trees.J48 -C 0.25 -M 2 -t $file.arff -T $validFile -d output/tree/pruned/$file.model > output/tree/pruned/$file.txt && node parse output/tree/pruned/$file.txt
    java -cp $wekaJar weka.classifiers.trees.J48 -U -M 2 -t $file.arff -T $validFile -d output/tree/unpruned/$file.model > output/tree/unpruned/$file.txt && node parse output/tree/unpruned/$file.txt
    echo "bayes\n"
    java -cp $wekaJar weka.classifiers.bayes.NaiveBayes -t $file.arff -T $validFile -d output/naiveBayes/normal/$file.model > output/naiveBayes/normal/$file.txt && node parse output/naiveBayes/normal/$file.txt
    java -cp $wekaJar weka.classifiers.bayes.NaiveBayes -D -t $file.arff -T $validFile -d output/naiveBayes//$file.model > output/naiveBayes/discretisation/$file.txt && node parse output/naiveBayes/discretisation/$file.txt
    java -cp $wekaJar weka.classifiers.bayes.NaiveBayes -K -t $file.arff -T $validFile -d output/naiveBayes/kernel/$file.model > output/naiveBayes/kernel/$file.txt && node parse output/naiveBayes/kernel/$file.txt
    for k in $ks; do
        echo "k:\t $k\n"
        java -cp $wekaJar weka.classifiers.lazy.IBk -K $k -W 0 -A "weka.core.neighboursearch.LinearNNSearch -A \"weka.core.EuclideanDistance -R first-last\"" -t $file.arff -T $validFile -d output/naiveBayes/noWeight/$k/$file.model > output/knn/noWeight/$k/$file.txt && node parse output/knn/noWeight/$k/$file.txt
        java -cp $wekaJar weka.classifiers.lazy.IBk -K $k -W 0 -I -A "weka.core.neighboursearch.LinearNNSearch -A \"weka.core.EuclideanDistance -R first-last\"" -t $file.arff -T $validFile -d output/naiveBayes/I/$k/$file.model > output/knn/I/$k/$file.txt && node parse output/knn/I/$k/$file.txt
        java -cp $wekaJar weka.classifiers.lazy.IBk -K $k -W 0 -F -A "weka.core.neighboursearch.LinearNNSearch -A \"weka.core.EuclideanDistance -R first-last\"" -t $file.arff -T $validFile -d output/naiveBayes/F/$k/$file.model > output/knn/F/$k/$file.txt && node parse output/knn/F/$k/$file.txt
    done
done
sh ./globalizeCsv.sh
