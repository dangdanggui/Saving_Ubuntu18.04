#include <iostream>
using namespace std;    //


class Person{



    int m_A;//非静态成员变量

};


void test01(){
    Person p;
    //空对象占用内存空间为:1
    //原因是：C++编译器会给每个空对象也分配一个字节空间，是为了区分空对象站内存空间的位置
    //每个空对象也应该有一个独一无二的内存地址
    cout<<"sizeof p="<<sizeof(p)<<endl;


}


int main(){

}