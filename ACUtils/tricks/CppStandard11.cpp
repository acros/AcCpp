#include "CppStandard11.h"
#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class DynamicArray
{
public:
	explicit DynamicArray(int size) :
		m_size{ size }, 
		m_array{ new T[size] }
	{
		cout << "Constructor: dynamic array is created!\n";
	}

	virtual ~DynamicArray(){
		delete[] m_array;
		cout << "Destructor: dynamic array is destroyed!\n";
	}

	DynamicArray(const DynamicArray& rhs) :
		m_size{ rhs.m_size }
	{
		m_array = new T[m_size];
		for (int i = 0; i < m_size; ++i)
			m_array[i] = rhs.m_array[i];
		cout << "Copy constructor: dynamic array is created!\n";
	}

	DynamicArray& operator=(const DynamicArray& rhs)
	{
		cout << "Copy assignment operator is called\n";
		if (this == &rhs)
			return *this;

		delete[] m_array;

		m_size = rhs.m_size;
		m_array = new T[m_size];
		for (int i = 0; i < m_size; ++i)
			m_array[i] = rhs.m_array[i];

		return *this;
	}

	T& operator[](int index)
	{
		return m_array[index];
	}

	const T& operator[](int index) const
	{
		return m_array[index];
	}

	int size() const { return m_size; }

	//Move Constructor
	DynamicArray(DynamicArray&& rhs)
		: m_size(rhs.size())
		, m_array(rhs.m_array)
	{
		rhs.m_size = 0;
		rhs.m_array = nullptr;
		cout << "Move constructor! Dynamic array moved." << endl;
	}

	//Move Assign
	DynamicArray& operator=(DynamicArray&& rhs)
	{
		cout << "Move assignment in Dynamic Array" << endl;
		if (this == &rhs)
			return *this;

		delete[] m_array;
		m_size = rhs.m_size;
		m_array = rhs.m_array;
		rhs.m_size = 0;
		rhs.m_array = nullptr;

		return *this;
	}

private:
	T* m_array;
	int m_size;
};

DynamicArray<int> arrayFactor(int size)
{
	DynamicArray<int> arr{ size };
	return arr;
}


void CppStandard0x11::exampleMove()
{
	//Example
	DynamicArray<int> arr = arrayFactor(10);

	int lValue = 200;
	int& refValue = lValue;
	int&& rValue = 100;
	
	cout << "rValue:" << rValue << endl;

	cout << "lValue:" << lValue << endl;
//	int&& rValue2 = lValue;	//Error assign
//	int&& rT = refValue;	//Error assign

	//std::move  - ����ֵת��Ϊ��ֵ
	int&& rValue2 = std::move(lValue);
	cout << "rValue2:" << rValue2 << endl;
	rValue2 = 150;
	cout << "lValue:" << lValue << endl;


	//
	cout << "-- Test Perfect Forward" << endl;

}


void CppStandard0x11::DoMoveAndForwardTest()
{
//	TestForMove dummyClass = DoInnerMoveAndForwardTest("CommonClass");

//	TestForMove& dummyMoveClass = DoInnerMoveAndForwardTest("Ref-Class");


	TestForMove dummyClass = DoInnerCopyTest("Common-Class");

//	TestForMove&& dummyMoveClass = DoInnerMoveTest("RightRef-MovedClass");
	TestForMove&& dummyRRefClass = TestForMove("Right-Ref-Class");

	TestForRRefParam(std::forward<TestForMove>(dummyRRefClass));
	TestForCommonParam(std::forward<TestForMove>(dummyRRefClass));

// 	cout << "*VECTOR*" << endl;
// 	std::vector<TestForMove> ds(3);
// 	ds.emplace_back("InStock");

}

CppStandard0x11::TestForMove CppStandard0x11::DoInnerCopyTest(const string& className)
{
	TestForMove dummy(className);
	return dummy;
}

CppStandard0x11::TestForMove&& CppStandard0x11::DoInnerMoveTest(const string& className)
{
	TestForMove dummy(className);
	return std::move(dummy);

}

void CppStandard0x11::TestForCommonParam(TestForMove t)
{
	cout << "Test for common param" << endl;

}

void CppStandard0x11::TestForRRefParam(const TestForMove&& t)
{
	cout << "Test for R-Ref param" << endl;
}


// template<typename T> void CppStandard0x11::DoInnerTemplate(T t)
// {
// 	cout << "Current version is r-ref : "  + std::is_rvalue_reference<T>().value<< endl;
// }


template <typename T>
typename remove_reference<T>::type&& move_in_stl(T&& param)
{
	using ReturnType = typename remove_reference<T>::type&&;
	return static_cast<ReturnType>(param);
}

template<typename T>
T&& forward_in_stl(typename remove_reference<T>::type& param)
{
	return static_cast<T&&>(param);
}