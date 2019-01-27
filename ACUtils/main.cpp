// ACUtils.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "tricks/convar_and_contravar.h"
#include "tricks/sfinae.h"

#include "tutorial/leptjson/leptjson.h"

#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

using namespace std;

int main()
{
#ifdef _WINDOWS
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	ConvarAndContraVarSolver solver;
	solver.showExample();

	SFINAE_Checker	sfinae_checker;
	sfinae_checker.testNoDestroy();

	string jsonStr = "true";

	lept_value v;
	lept_parse(&v,jsonStr.c_str());


	test_parse();

	system("pause");
}
