#include <iostream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <vector>
#include <thread>
#include <omp.h>

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

int calculateSegment(int start, int end) { // [start,end]  remove multiplicities
    int upperBound = getIndex(static_cast<int>(sqrt(getNum(end))));
	for (int i = 0; i <=upperBound; i++)

		if (tab[i]) {

            int number = getNum(i);

            int minJ = (getNum(start)/number + 1)*number; // getting first multiplicity that is bigger then start

            if (minJ < number*number){ // for small start, making sure first number is bigger than square
                minJ +=number;
            }

            if ((minJ & 1) == 0)
                minJ += number;

            for (int j = getIndex(minJ);  j>=start && j <= end; j+=number) { // should be bigger that start
                tab[j] = false;
            }
        }
    int total = 0;
    for(int i=start; i < end-1; i++ ){
        if(tab[i]){
            total++;
        }
    }
    return total;
}

int main() {
	int n;
	cout << "Podaj n: ";
	cin >> n ;
    omp_set_num_threads(omp_get_num_procs());

	tab = new bool[getIndex(n)+1];  //replace with array of bits
	fill_n(tab, getIndex(n)+1, true);
    cout<< "Generated data" << endl;
    int segments = 100;

    auto start = high_resolution_clock::now();

    int total = 1; //include number 2

    #pragma omp parallel for reduction(+:total) default(none) shared(segments,n,cout)
    for (int i=0; i < segments ; i++){

        int startSeg = getIndex(i*(n/segments));
        int endSeg = 1 + getIndex((i+1)*(n/segments));

        if(startSeg<0){
            startSeg=0;
        }
        total+=calculateSegment(startSeg,endSeg);
    }

	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);

    cout << "Liczb pierwszych w zakresie [2, n] jest: " << total;
    cout << "\nCzas: " << duration.count() << " microsekund." << endl;

/*    //Prepare vector of output prime numbers(optional)
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

// generate segments
// run segments in concurrent threads
// each thread should receive a chunk and remove non-prime numbers
// put together the results of each thread
// goal is for bound of 1000000000 get time around 1s, currently is 3.529232s 50847534
