#pragma once

//A stack can hold different kinds of data

namespace Acros
{
	/*
		Step:
		1. Implement custom stack
		2. Test performance
		3. Test rapidJson Stack performance
			https://github.com/Tencent/rapidjson/blob/v1.0.1/include/rapidjson/internal/stack.h
		4. Code Improve.
	*/
	class MixStack 
	{
		public:
			template<typename T> T* push(size_t t);
	};

}