#ifndef __SORTING_H
#define __SORTING_H
#include <vector>

template <typename T>
// Compares two objects. Returns -1 if a < b, 1 if a > b, and 0 otherwise.
int Compare(const T& a, const T& b) {

	if (a == b)
		return 0;
	else if (a < b)
		return -1;
	else
		return 1;

}

template <typename T>
void BubbleSort(std::vector<T>& items, int start = 0, int end = -1, int max_passes = -1) {

	// Initialize variables.
	int n = (end != -1) ? (end) : (-1 + items.size());
	int passes = 0;
	bool flag;

	// Perform the sorting algorithm.
	do {
		flag = false;
		for (size_t i = start; i < n; ++i) {
			if (Compare(items[i], items[i + 1]) == 1) {
				std::swap(items[i], items[i + 1]);
				flag = true;
			}
		}
		--n;
		++passes;
		if (max_passes != -1 && passes >= max_passes)
			break;
	} while (flag);

}

template <typename T>
void InsertionSort(std::vector<T>& items, int start = 0, int end = -1) {

	// Initialize variables.
	int n = (end != -1) ? (end) : (items.size());

	// Perform the sorting algorithm.
	for (int i = 1; i < n; ++i)
		for (int j = i; j > 0 && items[j] < items[j - 1]; --j)
			std::swap(items[j], items[j - 1]);

}

template <typename T>
void HeapSort(std::vector<T>& items) {



}

#endif