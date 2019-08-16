
#pragma once

namespace cntnr{

template <class T>
const void* compiletimeTypeid(){
	//return __PRETTY_FUNCTION__;
	return (const void*) &compiletimeTypeid<T>;
}

	template <typename T, typename _ = void>
	struct is_vector {
	    static const bool value = false;
	};
	template <typename T>
	struct is_vector< T, typename std::enable_if<std::is_same<T, std::vector< typename T::value_type, typename T::allocator_type > >::value>::type>
	{
	    static const bool value = true;
	};

	template <typename T, typename _ = void>
	struct is_shared_ptr {
		static const bool value = false;
	};
	template <typename T>
	struct is_shared_ptr< T, typename std::enable_if<std::is_same<T, std::shared_ptr<typename T::element_type> >::value>::type>
	{
		static const bool value = true;
	};


};
