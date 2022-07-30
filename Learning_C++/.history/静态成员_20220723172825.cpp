#include <iostream>
using namespace std;

class Person{
    public:
    static int m_A;
    //1.所有对象都共享同一份数据
    //2.编译阶段就分配内存
    //3.类内申明，类外初始化


};
int Person::m_A = 100;



void 
