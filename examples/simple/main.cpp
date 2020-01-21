#include "../../include/DLManager.h"
#include <iostream>
#include <functional>
#include <vector>
#include <ctime>

const int TOTAL_NUMS = 10000;
const int CIEL = 500;

using std::function;

int main() {
	srand(time(NULL));

	function<void()> test;
	function<double(std::vector<int>)> average;

	/*
	For linux it is important to give an absolute or relative path since
	dlopen does not check the current directory if a path is not given
	so in this case if "test.so" was passed to lib, it would not work as
	anticipated
	*/
	Polysoft::DLManager lib = Polysoft::DLManager("./test" + Polysoft::DLManager::getSuffix());
	Polysoft::DLManager lib2;
	test = lib.getFunction<void()>("test");
	test();
	
	lib2 = lib;
	lib.close();

	std::vector<int> numbers;
	for(int i = 0; i < TOTAL_NUMS; ++i){
		numbers.push_back(rand() % CIEL);
	}

	average = lib2.getFunction<double(std::vector<int>)>("average");

	std::cout << "Average of randoms is: " << average(numbers) << std::endl;

	try{
		lib2.getFunction<void()>("invalid_function_name");
	}catch(Polysoft::NoSuchFunctionException e){
		std::cout << "Error getting \"invalid_function_name\": " << e.what() << std::endl;
	}

	try{
		lib.getFunction<void()>("test");
	}catch(Polysoft::NoLibraryOpenException e){
		std::cout << "Error getting \"test\": " << e.what() << std::endl;
	}
}
