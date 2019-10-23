#include "GenericAlgorithm.h"
#include <assert.h>
#include <iostream>
#include <random>
#include <map>

using namespace std;


GenericAlgorithm::GenericAlgorithm():stepDebug(false)
{
}


GenericAlgorithm::~GenericAlgorithm()
{
}

void GenericAlgorithm::SampleTest(SortType t,int size /*= 10*/)
{
	cout << endl;
	cout << "=== Sort Test ===" << endl;

	vector<int> sampleNums;
	map<int, int> check;

	std::random_device rd;
	for (int i = 0 ; i < size; ++i)
	{
		int n = rd() % 100;
		sampleNums.push_back(n);

		auto itr = check.find(n);
		if (itr == check.end())
			check.insert(make_pair(n, 1));
		else
			itr->second++;
	}

	cout << "Nums: ";
	for (int i = 0; i < size; ++i)
	{
		cout << " " << sampleNums[i] << " ";
	}
	cout << endl;

	stepDebug = true;
	Sort(t, sampleNums);

	cout << "Checking...";

	bool result = true;
	for (int i = 1 ; i < size; ++i)
	{
		if (sampleNums[i] < sampleNums[i - 1])
		{
			result = false;
			break;
		}
	}

	if (result)
	{
		for (int i = 0; i < size; ++i)
		{
			auto itr = check.find(sampleNums[i]);
			if (itr == check.end())
			{
				result = false;
				break;
			}
			else
			{
				if (itr->second-- < 0)
				{
					result = false;
					break;
				}
			}
		}
	}


	if (result)
		cout << "	[Succeed]" << endl;
	else
		cout << "	[Failed]" << endl;
}

void GenericAlgorithm::Sort(SortType t, vector<int>& nums)
{
	switch (t)
	{
	case GenericAlgorithm::Selection:
		SelectionSort(nums);
		break;
	case GenericAlgorithm::Insertion:
		InsertionSort(nums);
		break;
	case GenericAlgorithm::Bubble:
		BubbleSort(nums);
		break;
	case GenericAlgorithm::Hash:
		break;
	case GenericAlgorithm::Fast:
		break;
	case GenericAlgorithm::Heap:
		break;
	case GenericAlgorithm::Merge:
		break;
	default:
		assert(false);
		break;
	}
}

//	Time		- O(n²) : N(N-1)/2	
//	Extra Space - O(1)	: 0
void GenericAlgorithm::SelectionSort(vector<int>& nums)
{
	cout << "Selection Sort BEGIN" << endl;
	for (int i =0; i < nums.size(); ++i)
	{
		int minIndex = i;
		for(int j = i + 1; j < nums.size(); ++j)
		{
			if (nums[j] < nums[minIndex])
			{
				minIndex = j;
			}
		}

		if(minIndex != i)
			swap(nums[minIndex], nums[i]);

		if (stepDebug)
		{
			debugSortStep(nums, i, -1);
		}
	}

	cout << "Selection Sort END" << endl;
}

void GenericAlgorithm::InsertionSort(vector<int>& nums)
{
	cout << "Insertion Sort BEGIN" << endl;
	for (int i = 0; i < nums.size(); ++i)
	{
		for (int j = i; j > 0; --j)
		{
			if (nums[j] < nums[j - 1])
			{
				swap(nums[j], nums[j - 1]);
			}
			else
			{
				break;
			}
		}

		if (stepDebug)
		{
			debugSortStep(nums, i+1, -1);
		}
	}

	cout << "Insertion Sort END" << endl;
}

void GenericAlgorithm::BubbleSort(vector<int>& nums)
{
	cout << "Bubble Sort BEGIN" << endl;
	for (int i = 0; i < nums.size() - 1; ++i)
	{
		for (int j = nums.size() - 1; j > i; --j)
		{
			if (nums[j] < nums[j - 1])
			{
				swap(nums[j], nums[j - 1]);
			}
		}

		if (stepDebug)
		{
			debugSortStep(nums, i, -1);
		}
	}

	cout << "Bubble Sort END" << endl;
}

void GenericAlgorithm::debugSortStep(const vector<int>& nums, int idx1, int idx2)
{
	for (int i =0; i < nums.size(); ++i)
	{
		if (i == idx1 || i == idx2)
		{
			cout << "[" << nums[i] << "] ";
		}
		else
		{
			cout << nums[i] << " ";
		}
	}

	cout << endl;
}
