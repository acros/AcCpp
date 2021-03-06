// ACUtils.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "tutorial/leptjson/leptjson.h"
#include "tutorial/ClassLayoutCheck.h"

#include "tricks/convar_and_contravar.h"
#include "tricks/sfinae.h"
#include "tricks/CppStandard11.h"
#include "tricks/evil_trick.h"

#include "utils/MixStack.h"
#include "utils/GenericMemoryBuffer.h"

#include "algorithm/GenericAlgorithm.h"

#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

using namespace std;
using namespace Acros;

int main()
{
#ifdef _WINDOWS
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	ConvarAndContraVarSolver solver;
	solver.showExample();

	SFINAE_Checker	sfinae_checker;
	sfinae_checker.testNoDestroy();

#if LEPT_JSON_TEST
	string jsonStr = "true";
	lept_value v;
	lept_parse(&v,jsonStr.c_str());
#endif

	CppStandard0x11	cppTester;
	cppTester.exampleMove();

	test_parse();


	cout << "MPL_ARGS_SIZE(a, b, c, d, e, f, g):" << MPL_ARGS_SIZE(a, b, c, d, e, f, g) << endl;;
	assert(MPL_ARGS_SIZE(a, b, c, d) == 4);
	cout << endl;


	ClassLayoutCheck	layoutChecker;
	layoutChecker.printClassLayout();


	MemoryPoolAllocator<>* alloc = new MemoryPoolAllocator<>();
	MixStack<MemoryPoolAllocator<> > stack(alloc,8);

	ClassLayoutCheck* p = stack.Push<ClassLayoutCheck>();
	*p = layoutChecker;
	int* i = stack.Push<int>();
	*i = 14;
	float* f = stack.Push<float>();
	*f = 3.141593f;


	float* gf = stack.Pop<float>(1);
	int* gi = stack.Pop<int>(1);
	ClassLayoutCheck* gp = stack.Pop<ClassLayoutCheck>(1);


	cout << "Int from stack:" << *gi << endl;
	cout << "Float from stack:" << *gf << endl;

	GenericMemoryBuffer gBuff;

	GenericAlgorithm algorithm;

	algorithm.SampleTest(GenericAlgorithm::Quick);

	system("pause");
}
