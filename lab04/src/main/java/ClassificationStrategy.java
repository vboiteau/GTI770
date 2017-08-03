import weka.core.Instance;


/**
 * Created by lodonoughue on 2017-08-02.
 */
interface ClassificationStrategy {

    void loadModel() throws Exception;
    String classify(Instance instance) throws Exception;

}
