//this指针
#include <iostream>
using namespace std;    //

class Person{
public:
    Person(int age){
        age=age;
    }
    int age;

};


void test01(){
        Person p1(18);
        cout<<"p1的年龄为"<<p1.age<<endl;
}