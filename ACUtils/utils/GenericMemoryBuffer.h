#pragma once

#include "Allocator.h"

/*
	https://github.com/Tencent/rapidjson/blob/v1.0.1/include/rapidjson/memorybuffer.h

	https://zhuanlan.zhihu.com/p/20029820
*/

namespace Acros
{
	template <typename Allocator = CrtAllocator>
	struct GenericMemoryBuffer {
		typedef char Ch; // byte

		GenericMemoryBuffer(Allocator* allocator = 0, size_t capacity = kDefaultCapacity) 
			: stack_(allocator, capacity) 
		{}

		void Put(Ch c) { 
			//In C++11 , can use stack_.Push<Ch>()
			*stack_.template Push<Ch>() = c; 
		}

		void Flush() {}

		void Clear() { stack_.Clear(); }
		void ShrinkToFit() { stack_.ShrinkToFit(); }
		Ch* Push(size_t count) { return stack_.template Push<Ch>(count); }
		void Pop(size_t count) { stack_.template Pop<Ch>(count); }

		const Ch* GetBuffer() const {
			return stack_.template Bottom<Ch>();
		}

		size_t GetSize() const { return stack_.GetSize(); }

		static const size_t kDefaultCapacity = 256;
		mutable MixStack<Allocator> stack_;
	};

	typedef GenericMemoryBuffer<> MemoryBuffer;

	/*
	//TODO: ???
	//! Implement specialized version of PutN() with memset() for better performance.
	template<>
	inline void PutN(MemoryBuffer& memoryBuffer, char c, size_t n) {
		std::memset(memoryBuffer.stack_.Push<char>(n), c, n * sizeof(c));
	}
	*/
}