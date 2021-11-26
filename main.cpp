#include <iostream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <vector>

using namespace std::chrono;
using namespace std;

bool* tab;

int offset = 3;
int getNum(int index){
    return 2*index + offset;
}

int getIndex(int number){
    return (number-offset)/2;
}

void fun(int start, int end, int n) { // [start,end]  remove multiplicities
    int upperBound = getIndex(static_cast<int>(sqrt(n)));
	for (int i = 0; i <=upperBound; i++)
		if (tab[i]) {
            int number = getNum(i);
            for (int j = getIndex(number*number);  j >= start && j <= end; j+=2*number) { // times 2 because odd + odd = even, and even are already out so add another odd
                tab[j] = false;
            }
        }
}

int main() {
	int n;
	cout << "Podaj n: ";
	cin >> n ;
	tab = new bool[getIndex(n)+1];  //replace with array of bits
	fill_n(tab, getIndex(n)+1, true);

	auto start = high_resolution_clock::now();
    vector<int> result;
    result.push_back(2);
    fun(getIndex(5), getIndex(33), n);
    for(int i=0; i < getIndex(n)+1; i++ ){
        if(tab[i])
            result.push_back(getNum(i));
    }
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
// adapt implentation to mut
// generate segments
// run segments in concurrent threads
// each thread should receive a chunk and remove non-prime numbers
// put together the results of each thread
// goal is for bound of 1000000000 get time around 1s, currently is 9.018258s

//todo test if checks from small numbers speeds up computations