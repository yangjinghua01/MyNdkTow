#include <jni.h>
#include <string>
#include "android/log.h" //导入安卓的日志包

#define  TAG "YJH"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myndktow_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
// jint == int
//jstring  == string
// jintArray == int []
//jobjectArray == 引用类型对象 例如String【】 test【】 student【】person【】
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myndktow_MainActivity_testArrayAction(JNIEnv *env, jobject thiz, jint count,
                                                       jstring text_info, jintArray ints,
                                                       jobjectArray strs) {
    // TODO: implement testArrayAction()
    int countInt = count; //jint本质上是int 可以用int接受
    LOGE("参数1%d", countInt);
    const jchar *jchar1 = env->GetStringChars(text_info, NULL);
    LOGE("参数2%s", jchar1);
    int *jinarray = env->GetIntArrayElements(ints, NULL);
// java 层数组的长度
// jsize GetArrayLength （jarray array） jintArray ints 可以放入 jarray
    jsize size = env->GetArrayLength(ints);
    for (int i = 0; i < size; ++i) {
        *(jinarray + i) += 100;
        LOGE("参数：jint【】：", jinarray + i);
    }
    /**
     * 0: 刷新java数组并释放C++层数组
     * JNI_COMMIT: 只提交刷新java数组，不释放C++层数组
     * JNI_ABORT:  只释放C++层数组
     */
    env->ReleaseIntArrayElements(ints, jinarray, 0);
//    jobjectArray 代表是java的引用类型数组，不一样
    jsize jsize1 = env->GetArrayLength(strs);
    for (int j = 0; j < jsize1; ++j) {
        jstring jstring1 = static_cast<jstring>(env->GetObjectArrayElement(strs, j));
        const char *jobjCharp = env->GetStringUTFChars(jstring1, NULL);
        LOGE("参数4%s", jobjCharp);
//      释放
        env->ReleaseStringUTFChars(jstring1, jobjCharp);
    }

}extern "C"
JNIEXPORT void JNICALL
Java_com_example_myndktow_MainActivity_putObject(JNIEnv *env,
                                                 jobject thiz,
                                                 jobject student,
                                                 jstring str) {
    const char *strChar = env->GetStringUTFChars(str, NULL);
    LOGD("strChar%s", strChar);
    env->ReleaseStringUTFChars(str, strChar);
//    1.找到类
    jclass StudnetClass = env->FindClass("com/example/myndktow/Student");//第一种方式
    jclass StudnetClass2 = env->GetObjectClass(student);
//    2。student类里面的函数柜责
    jmethodID setname = env->GetMethodID(StudnetClass, "setName", "(Ljava/lang/String;)V");
    jmethodID getname = env->GetMethodID(StudnetClass, "getName", "()Ljava/lang/String;");
    jmethodID showInfo = env->GetStaticMethodID(StudnetClass, "showInfo", "(Ljava/lang/String;)V");
    //调用 setName
    jstring val = env->NewStringUTF("AAA");
    env->CallVoidMethod(student, setname, val);
//    调用getName
    jstring str1 = static_cast<jstring>(env->CallObjectMethod(student, getname));
    const char *chars = env->GetStringUTFChars(str1, NULL);
    LOGE("getname的值：%s", chars);
    jstring jstring1 = env->NewStringUTF("静态方法，C++");
    env->CallStaticVoidMethod(StudnetClass2, showInfo, jstring1);

}extern "C"
JNIEXPORT void JNICALL
Java_com_example_myndktow_MainActivity_insertObject(JNIEnv *env, jobject thiz) {
//1.通过包名+类名的方式拿到Studen class  凭空创建对象
    const char *stu = "com/example/myndktow/Student";
    jclass stuclass = env->FindClass(stu);
//2. 通过studentclass 实力化对象  用C++去new
    jobject Student = env->AllocObject(stuclass); //AllocObject 只实例化对象，不会调用对象的构造函数
//    方法规则的定义
    jmethodID  setName = env->GetMethodID(stuclass, "setName", "(Ljava/lang/String;)V");
    jmethodID  setAge = env->GetMethodID(stuclass,"setAge", "(I)V");
    //调用
    jstring  s = env->NewStringUTF("yjh");
    env->CallVoidMethod(Student,setName,s);
    env->CallVoidMethod(Student,setAge,99);
    const char * Person = "com/example/myndktow/Person";
    jclass  PersonClass = env->FindClass(Person);
    jobject perobj = env->AllocObject(PersonClass);
    //setStudent 此函数的签名 规则
    jmethodID  setStudent = env->GetMethodID(PersonClass,"setStudent", "(Lcom/example/myndktow/Student;)V");
    env->CallVoidMethod(perobj,setStudent ,Student);
//    释放的环节
    env->DeleteLocalRef(stuclass);
    env->DeleteLocalRef(PersonClass);
    env->DeleteLocalRef(perobj);
    env->DeleteLocalRef(Student);


}
jclass dogClass;//局部引用
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myndktow_MainActivity_testQuote(JNIEnv *env, jobject thiz) {
    if (NULL == dogClass) {
        /*const char * dogStr = "com/derry/as_jni_project/Dog";
        dogClass = env->FindClass(dogStr);*/

        // 升级全局引用： JNI函数结束也不释放，反正就是不释放，必须手动释放   ----- 相当于： C++ 对象 new、手动delete
        const char * dogStr = "com/example/myndktow/Dog";
        jclass temp = env->FindClass(dogStr);
        dogClass = static_cast<jclass>(env->NewGlobalRef(temp)); // 提升全局引用
        // 记住：用完了，如果不用了，马上释放，C C++ 工程师的赞美
        env->DeleteLocalRef(temp);
    }

    // <init> V  是不会变的

    // 构造函数一
    jmethodID init = env->GetMethodID(dogClass, "<init>", "()V");
    jobject dog = env->NewObject(dogClass, init);

    // 构造函数2
    init = env->GetMethodID(dogClass, "<init>", "(I)V");
    dog = env->NewObject(dogClass, init, 100);


    // 构造函数3
    init = env->GetMethodID(dogClass, "<init>", "(II)V");
    dog = env->NewObject(dogClass, init, 200, 300);

    // 构造函数4
    init = env->GetMethodID(dogClass, "<init>", "(III)V");
    dog = env->NewObject(dogClass, init, 400, 500, 600);

    env->DeleteLocalRef(dog); // 释放
    // dogClass = NULL; // 是不是问题解决了，不能这样干（JNI函数结束后，还怎么给你释放呢）

    // 这样就解决了
    /*env->DeleteGlobalRef(studentClass);
    studentClass = NULL;*/
}
extern int age;
extern void show();
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myndktow_MainActivity_delQuote(JNIEnv *env, jobject thiz) {
if (dogClass!=NULL){
    LOGE("全局引用释放完毕");
    env->DeleteGlobalRef(dogClass);
}
show();
}