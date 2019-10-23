#pragma once

#include <vector>

using namespace std;

class GenericAlgorithm
{
public:
	GenericAlgorithm();
	~GenericAlgorithm();


	enum SortType {

		//Base
		Selection,
		Insertion,
		Bubble,

		//Fast
		Hash,
		Fast,
		Heap,

		//Space usage
		Merge,


	};

	void SampleTest(SortType t, int size = 10);

	void Sort(SortType t, vector<int>& nums);


	void SelectionSort(vector<int>& nums);

	void InsertionSort(vector<int>& nums);

	void BubbleSort(vector<int>& nums);


protected:
	bool stepDebug;

	void debugSortStep(const vector<int>& nums, int idx1, int idx2);

};

