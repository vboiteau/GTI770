import weka.classifiers.Classifier;
import weka.core.Attribute;
import weka.core.Instances;
import weka.core.SerializationHelper;
import weka.filters.Filter;
import weka.filters.unsupervised.attribute.Normalize;
import weka.filters.unsupervised.attribute.Remove;

import java.io.*;
import java.lang.String;
import java.net.URL;


class Lab implements Batch.Observer {

    private static final int NUM_THREADS = 1;

    private Attribute classAttribute;
    private int totalCount;


    void execute(String input, String output, ClassificationStrategy strategy) throws Exception {
        this.totalCount = 0;
        strategy.loadModel();
        Instances instances = load(input);
        if (instances != null) {

            String[] classNames = new String[instances.size()];
            Batch batch = splitInstances(instances, strategy, NUM_THREADS);

            System.out.format("Starting classification on %d threads.\n", NUM_THREADS);
            batch.run();
            batch.fillClassNames(classNames);

            System.out.println( ((double)batch.getAccurracy()/(double)instances.numInstances())*100 );

            write(output, classNames);
        }
    }

    Classifier loadClassifier(String filename) throws Exception {
        System.out.println("Loading classifier model '" + filename + "'.");
        ClassLoader classloader = Thread.currentThread().getContextClassLoader();
        InputStream is = classloader.getResourceAsStream(filename);

        return (Classifier) SerializationHelper.read(is);
    }

    String getClassName(int value) {
        String className = null;
        if (this.classAttribute != null) {
            className = this.classAttribute.value(value);
        }
        return className;
    }

    @Override
    public synchronized void instanceClassified(int count) {
        this.totalCount += count;
        System.out.format("%d instances classified.\n", this.totalCount);
    }

    private Batch splitInstances(Instances instances, ClassificationStrategy strategy, int number) {
        Batch batch;
        int batchSize = instances.size() / number;

        batch = new Batch(instances, strategy, 0, batchSize);
        batch.setObserver(this);

        return batch;
    }

    public Instances load(String input) throws Exception {
        Reader reader = null;
        Instances instances;
        try {

            reader = new BufferedReader(new FileReader(input));
            instances = new Instances(reader);
            cacheClassAttribute(instances);

        } finally {
            if (reader != null) {
                reader.close();
            }
        }
        return normalize(instances);
    }

    private Instances normalize(Instances instances) throws Exception {
        if (instances != null) {
            int[] rmIndices = getAttributeIndices(instances, new String[] {"SAMPLEID", "TRACKID"});

            Remove rm = new Remove();
            rm.setAttributeIndicesArray(rmIndices);
            rm.setInputFormat(instances);
            instances = Filter.useFilter(instances, rm);

            Normalize norm = new Normalize();
            norm.setScale(2.0);
            norm.setTranslation(-1.0);
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

    private void cacheClassAttribute(Instances instances) {
        this.classAttribute = instances.attribute("class");
        instances.setClass(this.classAttribute);
    }

    private void write(String output, String[] classNames) throws IOException {
        BufferedWriter writer = null;
        try {
            writer = new BufferedWriter(new FileWriter(output));
            for (String className: classNames) {
                writer.write(className);
                writer.newLine();
            }

        } finally {
            if (writer != null) {
                writer.close();
            }
        }
    }
}