#include <iostream>
#include "vector.h"

double quick_select(vector<int>& vec, int pos); //find an element on certain position in any array 

double median_ConstTime(vector<int>& vec); //median of array with size<6 



int main() {

	vector<int> vec;
	int count;
	std::cin >> count;
	if (count == 0)
		return 0;
	/*for (int i = 0; i < count; ++i) {
		int k;
		std::cin >> k;
		vec.push_back(k);
	}*/
	for (int i = 0; i < count; ++i)
		vec.push_back(i);
	if (count % 2 == 0)
		std::cout << quick_select(vec, count / 2)/2. + quick_select(vec, count / 2 - 1)/2.;
	else
		std::cout << quick_select(vec, count / 2);

	return 0;
}



void InsertionSort(vector<int>& vec) {
	if (vec.size() == 0 || vec.size() == 1)
		return;
	for (int i = 1; i <= vec.size() - 1; ++i) {
		int k = i;
		while (k > 0 && vec[k] < vec[k - 1]) {
			std::swap(vec[k], vec[k - 1]);
			--k;
		}
	}
}


double median_ConstTime(vector<int>& vec) { 
	InsertionSort(vec);
	int size = vec.size();
	return size % 2 == 1 ? vec[size / 2] : vec[size / 2] / 2. + vec[size / 2 - 1] / 2.;
}


double select_ConstTime(vector<int>& vec, int pos) {
	InsertionSort(vec);
	return vec[pos];
}


double quick_select(vector<int>& vec, int pos) {
	if (vec.size() < 6)
		return select_ConstTime(vec, pos);
	vector<int> medians;
	for (int i = 0; i < vec.size() / 5; ++i) {
		vector<int> additional;
		for (int k = i * 5; k < i * 5 + 5; ++k)
			additional.push_back(vec[k]);
		medians.push_back(median_ConstTime(additional));
	}
	if (vec.size() % 5 != 0) {
		vector<int> additional;
		for (int i = vec.size() - vec.size() % 5; i < vec.size(); ++i) 
			additional.push_back(vec[i]);
		medians.push_back(median_ConstTime(additional));
	}
	int median = median_ConstTime(medians);
	medians.clear();
	vector<int> low, high;
	for (int i = 0; i < vec.size(); ++i) {
		if (vec[i] == median)
			medians.push_back(vec[i]);
		else if (vec[i] > median)
			high.push_back(vec[i]);
		else 
			low.push_back(vec[i]);
	}
	if (pos < low.size())
		return quick_select(low, pos);
	else if (pos < low.size() + medians.size())
		return medians[0];
	else
		return quick_select(high, pos-low.size()-medians.size());
}

