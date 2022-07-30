#include <iostream>
using namespace std;

class Person{
    public:
    static void func(){
        cout<<"static void func调用"<<endl;
    }
};

//访问静态函数的方法
//静态成员函数只能访问静态函数对象


void test01(){
    //1.通过对象访问
    Person p;
    p.func();
    //2.通过类名访问
    Person::func();
}

