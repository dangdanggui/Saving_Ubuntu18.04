#include <iostream>
using namespace std;

//const修饰成员函数
//成员函数家const后我们成为这个函数为常数

class Person{
    public:
    
    //this执政的本质是指针常量那个  指针常量指向是不可以修改的
    //在成员函数后面加const，修饰的是this指向，让指针指向的值也不可以修改，从而什么都不能修改，但是可以加入特殊变量，那么也可以修改
    void showPerson() const{
        

    }
    int m_A;
    mutable int m_B;

};

