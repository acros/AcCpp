#include "pch.h"
#include <iostream>

/*
	SFINAE:
		用来检测某个参数是否有某个方法
*/

// 判断是否有 no_destroy 函数
template<typename T>
struct has_no_destroy {
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

class SFINAE_Checker
{
public:

	static void testNoDestroy() {
		printf("A:%d\n", has_no_destroy<A>::value);
		printf("B:%d\n", has_no_destroy<B>::value);
		printf("C:%d\n", has_no_destroy<C>::value);
		printf("D:%d\n", has_no_destroy<D>::value);
	}
};


///////////////////////////////////////////////////////


template <class T>
struct is_pointer
{
	template <class U>
	static char is_ptr(U *);

	template <class X, class Y>
	static char is_ptr(Y X::*);

	template <class U>
	static char is_ptr(U(*)());

	static double is_ptr(...);

	static T t;
	enum { value = sizeof(is_ptr(t)) == sizeof(char) };
};

struct Foo {
	int bar;
};

void testTypeCheck() {
	typedef int * IntPtr;
	typedef int Foo::* FooMemberPtr;
	typedef int(*FuncPtr)();

	printf("%d\n", is_pointer<IntPtr>::value);        // prints 1
	printf("%d\n", is_pointer<FooMemberPtr>::value);  // prints 1
	printf("%d\n", is_pointer<FuncPtr>::value);       // prints 1
}
