public class Hello {

    public static void main(String[] args) {
        System.out.println("java main start");
        //System.out.println(args[0]);
        new Hello().nativeThreadId();
        showThread();
        newThread();
        printAllThreads();
        System.out.println("java main exit");
    }

    public static void showThread() {
        Thread t = Thread.currentThread();
        System.out.println("name:" + t.getName() + ", id:" + t.getId());
    }

    public static void newThread() {
        new Thread(new Runnable() {
            public void run() {
                System.out.println("---- new thread ----");
                showThread();
                new Hello().nativeThreadId();
            }
        }).start();
    }

    public static Thread[] getAllThreads() {
        ThreadGroup group = Thread.currentThread().getThreadGroup();
        ThreadGroup topGroup = group;
        while (group != null) {
            topGroup = group;
            group = group.getParent();
        }
        int estimatedSize = topGroup.activeCount() * 2;
        Thread[] slackList = new Thread[estimatedSize];
        int actualSize = topGroup.enumerate(slackList);
        Thread[] threads = new Thread[actualSize];
        System.arraycopy(slackList, 0, threads, 0, actualSize);
        return threads;
    }

    public static void printAllThreads() {
        Thread[] threads = getAllThreads();
        for (Thread t : threads) {
            System.out.println("--name:" + t.getName() + ", id:" + t.getId());
        }
    }
    
    // ----- native method -----

    public native void nativeThreadId();

}

