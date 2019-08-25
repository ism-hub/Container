/**
 * Registering and resolving some type,
 * (if the type has a constructor with parameters please check the 'registerWithCreateFunction' example) 
 */

#include <iostream>
#include <memory>
#include <Container.hpp>

using namespace std;

class Person{
public:
    int age = 12;
};

int main(){
    cntnr::Container container;

    container.registerType<Person>();
    auto person = container.resolve<Person>();

    cout << person->age << endl;//12

    return 0;
}