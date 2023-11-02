#include <jni.h>
#include "com_example_demo_HelloJNI.h"
#include <stdio.h>

int sum(int matrix[3][3]) {
    int sum = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            sum += matrix[i][j];
        }
    }
    return sum;
}

JNIEXPORT void JNICALL Java_com_example_demo_HelloJNI_sayHello
  (JNIEnv *env, jobject thisObj) {
   printf("Hello World from C!\n");
   return;
}

JNIEXPORT jint JNICALL Java_com_example_demo_HelloJNI_sumMatrix
  (JNIEnv *env, jobject thisObj, jobjectArray matrix) {
    int c_matrix[3][3];
    int size = 3;
    for (int i = 0; i < size; i++) {
        jintArray row = (jintArray)(*env)->GetObjectArrayElement(env, matrix, i);
        jint *rowElements = (*env)->GetIntArrayElements(env, row, 0);
        for (int j = 0; j < size; j++) {
            c_matrix[i][j] = rowElements[j];
        }
        (*env)->ReleaseIntArrayElements(env, row, rowElements, 0);
    }
    return sum(c_matrix);
}