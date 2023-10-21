package com.example.demo;

public class HelloJNI {
    static {
        System.loadLibrary("hello");
    }

    public native void sayHello();

    public static void main(String[] args) {
        new HelloJNI().sayHello();
    }
}
