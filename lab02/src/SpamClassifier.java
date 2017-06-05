import weka.classifiers.Classifier;
import weka.classifiers.bayes.NaiveBayes;
import weka.classifiers.lazy.IBk;
import weka.classifiers.trees.J48;
import weka.core.Instance;
import weka.core.Instances;

import java.io.*;

public class SpamClassifier {

    public static void main(String[] args) throws IOException {
        if (args.length < 1) throw new RuntimeException ("File name is required.");
        new SpamClassifier(args[0]);
    }

    private SpamClassifier(String filename) {

        Thread th1 = new Thread(() -> {
            try {
                J48 algo = new J48();
                algo.setUnpruned(false);
                runAlgorithm(algo, filename, "EquipeI-plus.txt");
            } catch (Exception e) {
                e.printStackTrace();
            }
        });
        th1.run();

        Thread th2 = new Thread(() -> {
            try {
                runAlgorithm(new NaiveBayes(), filename, "EquipeI-moins.txt");
            } catch (Exception e) {
                e.printStackTrace();
            }
        });
        th2.run();

        Thread th3 = new Thread(() -> {
            try {
                runAlgorithm(new IBk(), filename, "EquipeI-ibk.txt");
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

            classifier.buildClassifier(data);

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