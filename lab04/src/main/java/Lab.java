import weka.classifiers.Classifier;
import weka.core.Instance;
import weka.core.Instances;
import weka.core.SerializationHelper;

import java.io.File;
import java.io.IOException;
import java.lang.String;
import java.net.URL;


class Lab {

    void execute(String input, String output, Strategy strategy) throws Exception {
        strategy.loadModel();
        Instances instances = load(input);
        if (instances != null) {
            for (Instance instance : instances) {
                write(strategy.classify(instance));
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

    private Instances load(String input) {
        return null;
    }

    private void write(String className) throws IOException {

    }
}