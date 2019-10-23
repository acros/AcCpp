#pragma once

#include <iostream>
#include <string>

using namespace std;

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

	void DoMoveAndForwardTest();

protected:

// 	class TestForForward
// 	{
// 	public:
// 		template<typename T> TestForForward(T&& m) : m_Name()
// 
// 		string m_Name;
// 	};


	class TestForMove
	{
	public:
		TestForMove()
		{
			m_Name = "DEFAULT";
			cout << "Class " << m_Name << " Construct" << endl;
		}

		TestForMove(const string& name)
		{
			m_Name = name;
			cout << "Class " << name << " Construct" << endl;
		}

		~TestForMove()
		{
			cout << "Class " <<m_Name << " Destroy" << endl;
		}

		TestForMove(const TestForMove& className)
		{
			m_Name = className.m_Name;
			m_Name.append("-Copy");

			cout << "Class " << m_Name << " copy constructed" << endl;
		}

		TestForMove operator=(const TestForMove& className)
		{
			m_Name = className.m_Name;
			m_Name.append("-OperateCopy");

			cout << "Class " << m_Name << " copy operation invoked" << endl;
		}

		void TestAlive()
		{
			cout << "Class " << m_Name << " test alive." << endl;
		}

		string m_Name;
	};

	TestForMove DoInnerCopyTest(const string& className);
	TestForMove&& DoInnerMoveTest(const string& className);

//	template<typename T> void DoInnerTemplate(T t);

	void TestForCommonParam(TestForMove t);
	void TestForRRefParam(const TestForMove&& t);

};