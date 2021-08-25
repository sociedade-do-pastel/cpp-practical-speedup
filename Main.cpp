#include <fstream>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

#include "Timer.hpp"

bool isPrime(int n)
{
	// extracted from:
	// https://learnprogramo.com/prime-number-program-in-c-plus-plus/
	
    // Corner cases
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    // This is checked so that we can skip
    // middle five numbers in below loop
    if (n % 2 == 0 || n % 3 == 0)
        return false;
    for (int i{5}; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    return true;
}

int threadWrapper(std::vector<int>* n, int b, int e)
{
    auto end = n->begin() + e;
    int qtPrimos{0};

    for (auto it = n->begin() + b; it != end; ++it) {
        if (isPrime(*it))
            ++qtPrimos;
    }

    return qtPrimos;
}

int main()
{
    const int c_qtThreads{5};
    std::vector<std::future<int>> threads;

    std::vector<int> numbers;

    std::string fileName{"data.csv"};
    std::ifstream inputFile;

    inputFile.open(fileName);

    if (!inputFile.is_open()) {
        std::cout << "Erro na leitura do arquivo" << std::endl;
        return 1;
    }

    int qtPrimos{0};
    std::string line{};

	// read the values and save in a vector
    while (inputFile) {
        std::getline(inputFile, line);
        numbers.push_back(std::stoi(line));
    }

	inputFile.close();

	// synchronous check
    {
        Timer::Timer timeiro;
        for (auto i : numbers) {
            if (isPrime(i))
                ++qtPrimos;
        }
    }

    std::cout << qtPrimos << std::endl;
    qtPrimos = 0;

    int parcelSize = numbers.size() / c_qtThreads;

	// asynchronous check
    {
        Timer::Timer timeiro;
        for (int i{0}; i < c_qtThreads; ++i) {
            if (i == c_qtThreads - 1)
                threads.push_back(std::async(threadWrapper, &numbers,
                                             i * parcelSize, numbers.size()));
            else
                threads.push_back(std::async(threadWrapper, &numbers,
                                             i * parcelSize,
                                             (i + 1) * parcelSize));
        }

        for (auto& i : threads) {
            i.wait();
            qtPrimos += i.get();
        }
    }

    std::cout << qtPrimos << std::endl;

    return 0;
}
