/**
 * Registering a type as a singleton will create the type only once and return the same instance
 * no matter how many times resolve has been called 
 * 
 * ServiceA is registered as a singleton while ServiceB isn't 
 */

#include <iostream>
#include <memory>
#include <Container.h>

using namespace std;

class ServiceA {
public:
    ServiceA () {
        cout << "ServiceA CTOR" << endl;
    }

    ~ServiceA () {
        cout << "ServiceA DTOR" << endl;
    }
};

class ServiceB {
public:
    ServiceB () {
        cout << "ServiceB CTOR" << endl;
    }

    ~ServiceB () {
        cout << "ServiceB DTOR" << endl;
    }
};

int main(){
    cntnr::Container container;

    container.registerType<ServiceA>();//registers as singleton by default
    container.registerType<ServiceB>(false);//registered ServiceB not as a singleton
    
    std::shared_ptr<ServiceA> serviceA = container.resolve<ServiceA>();//'serviceA CTOR'
    std::shared_ptr<ServiceB> serviceB = container.resolve<ServiceB>();//'serviceB CTOR'
    
    serviceA = container.resolve<ServiceA>();
    serviceB = container.resolve<ServiceB>();//'serviceB CTOR' 'ServiceB DTOR'

    serviceA = nullptr;
    serviceB = nullptr;//'ServiceB DTOR'

    container.~Container();//'serviceA DTOR'

    return 0;
}