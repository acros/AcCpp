#pragma once


class CppStandard0x11
{
public:

	/*
		move & perfect forwarding
		移动语义 & 完美转发

		移动语义：将内存的所有权从一个对象转移到另外一个对象，高效的移动用来替换效率低下的复制，
				对象的移动语义需要实现移动构造函数（move constructor）和移动赋值运算符（move assignment operator）。
		完美转发：定义一个函数模板，该函数模板可以接收任意类型参数，然后将参数转发给其它目标函数，且保证目标函数接受的参数其类型与传递给模板函数的类型相同。
	*/
	void exampleMove();

};