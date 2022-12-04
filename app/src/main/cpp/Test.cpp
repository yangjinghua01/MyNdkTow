#include <jni.h>
#include <string>
#include "android/log.h" //导入安卓的日志包
#include "iostream"
#define  TAG "YJH"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
int age = 99;
void show(){
LOGE("调用show函数");
}
