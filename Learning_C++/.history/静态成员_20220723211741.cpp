#include <iostream>
using namespace std;    //


class Person{




};


void test01(){
    Person p;
    //空对象占用内存空间为:1
    //原因是：C++编译器会给每个空对象也分配一个字节空间，是为了区分空对象站内存空间的位置

    cout<<"sizeof p="<<sizeof(p)<<endl;


}


int main(){

}