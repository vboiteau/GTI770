import weka.core.Instances;

/**
 * Created by lodonoughue on 2017-08-02.
 */
class Batch implements Runnable {

    private final Instances instances;
    private final ClassificationStrategy strategy;
    private final int startIndex;
    private final int size;
    private final String[] classNames;
    private final String[] currentClassNames;
    private Observer observer;

    Batch(Instances instances, ClassificationStrategy strategy, int index, int batchSize) {
        this.startIndex = index * batchSize;

        if (this.startIndex + batchSize * 2 < instances.size()) {
            this.size = batchSize;
        } else {
            this.size = instances.size() - this.startIndex;
        }

        this.classNames = new String[this.size];
        this.currentClassNames = new String[this.size];

        this.instances = new Instances(instances, this.startIndex, this.size);
        this.strategy = strategy;
    }

    @Override
    public void run() {
        try {
            int count = 0;
            for (int i = 0; i < this.classNames.length; i++) {
                this.classNames[i] = this.strategy.classify(this.instances.get(i));
                this.currentClassNames[i] = instances.classAttribute().value((int)instances.get(i).classValue());
                count++;

                if (this.observer != null && (i + 1) % 500 == 0) {
                    this.observer.instanceClassified(count);
                    count = 0;
                }
            }
            if (this.observer != null) {
                this.observer.instanceClassified(count);
            }

        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
    }

    void setObserver(Observer obs) {
        this.observer = obs;
    }

    void fillClassNames(String[] classNames) {
        System.arraycopy(this.classNames, 0, classNames, this.startIndex, this.size);
    }

    public int getAccurracy() {

        int count = 0;

        for (int i = 0; i < this.size; i++)
        {
            if (this.classNames[i].equals(this.currentClassNames[i]))
                count++;
        }

        return count;
    }

    interface Observer {
        void instanceClassified(int count);
    }

}
