/**
 * Registering type with its creation function (his 'ctor')
 * The container will resolve the function parameters
 */

#include <iostream>
#include <memory>
#include <Container.hpp>

using namespace std;

class ServiceA{
public:
    const char* description = "ServiceA";
    ServiceA(){cout << "ServiceA CTOR" <<endl;}
};

class ServiceB {
    std::shared_ptr<ServiceA> _serviceA;
public:

    ServiceB(std::shared_ptr<ServiceA> serviceA) : _serviceA(serviceA){
        cout << "ServiceB CTOR" << endl;
    }

    void sayHi(){
        cout << "I use service named: " << _serviceA->description << endl;
    }
};

std::shared_ptr<ServiceB> ServiceBCTORfnc(shared_ptr<ServiceA> serviceA){
    return make_shared<ServiceB>(serviceA);
}

int main(){
     cntnr::Container container;

     container.registerType<ServiceA>();
     container.registerType<ServiceB>(&ServiceBCTORfnc);

     auto serviceB = container.resolve<ServiceB>();//'serviceA CTOR' 'ServiceB CTOR'

     serviceB->sayHi();//'I use service named: ServiceA'

    return 0;
}