import weka.classifiers.Classifier;
import weka.classifiers.bayes.NaiveBayes;
import weka.classifiers.lazy.IBk;
import weka.classifiers.trees.J48;
import weka.core.Instance;
import weka.core.Instances;
import weka.core.SerializationHelper;

import java.io.*;

public class SpamClassifier {

    public static void main(String[] args) throws IOException {
        if (args.length < 1) throw new RuntimeException ("File name is required.");
        new SpamClassifier(args[0], args[1], args[2]);
    }

    private SpamClassifier(String fileNameData, String fileNamePlus, String fileNameMoins) {
        System.out.println("Starting classifiers in parallel");

        Thread th1 = new Thread(() -> {
            try {
                //J48 algo = new J48();
                //algo.setUnpruned(false);
                Classifier j48Classifier  =  (J48) SerializationHelper.read("best.model");
                runAlgorithm(j48Classifier, fileNameData, fileNamePlus);
            } catch (Exception e) {
                e.printStackTrace();
            }
            System.out.println("Execution of J48 ended");
        });
        th1.run();

        Thread th2 = new Thread(() -> {
            try {
                Classifier bayesClassifier  =  (NaiveBayes) SerializationHelper.read("worst.model");
                runAlgorithm(bayesClassifier, fileNameData, fileNameMoins);
            } catch (Exception e) {
                e.printStackTrace();
            }
            System.out.println("Execution of NaiveBayes ended");
        });
        th2.run();

        Thread th3 = new Thread(() -> {
            try {
                runAlgorithm(new IBk(), fileNameData, "EquipeI-ibk.txt");
            } catch (Exception e) {
                e.printStackTrace();
            }
        });
        //th3.run();

        try {
            th1.join();
            th2.join();
            th3.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private void runAlgorithm(Classifier classifier, String fileIn, String fileOut) throws Exception {
        BufferedReader reader = null;
        BufferedWriter writer = null;

        try {
            reader = new BufferedReader(new FileReader(fileIn));
            writer = new BufferedWriter(new FileWriter(fileOut));

            Instances data = new Instances(reader);

            int attributeIndex = data.numAttributes() - 1;
            data.setClassIndex(attributeIndex);

            //classifier.buildClassifier(data);

            for (Instance instance : data) {
                double c = classifier.classifyInstance(instance);
                writer.write(data.classAttribute().value((int) c));
                writer.newLine();
            }


        } finally {
            if (reader != null)
                reader.close();
            if (writer != null)
                writer.close();
        }
    }



}