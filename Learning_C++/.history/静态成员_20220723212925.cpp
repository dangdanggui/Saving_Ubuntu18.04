//this指针
#include <iostream>
using namespace std;    //

class Person{
public:
    Person(int age){
        this->age=age;
    }
    int age;
    void PersonAddAge(Person &p){
        this->age+=p.age;
    }


};
void test02(){
    Person p1(10);
    Person p2(10);
    p2.PersonAddAge(p1);
    cout<<"p2的年龄"<<p2.age<<endl;
}

void test01(){
        Person p1(18);
        cout<<"p1的年龄为"<<p1.age<<endl;
}

int main(){
    test01();
    test02();
}