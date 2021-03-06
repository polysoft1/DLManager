#include <iostream>
#include <vector>

#ifdef _WIN32
#define DllExport   __declspec( dllexport )
#else
#define DllExport
#endif

extern "C" DllExport void test(){
	std::cout << "Testing 123 testing!" << std::endl;
}

extern "C" DllExport double average(std::vector<int> nums){
	double avg = 0.0;

	for(int i = 0; i < nums.size(); ++i){
		avg += nums[i];
	}

	return avg / nums.size();
}