import weka.core.Instance;

import java.io.IOException;

/**
 * Created by lodonoughue on 2017-08-02.
 */
interface Strategy {

    void loadModel() throws Exception;
    String classify(Instance instance);

}
