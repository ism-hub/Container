# Container
Dependency Injection container in C++.
Not using RTTI - can run on the ESP8266 (can compile with the -fno-rtti flag on gcc) (see 'Remarks' section).

# Overview 
### Register And Resolve Types
You can register types as singletons (resolve will return the same instance every time)
```C++
container.registerType<Person>(); //registering Person with the default CTOR and as a singleton
//container.registerType<Person>(false);//Registering person not as a singleton
auto person = container.resolve<Person>();// will return a Person instance 
```
### Register And Resolve Types With Creation Function
If you want to register types which depends on other parameters and you want the Container to instantiate the type for you and resolve its parameters you need to define a creation function and register the type with it.
```C++
//the container will resolve serviceA and serviceC and inject them on ServiceB creation.
std::shared_ptr<ServiceB> ServiceBCTORfnc(shared_ptr<ServiceA> serviceA, shared_ptr<ServiceC> serviceC){
    return make_shared<ServiceB>(serviceA, serviceC);
}

//registering ServiceB with its creation function
container.registerType<ServiceB>(&ServiceBCTORfnc);

//the container will call ServiceBCTORfnc and resolve all its dependencies,
auto serviceB = container.resolve<ServiceB>();
```

### Register Type As Its Interface
You can register a type as an interface and resolve it later via the interface type -
```C++
container.registerType<Person, IPerson>(); //registering Person as Iperson
std::shared_ptr<IPerson> person = container.resolve<IPerson>();// will instantiate Person but will return an IPerson pointer.
```

### Register Instance
You can register instance of a type
```C++
auto person = make_shared<Person>();
container.registerInstance(person);
auto personFromContainer = container.resolve<Person>();//will return the instance we have registered
```

### Register Multiple Types With The Same Interface
You can resolve all of them at once - 
```C++
container.registerType<HandlePlus, IHandler>();
container.registerType<HandleMinus, IHandler>();
vector<shared_ptr<IHandler>> handlers = container.resolveAll<IHandler>();//will resolve HandlePlus and HandleMinus.
```

# Remarks
In order to avoid the use of RTTI we use this function to identify a type - 
```C++
template <class T>
const void* compiletimeTypeid(){
	return (const void*) &compiletimeTypeid<T>;
}
``` 
