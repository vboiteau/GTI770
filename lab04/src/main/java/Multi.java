import weka.classifiers.Classifier;
import weka.core.Instance;

/**
 * Created by lodonoughue on 2017-08-02.
 */
class Multi implements Strategy {


    public static void main(String[] args) throws Exception {
        Lab lab = new Lab();
        Strategy str = new Multi(lab);
        lab.execute(args[0], args[1], str);
    }

    private Lab lab;
    private Classifier knnClassifier;
    private Classifier svmClassifier;

    private Multi(Lab lab) {
        this.lab = lab;
    }

    @Override
    public void loadModel() throws Exception {
        knnClassifier = lab.loadClassifier("multi-knn.model");
        svmClassifier = lab.loadClassifier("multi-svm.model");
    }

    @Override
    public String classify(Instance instance) {
        return null;
    }
}
