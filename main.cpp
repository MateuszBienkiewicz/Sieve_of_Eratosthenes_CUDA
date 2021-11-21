#include <iostream>
#include <cmath>
#include <algorithm>
#include <chrono>
using namespace std::chrono;
using namespace std;

int fun(int n, bool* tab) {
	int k, wynik = 0;
	for (int i = 2; i <= sqrt(n); i++)
		if (tab[i])
			for (int j = 2; (k = j * i) <= n; j++)
				tab[k] = false;
	for (int i = 2; i <= n; i++)
		if (tab[i]) {
			wynik++;
			cout << i << ", ";
		}
	return wynik;
}

int main() {
	int n, wynik;
	cout << "Podaj n: ";
	cin >> n;
	bool* tab;
	//cudaMallocManaged(&tab, n * sizeof(bool));
	tab = new bool[n + 1];
	fill_n(tab, n, true);
	auto start = high_resolution_clock::now();
	wynik = fun(n, tab);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Czas: " << duration.count() << endl;
	cout << "Liczb pierwszych w zakresie [2, n] jest: " << wynik;
	//cudaFree(tab);
	delete [] tab;
	return 0;
}