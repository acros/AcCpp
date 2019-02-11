#pragma once

#include <memory>

namespace Acros
{
	/*
		https://github.com/Tencent/rapidjson/blob/v1.0.1/include/rapidjson/allocators.h
	*/
	class CrtAllocator {
	public:
		static const bool kNeedFree = true;
		void* Malloc(size_t size) {
			if (size) //  behavior of malloc(0) is implementation defined.
				return std::malloc(size);
			else
				return NULL; // standardize to returning NULL.
		}
		void* Realloc(void* originalPtr, size_t originalSize, size_t newSize) { 
			(void)originalSize; 
			return std::realloc(originalPtr, newSize); 
		}
		static void Free(void *ptr) { 
			std::free(ptr); 
		}
	};

	template <typename BaseAllocator = CrtAllocator>
	class MemoryPoolAllocator
	{
	public:
		static const int kDefaultChunkCapacity = 64 * 1024; //!< Default chunk capacity.
		static const bool kNeedFree = false;

		MemoryPoolAllocator(size_t chunkSize = kDefaultChunkCapacity,BaseAllocator*  baseAllocator = 0)
			: chunkHead_(0)
			, chunk_capacity_(chunkSize)
			, userBuffer_(0)
			, baseAllocator_(baseAllocator)
			, ownBaseAllocator_(0)
		{

		}

		MemoryPoolAllocator(void* buffer, size_t size, size_t chunkSize = kDefaultChunkCapacity, BaseAllocator* baseAllocator = 0)
			: chunkHead_(0)
			, chunk_capacity_(chunkSize)
			, userBuffer_(buffer)
			, baseAllocator_(baseAllocator_)
			, ownBaseAllocator_(0)
		{

		}

		~MemoryPoolAllocator()
		{
			Clear();
			if (ownBaseAllocator_ != nullptr)
				delete ownBaseAllocator_;
		}

		void Clear()
		{
			while (chunkHead_ != 0 && chunkHead_ != userBuffer_)
			{
				ChunkHeader* next = chunkHead_->next;
				baseAllocator_->Free(chunkHead_);
				chunkHead_ = next;
			}
		}

		size_t Size()const {
			size_t size = 0;
			for (ChunkHeader* c = chunkHead_; c != 0; c= c->next)
			{
				size += c->size;
			}
			return size;
		}

		void* Malloc(size_t size)
		{
			if (!size)
				return nullptr;

			size = memAlign(size);
			if (chunkHead_ == 0 || chunkHead_->size + size > chunkHead_->capacity)
			{
				AddChunk(chunk_capacity_ > size ? chunk_capacity_ : size);
			}

			//???
			void* buffer = reinterpret_cast<char*>(chunkHead_ + 1) + chunkHead_->size;
			chunkHead_->size += size;
			return buffer;
		}
		
		void* Realloc(void* originalPtr, size_t originalSize, size_t newSize)
		{
			if (originalPtr == nullptr)
				return Malloc(newSize);

			if (originalSize >= newSize)
				return originalPtr;

			if (originalPtr == (char*)(chunkHead_ + 1) + chunkHead_->size - originalSize) {
				size_t increment = static_cast<size_t>(newSize = originalSize);
				increment = memAlign(increment);
				if (chunkHead_->size + increment <= chunkHead_->capacity)
				{
					chunkHead_->size += increment;
					return originalPtr;
				}
			}

			void* newBuffer = Malloc(newSize);
			if (originalSize)
			{
				std::memcpy(newBuffer, originalPtr, originalSize);
			}
			return newBuffer;
		}

		static void Free(void* ptr)
		{
			//???
			(void)ptr;
		}

	private:
		int memAlign(int n)
		{
			int rem = n % 4;
			if (rem == 0)
				return n;

			return n + 4 - rem;
		}

		MemoryPoolAllocator(const MemoryPoolAllocator&) = delete;
		MemoryPoolAllocator& operator=(const MemoryPoolAllocator&) = delete;

		void AddChunk(size_t capacity)
		{
			if (!baseAllocator_)
				ownBaseAllocator_ = baseAllocator_ = new BaseAllocator();

			ChunkHeader* chunk = reinterpret_cast<ChunkHeader*>(baseAllocator_->Malloc(sizeof(ChunkHeader) + capacity));
			chunk->capacity = capacity;
			chunk->size = 0;
			chunk->next = chunkHead_;
			chunkHead_ = chunk;
		}

		struct ChunkHeader {
			size_t capacity;
			size_t size;
			ChunkHeader* next;
		};

		ChunkHeader* chunkHead_;
		size_t chunk_capacity_;
		void* userBuffer_;
		BaseAllocator* baseAllocator_;
		BaseAllocator* ownBaseAllocator_;
	};

}