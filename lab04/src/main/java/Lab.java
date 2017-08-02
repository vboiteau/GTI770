import weka.core.Instance;
import weka.core.Instances;

import java.io.IOException;
import java.lang.String;


class Lab {

    Lab(String input, String output, Strategy strategy) throws Exception {
        strategy.loadModel();
        Instances instances = load(input);
        if (instances != null) {
            for (Instance instance : instances) {
                write(strategy.classify(instance));
            }
        }
    }

    private Instances load(String input) {
        return null;
    }

    private void write(String className) throws IOException {

    }
}