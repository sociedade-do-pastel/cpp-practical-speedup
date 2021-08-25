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

std::vector<int> getFileContent(const std::string& fileName)
{
    std::ifstream inputFile;

    inputFile.open(fileName);

    if (!inputFile.is_open()) {
        std::cout << "Erro na leitura do arquivo" << std::endl;
        exit(1);
    }

    // read the values and save in a vector
    std::string line{};
    std::vector<int> numbers;

    while (inputFile) {
        std::getline(inputFile, line);
        numbers.push_back(std::stoi(line));
    }

    inputFile.close();

    return numbers;
}


std::string findArgument(int argc, char* argv[], const std::string& pattern)
{
	for (int i{1}; i < argc; ++i) {
		std::string arg{argv[i]};
		
		auto pos = arg.find(pattern);

		if (pos != std::string::npos)
		    return arg.substr(arg.find('=')+1);
	}
	
	return "1";
}

int main(int argc, char* argv[])
{
    int qtThreads = std::stoi(findArgument(argc, argv, "--qt-threads="));
	bool shouldPrint = std::stoi(findArgument(argc, argv, "--output="));

    const std::string fileName{"data.csv"};
    std::vector<int> numbers = getFileContent(fileName);

    // open threads and compute them
    std::vector<std::future<int>> threads;

    int parcelSize = numbers.size() / qtThreads;
    int qtPrimos{0};

    auto f = [&](int i, int j) {
        return std::async(threadWrapper, &numbers, i, j);
    };

    // scope for timer, which uses destructor as end
    {
        Timer::Timer timeiro;

        for (int i{0}; i < qtThreads; ++i) {
            if (i == qtThreads - 1)
                threads.push_back(f(i * parcelSize, numbers.size()));
            else
                threads.push_back(f(i * parcelSize, (i + 1) * parcelSize));
        }

        for (auto& i : threads) {
            i.wait();
            qtPrimos += i.get();
        }
    }

	if (shouldPrint)
		std::cout << qtPrimos << std::endl;

    return 0;
}
