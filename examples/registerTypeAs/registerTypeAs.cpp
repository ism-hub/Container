/**
 * Registers the type as an interface. 
 * (if the type has a constructor with parameters please check the 'registerWithCreateFunction' example)
 */

#include <iostream>
#include <memory>
#include <Container.h>

using namespace std;

class IPerson {
public:
    virtual void sayHi() = 0;
};

class Person : public IPerson{
public:
    void sayHi() override{
        cout << "Hi, happy?" << endl;
    }
};

int main(){
    cntnr::Container container;

    container.registerType<Person, IPerson>();

    auto person = container.resolve<IPerson>();

    person->sayHi();

    return 0;
}