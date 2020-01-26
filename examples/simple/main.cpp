#include "../../include/DLManager.h"
#include <iostream>
#include <functional>
#include <vector>
#include <ctime>
#if __cpp_lib_filesystem >= 201703L
#include <filesystem>
#endif

const int TOTAL_NUMS = 10000;
const int CIEL = 500;

using std::function;

int main() {
	srand(time(NULL));

	function<void()> test;
	function<double(std::vector<int>)> average;

	std::string dyLibFileName = "test" + Polysoft::DLManager::getSuffix();

	std::cout << "Testing basic functionality:" << std::endl;
	/*
	For linux it is important to give an absolute or relative path since
	dlopen does not check the current directory if a path is not given
	so in this case if "test.so" was passed to lib, it would not work as
	anticipated. See below to just use C++17 filesystem paths.
	*/
	Polysoft::DLManager lib("./" + dyLibFileName);
	Polysoft::DLManager lib2;
	test = lib.getFunction<void()>("test");
	test();
	
	std::cout << std::endl << "Testing the copy function:" << std::endl;
	lib2 = lib; // Test the = overload
	lib.close();

	std::vector<int> numbers;
	for (int i = 0; i < TOTAL_NUMS; ++i) {
		numbers.push_back(rand() % CIEL);
	}

	average = lib2.getFunction<double(std::vector<int>)>("average");

	std::cout << "Average of randoms is: " << average(numbers) << std::endl;

	std::cout << std::endl << "Testing fail cases:" << std::endl;
	try {
		lib2.getFunction<void()>("invalid_function_name");
	} catch (Polysoft::NoSuchFunctionException e) {
		std::cout << "Intentional error getting \"invalid_function_name\": " << e.what() << std::endl;
	}

	try {
		lib.getFunction<void()>("test");
	} catch (Polysoft::NoLibraryOpenException e) {
		std::cout << "Intentional error getting \"test\": " << e.what() << std::endl;
	}

#if __cpp_lib_filesystem >= 201703L

	// Test using filesystem
	std::cout << std::endl << "Testing basic functionality with C++17 filesystem:" << std::endl;

	std::filesystem::path path = std::filesystem::current_path() / dyLibFileName;
	Polysoft::DLManager lib3(path);
	test = lib3.getFunction<void()>("test");
	test();
#else
	std::cout << std::endl << "C++17 not supported." << std::endl;
#endif

}
