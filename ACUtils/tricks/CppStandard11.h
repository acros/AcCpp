#pragma once

#include <iostream>
#include <string>

using namespace std;

class CppStandard0x11
{
public:

	/*
		move & perfect forwarding
		�ƶ����� & ����ת��

		�ƶ����壺���ڴ������Ȩ��һ������ת�Ƶ�����һ�����󣬸�Ч���ƶ������滻Ч�ʵ��µĸ��ƣ�
				������ƶ�������Ҫʵ���ƶ����캯����move constructor�����ƶ���ֵ�������move assignment operator����
		����ת��������һ������ģ�壬�ú���ģ����Խ����������Ͳ�����Ȼ�󽫲���ת��������Ŀ�꺯�����ұ�֤Ŀ�꺯�����ܵĲ����������봫�ݸ�ģ�庯����������ͬ��
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