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
        Lab lab = new Lab();
        Strategy str = new Single();
        lab.execute(args[0], args[1], str);
    }

    @Override
    public void loadModel() {

    }

    @Override
    public String classify(Instance instance) {
        return null;
    }
}
