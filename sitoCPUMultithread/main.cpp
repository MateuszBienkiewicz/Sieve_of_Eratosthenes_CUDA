#include <iostream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <vector>
#include <thread>

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

            int minJ = (getNum(start)/number + 1)*number; // getting first multiplicity that is bigger then start

            if (minJ <= number*number){ // for small start, making sure first number is bigger than square
                minJ +=number;
            }

            if ((minJ & 1) == 0)
                minJ += number;

            for (int j = getIndex(minJ);  j>=start && j <= end; j+=number) { // should be bigger that start
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

    std::thread t1(fun, 0,  getIndex(n/3), n);
    std::thread t2(fun, getIndex(n/3+1),  getIndex(2*n/3), n);
    std::thread t3(fun, getIndex(2*n/3+1),  getIndex(n), n);

    auto start = high_resolution_clock::now();

    t1.join();
    t2.join();
    t3.join();
//    fun(0,  getIndex(n), n);

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

// concurrent 4.197049ms 3 threads

//todo test if checks from small numbers speeds up computations