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
}