#include <iostream>
using namespace std;

class Person{
    public:
    static void func(){
        cout<<"static void func调用"<<endl;
    }
};

void test01(){
    //1.通过对象访问
    Person p;
    p.func();

    //2.通过类名访问


}