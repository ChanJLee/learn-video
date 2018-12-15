package com.chan.app;

import java.util.List;

public class Main {
    public static final String TAG = "Main";
    public static final int ID = 0xf0c0;

    private int mId = 10;
    private String mName = "jack";

    public static void main(String args[]) {
        System.out.println("hello world");
    }

    private void foo() {
        for (int i = 0; i < 10; ++i) {
            System.out.println("hello");
        }
    }

    private void hugo() {
        try {
            System.out.println("hugo");
        } catch(Exception e) {
            System.out.println("exception");
        }
    }

    @Deprecated
    public <T> void echo(T o, List<?> list, List<? extends Main> list2) {
        System.out.println(o);
    }
}