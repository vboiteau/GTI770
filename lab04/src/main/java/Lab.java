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
            Thread[] threads = new Thread[NUM_THREADS];
            Batch[] batches = splitInstances(instances, strategy, NUM_THREADS);

            for (int i = 0; i < threads.length; i++) {
                threads[i] = new Thread(batches[i]);
                threads[i].start();
            }

            for (Thread thread : threads) {
                thread.join();
            }

            for (Batch batch : batches) {
                batch.fillClassNames(classNames);
            }

            System.out.println( ((double)batches[0].getAccurracy()/(double)instances.numInstances())*100 );

            write(output, classNames);
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

    private Batch[] splitInstances(Instances instances, ClassificationStrategy strategy, int number) {
        Batch[] batches = new Batch[number];
        int batchSize = instances.size() / number;

        int index;
        for (index = 0; index < batches.length; index++) {
            batches[index] = new Batch(instances, strategy, index, batchSize);
            batches[index].setObserver(this);
        }

        return batches;
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