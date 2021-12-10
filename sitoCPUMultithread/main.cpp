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

void calculateSegment(int start, int end) { // [start,end]  remove multiplicities
    //cout<< start<< "  "<< end<< endl;
    int upperBound = getIndex(static_cast<int>(sqrt(getNum(end))));
	for (int i = 0; i <=upperBound; i++)

		if (tab[i]) {

            int number = getNum(i);

            if (number >= 3*3 && number % 3 == 0)
                continue;

            if (number >= 5*5 && number % 5 == 0)
                continue;
            if (number >= 7*7 && number % 7 == 0)
                continue;

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
    cout<< "Generated data" << endl;
    int nThreads = 4;

    vector<thread> threadPool;

    threadPool.reserve(nThreads);

    for (int i=0; i < nThreads ; i++){
        threadPool.emplace_back(calculateSegment, getIndex(i*(n/nThreads)),  1+getIndex(n/nThreads*(i+1)));
        //cout<< "Starting thread " << i+1 << endl;
    }

    auto start = high_resolution_clock::now();

    for(auto & thread: threadPool){
        thread.join();
    }

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