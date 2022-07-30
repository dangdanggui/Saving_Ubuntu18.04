#include <iostream>
using namespace std;

class Person{
    public:
    static void func(){
        m_A=100;
        cout<<"static void func调用"<<endl;
    }
    static int m_A;
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

