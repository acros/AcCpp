#include "sfinae.h"
#include <iostream>
/*
	SFINAE:
		检测类内成员的存在
*/

///////////////////////////////////////////////////////

struct Foo {
	int bar;
};

template <class T>
struct is_pointer
{
	//General pointer
	template <class U>
	static char is_ptr(U*);

	//Member function pointer
	template <class X, class Y>	static char is_ptr(Y X::*);

	//Function pointer
	template <class U>
	static char is_ptr(U(*)());

	static double is_ptr(...);

	static T t;
	enum { value = sizeof(is_ptr(t)) == sizeof(char) };
};


void customIsPointerCheck() {
	typedef int* IntPtr;
	typedef int Foo::* FooMemberPtr;
	typedef int(*FuncPtr)();

	printf("IntPtr Is Pointer : %d\n", is_pointer<IntPtr>::value);        // prints 1

//	template <Foo,int>	static char is_ptr(int Foo::*);
	printf("Foo::* Is Pointer :%d\n", is_pointer<FooMemberPtr>::value);  // prints 1

	printf("FuncPtr Is Pointer :%d\n", is_pointer<FuncPtr>::value);       // prints 1

}



// 判断是否有 no_destroy 函数
template<typename T>
struct has_no_destroy {
//	template <typename C> static char test(typeof(&C::no_destroy));

	template<typename C>
	static char test(decltype(&C::no_destroy));

	template<typename C>
	static int32_t test(...);

	const static bool value = sizeof(test<T>(0)) == 1;
};


struct A {

};

struct B {
	void no_destroy() {}
};
struct C {
	int no_destroy;
};

struct D : B {

};

void SFINAE_Checker::testNoDestroy()
{
	/*
		IN type_traits:

		is_class
		is_pointer
		is_member_function_pointer
		is_const
	*/
	printf("\n");
	printf("SFINAE , Check if class has member of a specific name:\n");

	printf("A(Empty):%d\n", has_no_destroy<A>::value);

	const bool bHasDestroy = has_no_destroy<B>::value;
	printf("B(With Constructor) :%d\n", bHasDestroy);
	if (bHasDestroy)
	{
		printf("Is B has function member named no_destroy:%d\n", std::is_member_function_pointer<decltype(&B::no_destroy)>::value);
	}

	printf("C(Member variant):%d\n", has_no_destroy<C>::value);
	if (bHasDestroy)
	{
		printf("Is C has function member named no_destroy:%d\n", std::is_member_function_pointer<decltype(&C::no_destroy)>::value);
	}
	printf("D(Inherit from B):%d\n", has_no_destroy<D>::value);

	printf("\n");

	customIsPointerCheck();

	printf("\n");

}

