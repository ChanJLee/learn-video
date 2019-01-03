package com.chan.app;

import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;

public class Main {
    public static final String TAG = "Main";
    public static final int ID = 0xf0c0;

    private int mId = 10;
    private String mName = "jack";

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
    public static void main(String args[]) {
        System.out.println("hello world");
        foo();
        hugo();
    }

    private static void foo() {
        for (int i = 0; i < 10; ++i) {
            System.out.println("hello");
        }
    }

    private static void hugo(String... args) {
        try {
            System.out.println(args);
        } catch(Exception e) {
            System.out.println("exception");
        }
    }

    @Deprecated
    public <T> void echo(T o, List<?> list, List<? extends Main> list2) {
        System.out.println(o);
    }

    public void echo(String... args) {
        for (int i = 0; args != null && i < args.length; ++i) {
            System.out.println(args[i]);
        }
    }
}