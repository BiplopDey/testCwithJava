package com.example.demo;

public class HelloJNI {
    static {
        System.loadLibrary("hello");
    }

    public native void sayHello();
    public native int sumMatrix(int[][] matrix);
}
