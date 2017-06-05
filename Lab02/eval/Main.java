/**
 * Created by Olivier on 2017-06-02.
 */

import weka.classifiers.meta.FilteredClassifier;
import weka.classifiers.trees.J48;
import weka.core.Instances;
import weka.core.converters.ConverterUtils.DataSource;

import java.io.PrintWriter;
import java.io.File;

class ObservationSet {
    public int cut;
    public int noise
}

public class Main {

    static String OUTPUT_DIR = "output";
    static String EVAL_DIR = OUTPUT_DIR + "/evals";
    private ArrayList<int> cuts = {100, 50, 25, 25, 25};
    private ArrayList<int> noises = {0, 0, 0, 5, 20};

    public static void main(String[] args) {
        try {
            for (int i = 0; i < args.length; i++) {
                System.out.printf("%d: %s\n", i, args[i]) ;
            }
            new File(EVAL_DIR).mkdirs();
            for (int i = 0; i < cuts.length; i++) {
                
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static FilteredClassifier arbreDecision(Instances data, boolean unpruned) throws Exception {

        J48 tree = new J48();
        tree.setUnpruned(unpruned);

        FilteredClassifier fc = new FilteredClassifier();

        fc.setClassifier(tree);
        fc.buildClassifier(data);
        return fc;

    }



}
