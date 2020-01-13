#include <iostream>
#include <vector>

extern "C" void test(){
	std::cout << "Testing 123 testing!" << std::endl;
}

extern "C" double average(std::vector<int> nums){
	double avg = 0.0;

	for(int i = 0; i < nums.size(); ++i){
		avg += nums[i];
	}

	return avg / nums.size();
}