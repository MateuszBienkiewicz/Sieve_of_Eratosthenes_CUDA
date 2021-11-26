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
            for (int j = getIndex(number*number);  j <= end; j+=number) {
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
    fun(0,  getIndex(n/2), n);
    fun(getIndex(n/3+1),  getIndex(2*n/3), n);
    fun(getIndex(2*n/3+1),  getIndex(n), n);
    cout<< "from "<< 0 << " to " << getIndex(n/2) << endl;
    cout<< "from "<< getIndex(n/2+1) << " to " << getIndex(n)+1<< endl;
    int found = 1; // including number 2
    for(int i=0; i < getIndex(n)+1; i++ ){
        if(tab[i]){
            found++;
        }
    }
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

    cout << "Liczb pierwszych w zakresie [2, n] jest: " << found;
    cout << "\nCzas: " << duration.count() << " microsekund." << endl;

    //Prepare vector of output prime numbers(optional)
/*
    vector<int> result;
    result.push_back(2);
    for(int i=0; i < getIndex(n)+1; i++ ){
        if(tab[i])
            result.push_back(getNum(i));
    }

    for(auto const& value: result){
        cout << value << " ";
    }*/


	delete [] tab;
	return 0;
}
// adapt implentation to mut
// generate segments
// run segments in concurrent threads
// each thread should receive a chunk and remove non-prime numbers
// put together the results of each thread
// goal is for bound of 1000000000 get time around 1s, currently is 9.018258s 50847534
//new goal after further optimization: 6.801863s
//1000000 78498 8056ms

//todo test if checks from small numbers speeds up computations