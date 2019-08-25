/**
 * How to register an existing instance.
 */

#include <iostream>
#include <memory>
#include <Container.hpp>

using namespace std;

class Person{
    string _name;
public:
    Person (string name) : _name(name){}

    void sayHi(){
        cout << "Hi, My name is " << _name << endl;
    }
};

int main(){
    cntnr::Container container;

    auto person = make_shared<Person>("Jeff");
    person->sayHi();//Hi, My name is Jeff

    container.registerInstance(person);

    auto personFromContainer = container.resolve<Person>();
    personFromContainer->sayHi();//Hi, My name is Jeff

    return 0;
}