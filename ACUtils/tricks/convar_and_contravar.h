#pragma once

#include <iostream>

/*
	https://quuxplusone.github.io/blog/2019/01/20/covariance-and-contravariance/
*/

struct Animal {
public:
	virtual void makeNoise() = 0;

	//C++11 - default: 编译器会自动生成函数体。 此处析构inline。
	virtual ~Animal() = default;
};

struct Cat : Animal {
public:
	void makeNoise()override
	{
		puts("Meow!");
	}
};


struct Horse : Animal {
public:
	void makeNoise()override
	{
		puts("Neigh.");
	}

	virtual void ride()
	{
		puts("Okay, sure.");
	}
};

//////////////////////////////////////////////////////////////////////////

struct AnimalBreeder {
public:
	virtual Animal* produce() = 0;
};

struct CatBreeder : AnimalBreeder {
public:
	Animal* produce()override
	{
		return new Cat();
	}

	/*
	//返回值不影响重载，因此这里也可以直接返回Cat类
	Cat* produce()override		{return new Cat();}
	*/
};

struct HorseBreeder : AnimalBreeder {
public:
	Animal* produce()override
	{
		return new Horse();
	}

};

class ConvarAndContraVarSolver
{
public:
	void showExample();


#if _MSVC_LANG >= 201703L
	template<auto> struct dolittle {};
	template<int> struct testDolitte{};
//	template<Horse> struct hackenbush {};

	template<template<auto> class>  struct you {};
#endif
};