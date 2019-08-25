
#pragma once

#include <vector>
#include <memory>
#include <map>
#include <utility>
#include "utils/utils.hpp"
#include <functional>

namespace cntnr {

class Container {

protected:

	template<class InstanceType, class Ret, class ...Deps>
	using class_member_function = Ret(InstanceType::*)(std::shared_ptr<Deps> ...);

	template<class Ret ,class ...Deps>
	using ctor_function = Ret(*)(Deps ...);

	class RegisteredType {
	public:
		using CreateFn = std::function<std::shared_ptr<void>(RegisteredType&)>;
	private:
		CreateFn _create;
	public:
		const void* _compiletimeType;
		bool isSingelton;
		std::shared_ptr<void> obj = nullptr;

		RegisteredType(const void* typeinfo, CreateFn createFn, bool isSingelton) :
			_create(createFn), _compiletimeType(typeinfo), isSingelton(isSingelton)  {
		}

		std::shared_ptr<void> create(){
			return _create(*this);
		}

		 ~RegisteredType() {}

		std::shared_ptr<void> getInstance() {
			return _create(*this);
		}

	};

	std::map<const void*, std::vector<RegisteredType> > typeRegisteredTypeMap;

public:

	Container(){}
	 ~Container(){}

	/**
	 * Registers instance of an object 
	 * (when asking for type M you will get that instance)
	 */
	template <class T>
	void registerInstance(std::shared_ptr<T> instance){
		const void* id = compiletimeTypeid<T>();

		RegisteredType::CreateFn create = [](RegisteredType& regType) {
					return regType.obj;
				};

		RegisteredType regType(id, create, true);
		regType.obj = instance;
		typeRegisteredTypeMap[id].push_back(std::move(regType));
	}

	/** 
	 * Registers object of type T as type M
	 * (using the default ctor to initiate)
	 * 
	 * isSingleton - if 'false' resolve will create a new object on each call
	 * 				else resolve will create the object only once and return it on each call.
	*/
	template<class T, class M = T>
	void registerType(bool isSingleton = true) {
		ctor_function<std::shared_ptr<M>> createFnc = [](){return std::shared_ptr<M>(new T());};
		return registerType<M>(createFnc, isSingleton);
	}

	/**
	 * Registering type with a creation function. 
	 * When we want to create the type we call the function.
	 * 
	 * ctorFnc - a function that creates and return a shared_ptr to the object.
	 * 			The function can take variable number of parameters.
	 * 
	 * isSingleton - if 'true' resolve will return the same created object on each call (meaning we ctorFnc will be called only once)
	 * 				 else each time we call resolve (with that type) we will call ctorFnc.
	 */
	template< class Ret, class ...Deps >
	void registerType(ctor_function<std::shared_ptr<Ret>, Deps ...> ctorFnc, bool isSingleton = true) {
		const void* compileType = compiletimeTypeid<Ret>();
		RegisteredType::CreateFn create = [&, ctorFnc](RegisteredType& regType) {
			std::shared_ptr<Ret> obj = std::static_pointer_cast<Ret>(regType.obj);
			if(regType.obj == nullptr) {
				 obj = this->ctorFunctionResolver(ctorFnc);
				if(regType.isSingelton)
					regType.obj = obj;
			}
			return obj;
		};
		typeRegisteredTypeMap[compileType].push_back(RegisteredType(compileType, create, isSingleton));//adding to the .text @@@@
	}


	/**
	 * It is possible to register the same type multiple times (one of the examples shows why we may want that option).
	 * This function will return a list with all the instances the user registered for that type 
	 */
	template<class T>
	std::vector<std::shared_ptr<T>> resolveAll() {
		std::vector<std::shared_ptr<T>> t_vector;
		if (typeRegisteredTypeMap.find(compiletimeTypeid<T>())== typeRegisteredTypeMap.end())	//we didnt register it
			return t_vector;

		for (auto &t : typeRegisteredTypeMap[compiletimeTypeid<T>()]) // access by reference to avoid copying
		{
			t_vector.push_back(std::static_pointer_cast<T>(t.create()));
		}
		return t_vector;
	}

private:

	template<class T, typename std::enable_if<is_shared_ptr<T>::value, void>::type* = nullptr>
	T resolve() {
		return resolve<typename T::element_type>();
	}

	template<class T,typename std::enable_if<is_vector<T>::value,  void>::type* = nullptr>
	T resolve() {
		return resolveAll<typename T::value_type::element_type >();
	} 

public:
	/**
	 * Returns an instance of the type T (if type T was registered)
	 * If the type wasn't registered returns nullptr.
	 */
	template<class T, typename std::enable_if<(!is_vector<T>::value && !is_shared_ptr<T>::value), void>::type* = nullptr>
	std::shared_ptr<T> resolve() {
		if (typeRegisteredTypeMap.find(compiletimeTypeid<T>())== typeRegisteredTypeMap.end()) //we didnt register it
			return nullptr;
		return std::static_pointer_cast<T>(typeRegisteredTypeMap[compiletimeTypeid<T>()].front().create());
	}

	/**
	 * Unregisters type from the container (resolve<T> wont find that type)
	 */
	template <class T>
	void unregister(){
		typeRegisteredTypeMap.erase(compiletimeTypeid<T>());//adding to the .text @@@@
	}

	/**
	 * Input: instance and a pointer to a member function 
	 * Output: the container will resolve all the function parameters and call the function 
	 */
	template<class InstanceType, class Ret, class ...Deps>
	Ret memberFunctionResolver(InstanceType& instance, class_member_function<InstanceType, Ret, Deps...> ctorFnc){
		return (instance.*ctorFnc)( resolve<Deps>() ...);
	}

	/**
	 * Input: pointer to a function with signature of std::shared_ptr<Ret>(...)
	 * Output: the container will call and resolve all the parameters of the funcion 
	 */
	template <class Ret, class ...Deps>
	std::shared_ptr<Ret> ctorFunctionResolver(ctor_function< std::shared_ptr<Ret>, Deps ...> ctorFunction){
		return ctorFunction(resolve<Deps>() ...);
	}
};

} /* namespace moduleFramework */

