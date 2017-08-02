/**
 * Created by Olivier on 2017-06-02.
 */

import weka.classifiers.meta.FilteredClassifier;
import weka.classifiers.trees.J48;
import weka.core.Instance;
import weka.core.Instances;
import weka.core.converters.ConverterUtils.DataSource;

import java.io.IOException;
import java.io.PrintWriter;

public class Single implements Strategy {

    public static void main(String[] args) throws Exception {
        new Lab(args[0], args[1], new Single());
    }

    @Override
    public void loadModel() {

    }

    @Override
    public String classify(Instance instance) {
        return null;
    }
}
