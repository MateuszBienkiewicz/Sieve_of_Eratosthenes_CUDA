#include <iostream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <vector>

using namespace std::chrono;
using namespace std;

int offset = 3;
int getNum(int index){
    return 2*index + offset;
}

int getIndex(int number){
    return (number-offset)/2;
}

vector<int> fun(int n, bool* tab) {
    vector<int> result;      result.push_back(1);    result.push_back(2);
    int upperBound = getIndex(static_cast<int>(sqrt(n)));
	for (int i = 0; i <=upperBound; i++)
		if (tab[i]) {
            int number = getNum(i);
            result.push_back(number);
            for (int j = getIndex(number*number); j < getIndex(n); j+=number)
                tab[j] = false;
        }
	for (int i = upperBound + 1; i <= getIndex(n); i++)
		if (tab[i]) {
            result.push_back(getNum(i));
		}
	return result;
}

int main() {
	int n;
	cout << "Podaj n: ";
	cin >> n ;
	bool* tab;
	tab = new bool[getIndex(n)+1];  //replace with array of bits
	fill_n(tab, getIndex(n), true);
	auto start = high_resolution_clock::now();
    vector<int> result = fun(n, tab);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
    for(auto const& value: result){
        cout << value << " ";
    }
	cout << "\nCzas: " << duration.count() << " microsekund." << endl;
	cout << "Liczb pierwszych w zakresie [2, n] jest: " << result.size();
	delete [] tab;
	return 0;
}
