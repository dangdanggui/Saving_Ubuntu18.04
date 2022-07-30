#include <iostream>
using namespace std;    //


class Person{



    int m_A;//非静态成员变量  属于类的对喜上
    static int m_B;//静态成员变量   不属于类的对象，也就是即使有这句话，输出的sizeof(p)=1
};


void test01(){
    Person p;
    //空对象占用内存空间为:1,空对象是指class Person{};没有任何数据类型和函数
    //原因是：C++编译器会给每个空对象也分配一个字节空间，是为了区分空对象站内存空间的位置
    //每个空对象也应该有一个独一无二的内存地址
    //如果含有int m_A,则内存空间为4

    cout<<"sizeof p="<<sizeof(p)<<endl;


}


int main(){

}