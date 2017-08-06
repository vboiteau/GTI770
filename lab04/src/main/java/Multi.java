import weka.classifiers.Classifier;
import weka.core.Instance;

import java.util.Arrays;

/**
 * Created by lodonoughue on 2017-08-02.
 */
class Multi implements ClassificationStrategy {


    public static void main(String[] args) throws Exception {
        Lab lab = new Lab();
        ClassificationStrategy str = new Multi(lab, new MaxCombinationStrategy());
        lab.execute(args[0], args[1], str);
    }


    private Lab lab;
    private CombinationStrategy strategy;
    private Classifier knnClassifier;
    private Classifier svmClassifier;


    private Multi(Lab lab, CombinationStrategy strategy) {
        this.lab = lab;
        this.strategy = strategy;
    }

    @Override
    public void loadModel() throws Exception {
        knnClassifier = lab.loadClassifier("multi-knn.model");
        svmClassifier = lab.loadClassifier("multi-svm.model");
    }

    @Override
    public String classify(Instance instance) throws Exception {
        double[] knnDistr = this.knnClassifier.distributionForInstance(instance);
        double[] svmDistr = this.svmClassifier.distributionForInstance(instance);

        int index = strategy.classIndex(knnDistr, svmDistr);
        return lab.getClassName(index);
    }


    static class SumCombinationStrategy implements CombinationStrategy {
        @Override
        public int classIndex(double[] dstr1, double[] dstr2) {
            final int size = Math.min(dstr1.length, dstr1.length);
            double max = 0.0;
            int index = -1;

            for (int i = 0; i < size; i++) {
                double sum = dstr1[i] + dstr2[i];
                if (sum > max) {
                    max = sum;
                    index = i;
                }
            }

            return index;
        }
    }

    static class MaxCombinationStrategy implements CombinationStrategy {

        @Override
        public int classIndex(double[] dstr1, double[] dstr2) {
            double max = 0.0;
            int index = -1;

            for (int i = 0; i < dstr1.length; i++) {
                if (dstr1[i] > max) {
                    max = dstr1[i];
                    index = i;
                }
            }

            for (int i = 0; i < dstr2.length; i++) {
                if (dstr2[i] > max) {
                    max = dstr2[i];
                    index = i;
                }
            }

            return index;
        }
    }

}
