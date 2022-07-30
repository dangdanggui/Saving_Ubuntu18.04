//this指针
#include <iostream>
using namespace std;    //

class Person{
public:
    Person(int age){
        this->age=age;
    }
    int age;

};
void test02(){
    Person p1(10);
    Person p2(10);
}

void test01(){
        Person p1(18);
        cout<<"p1的年龄为"<<p1.age<<endl;
}

int main(){
    test01();

}