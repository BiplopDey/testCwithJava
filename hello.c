#include <jni.h>
#include "HelloJNI.h"
#include <stdio.h>

JNIEXPORT void JNICALL Java_com_example_demo_HelloJNI_sayHello
  (JNIEnv *env, jobject thisObj) {
   printf("Hello World from C!\n");
   return;
}
