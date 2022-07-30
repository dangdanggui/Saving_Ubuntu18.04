#include <iostream>
using namespace std;

class Person{
    public:
    static int m_A;
    //1.所有对象都共享同一份数据
    //2.编译阶段就分配内存
    //3.类内申明，类外初始化
    private:
    static int m_B;


};
int Person::m_A = 100;
int Person::m_B=200;


void test01(){
    Person p;
    cout<<p.m_A<<endl;

    Person p2;
    p2.m_A=200;

    cout<<p2.m_A<<endl;
    cout<<p.m_A<<endl;
}


int main(){
    test01();
}