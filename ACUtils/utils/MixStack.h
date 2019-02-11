#pragma once

//A stack can hold different kinds of data
#include <cassert>

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

	template<typename Allocator>
	class MixStack
	{
	public:
		MixStack(Allocator* allocator, size_t stackCapacity)
			: allocator_(allocator)
			, ownAllocator_(0)
			, stack_(0)
			, stackTop_(0)
			, stackEnd_(0)
			, initialCapacity_(stackCapacity)
		{
			assert(stackCapacity > 0);
		}

		~MixStack()
		{
			Destroy();
		}

		void Clear()
		{
			stackTop_ = stack_;
		}

		void ShrinkToFit() {
			if (Empty())
			{
				Allocator::Free(stack_);
				stack_ = stackEnd_ = stackTop_ = nullptr;
			}
			else
				Resize(GetSize());
		}

		template<typename T> T* Push(size_t count = 1)
		{
			if (stackTop_ + sizeof(T) * count >= stackEnd_)
				Expand<T>(count);

			T* ret = reinterpret_cast<T*>(stackTop_);
			stackTop_ += sizeof(T) * count;
			return ret;
		}

		//混合类型栈本身不记录类型信息，需要制定弹出类型
		template<typename T> T* Pop(size_t count)
		{
			assert(GetSize() >= count * sizeof(T));
			stackTop_ -= count * sizeof(T);
			return reinterpret_cast<T*>(stackTop_);
		}

		template<typename T> T* Top()
		{
			assert(GetSize() >= sizeof(T));
			return reinterpret_cast<T*>(stackTop_);
		}

		Allocator& GetAllocator() { return allocator_; }

		size_t GetSize()const { return  static_cast<size_t>(stackTop_ - stack_); }
		size_t GetCapacity()const { return static_cast<size_t>(stackEnd_ - stack_); }

		bool Empty()const { return stackTop_ == stack_; }

	private:

		template<typename T> void Expand(size_t count)
		{
			size_t newCapacity = 0;
			if (stack_ == nullptr){
				if (allocator_ == nullptr)
				{
					allocator_ = new Allocator();
					ownAllocator_ = allocator_;
				}

				newCapacity = initialCapacity_;
			}
			else {
				newCapacity = GetCapacity();
				newCapacity += (newCapacity + 1) / 2; //  *1.5f
			}

			size_t newSize = GetSize() + sizeof(T) * count;
			if (newCapacity < newSize)
			{
				newCapacity = newSize;
			}

			Resize(newCapacity);
		}

		void Resize(size_t newCapacity)
		{
			const size_t sz = GetSize();
			stack_ = (char*)allocator_->Realloc(stack_, GetCapacity(), newCapacity);
			stackTop_ = stack_ + sz;
			stackEnd_ = stack_ + newCapacity;
		}

		void Destroy()
		{
			Allocator::Free(stack_);
			if (ownAllocator_ != nullptr)
			{
				delete(ownAllocator_);
				ownAllocator_ = nullptr;
			}
		}

		MixStack(const MixStack&) = delete;
		MixStack& Operator(const MixStack&) = delete;

	private:
		Allocator* allocator_;
		Allocator* ownAllocator_;
		char* stack_;
		char* stackTop_;			//Stack + Size
		char* stackEnd_;			//Stack + Capacity
		size_t initialCapacity_;
	};

}