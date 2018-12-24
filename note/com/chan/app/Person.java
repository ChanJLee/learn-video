package com.chan.app;

public class Person {

    public int age(String value) {
        try {
            return Integer.parseInt(value);
        } catch(Exception e) {
            return -1;
        } finally {
            System.out.println("age end");
        }
    }

    public int foo(int arg1, float arg2) {
        for (; arg1 < 10; ++arg1) {
            arg2 *= 2;
        }

        System.out.println("hello");
        return 0;
    }
}