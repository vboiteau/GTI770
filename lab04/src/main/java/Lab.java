import weka.classifiers.Classifier;
import weka.core.Attribute;
import weka.core.Instance;
import weka.core.Instances;
import weka.core.SerializationHelper;
import weka.filters.Filter;
import weka.filters.unsupervised.attribute.Normalize;
import weka.filters.unsupervised.attribute.Remove;
import weka.filters.unsupervised.attribute.Standardize;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.Reader;
import java.lang.String;
import java.net.URL;
import java.util.Arrays;


class Lab {

    void execute(String input, String output, Strategy strategy) throws Exception {
        strategy.loadModel();
        Instances instances = load(input);
        if (instances != null) {
            for (Instance instance : instances) {
                write(output, strategy.classify(instance));
            }
        }
    }

    Classifier loadClassifier(String filename) throws Exception {
        ClassLoader classloader = getClass().getClassLoader();
        URL fileUrl = classloader.getResource(filename);
        String path = null;

        if (fileUrl != null) {
            path = fileUrl.getFile();
        }

        return (Classifier) SerializationHelper.read(path);
    }

    private Instances load(String input) throws Exception {
        Reader reader = null;
        Instances instances;
        try {

            reader = new BufferedReader(new FileReader(input));
            instances = new Instances(reader);

        } finally {
            if (reader != null) {
                reader.close();
            }
        }
        return normalize(instances);
    }

    private Instances normalize(Instances instances) throws Exception {
        if (instances != null) {
            int[] rmIndices = getAttributeIndices(instances, new String[] {"SAMPLEID", "TRACKID", "class"});

            Remove rm = new Remove();
            rm.setAttributeIndicesArray(rmIndices);
            rm.setInputFormat(instances);
            instances = Filter.useFilter(instances, rm);

            Standardize norm = new Standardize();
            norm.setOptions(new String[] {"-S 2.0", "-T -1.0"});
            norm.setInputFormat(instances);
            instances = Filter.useFilter(instances, norm);
        }
        return instances;
    }

    private int[] getAttributeIndices(Instances instances, String[] attributes) {
        int[] indices = null;

        if (instances != null && attributes != null) {
            indices = new int[attributes.length];
            int nbIndices = 0;

            for (String attrName : attributes) {
                Attribute attr = instances.attribute(attrName);
                if (attr != null) {
                    indices[nbIndices] = attr.index();
                    nbIndices ++;
                }
            }

            if (indices.length > nbIndices) {
                int[] temp = new int[nbIndices];
                System.arraycopy(indices, 0, temp, 0, nbIndices);
                indices = temp;
            }
        }

        return indices;
    }

    private void write(String ouput, String className) throws IOException {

    }
}