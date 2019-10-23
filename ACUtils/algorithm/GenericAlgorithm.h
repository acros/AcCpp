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
//		Shell,
		Quick,
		Heap,

		//Space usage
		Merge,


	};

	void SampleTest(SortType t, int size = 10);

	void Sort(SortType t, vector<int>& nums);


	void SelectionSort(vector<int>& nums);

	void InsertionSort(vector<int>& nums);

	void BubbleSort(vector<int>& nums);

	void QuickSort(vector<int>& nums);

	void HeapSort(vector<int>& nums);

	void MergeSort(vector<int>& nums);

protected:

	void InnerQuickSort(vector<int>& nums, int left, int right);

	void InnerMergeSortConverge(vector<int>& nums,int left,int mid,int right);

protected:
	bool stepDebug;

	void debugSortStep(const vector<int>& nums, int idx1, int idx2);

};

