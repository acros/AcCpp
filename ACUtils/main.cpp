// ACUtils.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "tricks/convar_and_contravar.h"
#include "tricks/sfinae.h"

#include "tutorial/leptjson/leptjson.h"

using namespace std;

int main()
{
	ConvarAndContraVarSolver solver;
	solver.showExample();

	SFINAE_Checker	sfinae_checker;
	sfinae_checker.testNoDestroy();

	string jsonStr = "true";

	lept_value v;
	lept_parse(&v,jsonStr.c_str());

	system("pause");
}
