/**
 * Created by Olivier on 2017-06-02.
 */

import weka.classifiers.meta.FilteredClassifier;
import weka.classifiers.trees.J48;
import weka.core.Instances;
import weka.core.converters.ConverterUtils.DataSource;

import java.io.PrintWriter;

public class Main {

    public static void main(String[] args) {
        try {
            DataSource source = new DataSource("spambase-valid.arff");
            Instances data = source.getDataSet();

            data.setClassIndex(data.numAttributes() - 1);

            PrintWriter output = new PrintWriter("test.txt");
            output.print(arbreDecision(data, false));
            output.close();
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
