import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;

public class Main {
    private static void testFeature() {
        FutureTask<Integer> task = new FutureTask<Integer>(new Callable<Integer>() {
            @Override
            public Integer call() throws Exception {
                try {
                    for (int i = 0; i < 10; ++i) {
                        System.out.println("times " + i);
                        Thread.sleep(1000);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }

                return 12;
            }
        });

        new Thread(task).start();
        try {
            System.out.println("feature result: " + task.get());
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }
    }

    private static void testInterrupt() {
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {

                while (true) {
                    try {
                        Thread.sleep(1000);
                        System.out.println("fuck");
                    } catch (InterruptedException e) {
                        System.out.println("InterruptedException");
                        Thread.currentThread().interrupt();
                    }
                }
            }
        });
        thread.start();
        try {
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        thread.interrupt();
    }

    private static void testSemaphore() {
        final CountingSemaphoreBuffer countingSemaphoreBuffer = new CountingSemaphoreBuffer(10);
        for (int i = 0; i < 20; ++i) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        countingSemaphoreBuffer.add(new Person("fuck"));
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }
    }

    public static void main(String[] args) {
        //testFeature();
        //testInterrupt();
        testSemaphore();
    }
}
