// ACUtils.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "tricks/convar_and_contravar.h"
#include "tricks/sfinae.h"

int main()
{
	ConvarAndContraVarSolver solver;
	solver.showExample();

	SFINAE_Checker	sfinae_checker;
	sfinae_checker.testNoDestroy();


	system("pause");
}
