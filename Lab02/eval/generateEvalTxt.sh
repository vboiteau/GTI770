#!/bin/bash
trainFiles=$(ls -1 *.arff | grep "train" | sed -e 's/\..*$//')
for folder in output/tree/pruned output/tree/unpruned output/naiveBayes/normal output/naiveBayes/discretisation output/naiveBayes/kernel output/knn/noWeight output/knn/I output/knn/F; do
    mkdir -p $folder
done
ks="1 3 5 7 9"
for k in $ks; do
    mkdir -p output/knn/noWeight/$k 
    mkdir -p output/knn/I/$k 
    mkdir -p output/knn/F/$k 
done

for file in $trainFiles; do
    echo $file
    java -cp /Applications/Weka.app/Contents/Java/weka.jar weka.classifiers.trees.J48 -C 0.25 -M 2 -t $file.arff > output/tree/pruned/$file.txt && node parse output/tree/pruned/$file.txt
    java -cp /Applications/Weka.app/Contents/Java/weka.jar weka.classifiers.trees.J48 -U -M 2 -t $file.arff > output/tree/unpruned/$file.txt && node parse output/tree/unpruned/$file.txt
    echo "bayes\n"
    java -cp /Applications/Weka.app/Contents/Java/weka.jar weka.classifiers.bayes.NaiveBayes -t $file.arff > output/naiveBayes/normal/$file.txt && node parse output/naiveBayes/normal/$file.txt
    java -cp /Applications/Weka.app/Contents/Java/weka.jar weka.classifiers.bayes.NaiveBayes -D -t $file.arff > output/naiveBayes/discretisation/$file.txt && node parse output/naiveBayes/discretisation/$file.txt
    java -cp /Applications/Weka.app/Contents/Java/weka.jar weka.classifiers.bayes.NaiveBayes -K -t $file.arff > output/naiveBayes/kernel/$file.txt && node parse output/naiveBayes/kernel/$file.txt
    for k in $ks; do
        echo "k:\t $k\n"
        java -cp /Applications/Weka.app/Contents/Java/weka.jar weka.classifiers.lazy.IBk -K $k -W 0 -A "weka.core.neighboursearch.LinearNNSearch -A \"weka.core.EuclideanDistance -R first-last\"" -t $file.arff > output/knn/noWeight/$k/$file.txt && node parse output/knn/noWeight/$k/$file.txt
        java -cp /Applications/Weka.app/Contents/Java/weka.jar weka.classifiers.lazy.IBk -K $k -W 0 -I -A "weka.core.neighboursearch.LinearNNSearch -A \"weka.core.EuclideanDistance -R first-last\"" -t $file.arff > output/knn/I/$k/$file.txt && node parse output/knn/I/$k/$file.txt
        java -cp /Applications/Weka.app/Contents/Java/weka.jar weka.classifiers.lazy.IBk -K $k -W 0 -F -A "weka.core.neighboursearch.LinearNNSearch -A \"weka.core.EuclideanDistance -R first-last\"" -t $file.arff > output/knn/F/$k/$file.txt && node parse output/knn/F/$k/$file.txt
    done
done
sh ./globalizeCsv.sh
