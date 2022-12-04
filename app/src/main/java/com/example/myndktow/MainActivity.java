package com.example.myndktow;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.example.myndktow.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

private final static String TAG = MainActivity.class.getSimpleName();

    static {
        System.loadLibrary("native-lib"); //加载库文件
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
        Log.d(TAG, "onCreate: "+stringFromJNI());
    }

    /**
     * 下面是native的区域
     */
    public native String stringFromJNI();
    public native void testArrayAction(int count,String textInfo,int[] ints,String[]strs);
    public native void putObject(Student student, String str); // 传递引用类型，传递对象
    public native void insertObject(); // 凭空创建Java对象
    public native void testQuote(); // 测试引用
    public native void delQuote(); // 释放全局引用
    public void test01(View view) {
        int[] ints = new int[]{1,2,3,4,5,6}; // 基本类型的数组

        String[] strs = new String[]{"李小龙","李连杰","李元霸"}; // 对象类型的数组

         testArrayAction(99, "你好", ints, strs);

        for (int anInt : ints) {
            Log.d(TAG, "Java test01: anInt:" + anInt);
        }
    }

    public void test02(View view) {
        Student student = new Student(); // Java new
        student.name = "史泰龙";
        student.age = 88;
        putObject(student, "九阳神功");
    }

    public void test03(View view) {
        insertObject();
    }

    public void test04(View view) {
        testQuote();
    }

    public void test05(View view) {
        delQuote();
    }
}