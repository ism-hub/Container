/**
 *  It possible to register the same type multiple times
 *  A call to 'resolveAll' will return a list with all of them.
 * 
 *  (to show why one will need that feature we implemented a mini chain of responsibility design pattern 
 *   baed on https://www.youtube.com/watch?v=jDX6x8qmjbA)
 */

#include <iostream>
#include <memory>
#include <Container.h>
#include <list>

using namespace std;

class Numbers{
public:
    int num1;
    int num2;
    string calculationWanted;
};

class INumbersHandler{
public:
    virtual bool canHandle(const Numbers& numbers) = 0;
    virtual void handle(Numbers& str) = 0;
    virtual ~INumbersHandler(){}
};

class AddNumbersHandler : public INumbersHandler {
public:
    bool canHandle(const Numbers& numbers) override{
        return numbers.calculationWanted == "Add";
    }

    virtual void handle(Numbers& numbers) override{
        cout << (numbers.num1 + numbers.num2) << endl;
    }
};

class SubstructNumbersHandler : public INumbersHandler{
public:
    bool canHandle(const Numbers& numbers) override{
        return numbers.calculationWanted == "Sub";
    }

    virtual void handle(Numbers& numbers) override{
        cout << (numbers.num1 - numbers.num2) << endl;
    }
};

class NumbersDispatcher{
    vector<shared_ptr<INumbersHandler>> _handlers;
public:
    NumbersDispatcher(vector<shared_ptr<INumbersHandler>>&& handlers) : _handlers(handlers){}

    void handleNumbers(Numbers& numbers){
        for(auto handler : _handlers)
            if(handler->canHandle(numbers))
                handler->handle(numbers);  
    }
};


shared_ptr<NumbersDispatcher> numbersDispatcherCTOR(vector<shared_ptr<INumbersHandler>> handlers){
    return make_shared<NumbersDispatcher>(std::move(handlers));
}

int main(){
    cntnr::Container container;

    container.registerType<AddNumbersHandler, INumbersHandler>();
    container.registerType<SubstructNumbersHandler, INumbersHandler>();

    container.registerType<NumbersDispatcher>(&numbersDispatcherCTOR);


    auto dispatcher = container.resolve<NumbersDispatcher>();//same as: make_shared<NumbersDispatcher>(container.resolveAll<INumbersHandler>())

    Numbers numbers{3,5,"Add"};
    Numbers numbers2{11,7,"Sub"};

    dispatcher->handleNumbers(numbers);//8
    dispatcher->handleNumbers(numbers2);//4

    return 0;
}