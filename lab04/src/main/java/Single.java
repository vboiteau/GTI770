/**
 * Created by Olivier on 2017-06-02.
 */

import weka.classifiers.Classifier;
import weka.core.Instance;
import weka.core.Instances;

class Single implements ClassificationStrategy {

    public static void main(String[] args) throws Exception {
        Lab lab = new Lab();
        ClassificationStrategy str = new Single(lab);
        lab.execute(args[0], args[1], str);
    }

    private Lab lab;
    private Classifier classifier;

    private Single(Lab lab) {
        this.lab = lab;
    }

    @Override
    public void loadModel() throws Exception {
        classifier = lab.loadClassifier("single.model");
    }

    @Override
    public synchronized String classify(Instance instance) throws Exception {

        return lab.getClassName((int)classifier.classifyInstance(instance));
    }
}
