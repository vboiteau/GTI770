import weka.classifiers.Classifier;
import weka.core.Instance;
import weka.core.SerializationHelper;

/**
 * Created by lodonoughue on 2017-08-02.
 */
public class Multi implements Strategy {


    public static void main(String[] args) throws Exception {
        new Lab(args[0], args[1], new Multi());
    }

    private Classifier knnClassifier;
    private Classifier svmClassifier;

    @Override
    public void loadModel() throws Exception {
        knnClassifier = (Classifier) SerializationHelper.read("multi-knn.model");
        svmClassifier = (Classifier) SerializationHelper.read("multi-svm.model");
    }

    @Override
    public String classify(Instance instance) {
        return null;
    }
}
