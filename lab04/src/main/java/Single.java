/**
 * Created by Olivier on 2017-06-02.
 */

import weka.core.Instance;

class Single implements ClassificationStrategy {

    public static void main(String[] args) throws Exception {
        Lab lab = new Lab();
        ClassificationStrategy str = new Single();
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
