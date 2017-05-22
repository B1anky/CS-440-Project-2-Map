#include "Map.hpp"
#include <string>
#include <random>
#include <map>
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <string>
#include <chrono>
#include <iterator>
#include <cassert>
#include <utility>
#include <random>
#include "/usr/include/valgrind/valgrind.h"

using namespace cs540;

int main(){
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 100);

    srand(time(NULL));

	size_t key1 = (size_t)dis(gen);
	size_t value1 = 50;
	size_t key2 = (size_t)dis(gen);
	size_t value2 = 75;
	size_t key3 = (size_t)dis(gen);
	size_t value3 = 40;
	size_t key4 = (size_t)dis(gen);
	size_t value4 = 35;
	size_t key5 = (size_t)dis(gen);
	size_t value5 = 2;
	size_t value6 = 10;
	size_t key6 = (size_t)dis(gen);
	size_t key7 = (size_t)dis(gen);
	size_t key8 = (size_t)dis(gen);
	size_t key9 = (size_t)dis(gen);
	size_t key10 = (size_t)dis(gen);
	size_t key11 = 102;
	
	auto pair1 = std::make_pair(key1, value1);
	auto pair2 = std::make_pair(key2, value2);
	auto pair3 = std::make_pair(key3, value3);
	auto pair4 = std::make_pair(key4, value4);
	auto pair5 = std::make_pair(key5, value5);
	auto pair6 = std::make_pair(key6, value6);
	auto pair7 = std::make_pair(key7, value6);
	auto pair8 = std::make_pair(key8, value6);
	auto pair9 = std::make_pair(key9, value6);
	auto pair10 = std::make_pair(key10, value6);
	auto pair11 = std::make_pair(key11, value6);

	auto lpair1 = std::make_pair(1, 1);
	auto lpair2 = std::make_pair(2, 1);
	auto lpair3 = std::make_pair(3, 1);
	auto lpair4 = std::make_pair(4, 1);
	auto lpair5 = std::make_pair(5, 1);

	auto gpair1 = std::make_pair(1, 5);
	auto gpair2 = std::make_pair(2, 5);
	auto gpair3 = std::make_pair(3, 5);
	auto gpair4 = std::make_pair(4, 5);
	auto gpair5 = std::make_pair(5, 5);
	auto gpair6 = std::make_pair(6, 5);

	std::vector<std::pair<int, int>> lpairs = {lpair1, lpair2, lpair3, lpair4, lpair5};	
	std::vector<std::pair<int, int>> gpairs = {gpair1, gpair2, gpair3, gpair4, gpair5, gpair6};	

	std::vector<std::pair<size_t, size_t>> pairs = {pair1, pair2, pair3, pair4, pair5, pair6, pair7, pair8, pair9, pair10};

	//Constructor testing
	std::cout << "********   Constructor and assignment operators   ********" << std::endl;

	cs540::Map<size_t, size_t> map1;
	map1.insert(pairs.begin(), pairs.end());

	cs540::Map<std::string,int> defaultConstructor; //default constructor
	std::cout << "Passed default constructor" << std::endl;

	cs540::Map<size_t, size_t> copyTest(map1); //copy constructor
	std::cout << "Passed copy constructor" << std::endl;

	cs540::Map<size_t, size_t> assignmentTest = map1;
	std::cout << "Passed assignment operator" << std::endl;

	cs540::Map<std::string, int> m{{"ba", 1}, {"a", 2}}; //initializer list
	std::cout << "Passed initializer list" << std::endl;

	std::cout << std::endl;
	std::cout << "********   Size   ********" << std::endl;

	std::map<size_t, size_t> stdMapToCompareTo(pairs.begin(), pairs.end());

	assert(map1.size() == stdMapToCompareTo.size());
	if(map1.size() == stdMapToCompareTo.size()){
		std::cout << "Pair list size is equal to std::map size" << std::endl; 
	}else{
		std::cout << "Error: sizes not equal" << std::endl; 
	}

	cs540::Map<int, int> emptyTest;
	assert(emptyTest.empty());
	if(emptyTest.empty()){
		std::cout << "Empty map is empty" << std::endl; 
	}else{
		std::cout << "Error: map should be empty" << std::endl; 
	}

	std::cout << std::endl;
	std::cout << "********   Iterators   ********" << std::endl;

	//Iterator testing

	cs540::Map<size_t, size_t>::Iterator testMyIterBegin = map1.begin();
	std::map<size_t, size_t>::iterator testStdIterBegin = stdMapToCompareTo.begin();
	assert(testMyIterBegin->first == testStdIterBegin->first);
	if(testMyIterBegin->first == testStdIterBegin->first){
		std::cout << "Begin iterator points to correct index" << std::endl; 
	}else{
		std::cout << "Error: begin() returned wrong iterator" << std::endl; 
	}

	cs540::Map<size_t, size_t>::Iterator testMyIterEnd = map1.end();
	std::map<size_t, size_t>::iterator testStdIterEnd = stdMapToCompareTo.end();
	testMyIterEnd--;
	testStdIterEnd--;
	assert(testMyIterEnd->first == testStdIterEnd ->first);
	if(testMyIterEnd->first == testStdIterEnd ->first){
		std::cout << "End iterator points to correct index" << std::endl; 
	}else{
		std::cout << "Error: end() returned wrong iterator" << std::endl; 
	}

	cs540::Map<size_t, size_t>::ConstIterator testMyConstIterBegin = map1.begin();
	std::map<size_t, size_t>::const_iterator testStdConstIterBegin = stdMapToCompareTo.begin();
	assert(testMyConstIterBegin->first == testStdConstIterBegin->first);
	if(testMyConstIterBegin->first == testStdConstIterBegin->first){
		std::cout << "Begin const_iterator points to correct index" << std::endl; 
	}else{
		std::cout << "Error: begin() returned wrong const_iterator" << std::endl; 
	}

	cs540::Map<size_t, size_t>::ConstIterator testMyConstIterEnd = map1.end();
	std::map<size_t, size_t>::const_iterator testStdConstIterEnd = stdMapToCompareTo.end();
	testMyConstIterEnd--;
	testStdConstIterEnd--;
	assert(testMyConstIterEnd->first == testStdConstIterEnd ->first);
	if(testMyConstIterEnd->first == testStdConstIterEnd ->first){
		std::cout << "End const_iterator points to correct index" << std::endl; 
	}else{
		std::cout << "Error: end() returned wrong const_iterator" << std::endl; 
	}

	cs540::Map<size_t, size_t>::ReverseIterator testMyRevIterBegin = map1.rbegin();
	std::map<size_t, size_t>::reverse_iterator testStdRevIterBegin = stdMapToCompareTo.rbegin();
	assert(testMyRevIterBegin->first == testStdRevIterBegin->first);
	if(testMyRevIterBegin->first == testStdRevIterBegin->first){
		std::cout << "Begin reverse_iterator points to correct index" << std::endl; 
	}else{
		std::cout << "Error: rbegin() returned wrong reverse_iterator" << std::endl; 
	}

	cs540::Map<size_t, size_t>::ReverseIterator testMyRevIterEnd = map1.rend();
	std::map<size_t, size_t>::reverse_iterator testStdRevIterEnd = stdMapToCompareTo.rend();
	testMyRevIterEnd--;
	testStdRevIterEnd--;
	assert(testMyRevIterEnd->first == testStdRevIterEnd->first);
	if(testMyRevIterEnd->first == testStdRevIterEnd->first){
		std::cout << "End reverse_iterator points to correct index" << std::endl; 
	}else{
		std::cout << "Error: end() returned wrong reverse_iterator" << std::endl; 
	}

	//Element Access
	std::cout << std::endl;
	std::cout << "********   Element Access   ********" << std::endl;

	cs540::Map<size_t, size_t>::Iterator lookUp1 = map1.find(key1);
	assert(lookUp1->first == key1 && lookUp1->second == value1);
	if(lookUp1->first == key1 && lookUp1->second == value1){
		std::cout << "Found " << key1 << " in map1 (Iterator)" << std::endl;
	}else{
		std::cout << "Error: Did not find " << key1 << " in map1 (Iterator)" << std::endl;
	}

	cs540::Map<size_t, size_t>::Iterator lookUpNotInMap1 = map1.find(101);
	assert(lookUpNotInMap1 == map1.end());
	if(lookUpNotInMap1 == map1.end()){
		std::cout << "Did not find 101 in map1 (Iterator)" << std::endl;
	}else{
		std::cout << "Error: Found 101 in map1, which is impossible (Iterator)" << std::endl;
	}

	cs540::Map<size_t, size_t>::ConstIterator lookUp2 = map1.find(key1);
	assert(lookUp2->first == key1 && lookUp2->second == value1);
	if(lookUp2->first == key1 && lookUp2->second == value1){
		std::cout << "Found " << key1 << " in map1 (ConstIterator)" << std::endl;
	}else{
		std::cout << "Error: Did not find " << key1 << " in map1 (ConstIterator)" << std::endl;
	}

	cs540::Map<size_t, size_t>::ConstIterator lookUpNotInMap2 = map1.find(101);
	assert(lookUpNotInMap2 == map1.end());
	if(lookUpNotInMap2 == map1.end()){
		std::cout << "Did not find 101 in map1 (ConstIterator)" << std::endl;
	}else{
		std::cout << "Error: Found 101 in map1, which is impossible (ConstIterator)" << std::endl;
	}

	std::cout << std::endl;
	std::cout << "********   Modifiers   ********" << std::endl;

	size_t originalVal = map1.at(key9);
	map1.at(key9) = 40;
	
	if(originalVal != map1.at(key9) && map1.at(key9) == 40){
		std::cout << "At key " << key9 << " the value was changed to 40 correctly" << std::endl;
	}else{
		std::cout << "Error: At key " << key9 << " the value was not changed to 40 correctly" << std::endl;
	}
	assert(originalVal != map1.at(key9) && map1.at(key9) == 40);

	bool exceptionThrown = false;
	try{
	    map1.at(101) = 40;
	}
	catch(const std::exception& e){
	    exceptionThrown = true;
	}
	assert(exceptionThrown);
	
	const cs540::Map<size_t, size_t> constMap{{5, 10}, {2, 4}, {74, 5}, {34, 1}};
	assert(constMap.at(5) == 10);
	if(constMap.at(5) == 10){
		std::cout << "At (const) key 5 the value is 10" << std::endl;
	}else{
		std::cout << "Error: At key (const) 5 the value is not 10" << std::endl;
	}

	size_t originalVal2 = map1.at(key9);
	map1[key9] = 57;
	assert(originalVal2 != map1.at(key9) && map1.at(key9) == 57);
	if(originalVal2 != map1.at(key9) && map1.at(key9) == 57){
		std::cout << "At key " << key9 << " the value was changed to 57 correctly" << std::endl;
	}else{
		std::cout << "Error: At key " << key9 << " the value was not changed to 57 correctly" << std::endl;
	}

	//Testing [] with invalid key
	map1[key11] = 1000;
	assert(map1.at(key11) == 1000);
	if(map1.at(key11) == 1000){
		std::cout << "At key " << key11 << " the value 1000 was inserted correctly" << std::endl;
	}else{
		std::cout << "Error: At key " << key11 << " the value 1000 was not inserted correctly" << std::endl;
	}

	//Modifiers
	
	cs540::Map<size_t, size_t> map2;
	map2.insert(pairs.begin(), pairs.end());
	std::pair<cs540::Map<size_t, size_t>::Iterator, bool> checkPair1 = map2.insert(pair1);
	assert(checkPair1.first->first == pair1.first && checkPair1.second == false);
	if(checkPair1.first->first == pair1.first && checkPair1.second == false){
		std::cout << "Correct pair<iter, bool> returned for inserting already inserted key" << std::endl;
	}else{
		std::cout << "Error: Incorrect pair<iter, bool> returned for inserting already inserted key" << std::endl;
	}
	
	std::pair<cs540::Map<size_t, size_t>::Iterator, bool> checkPair2 = map2.insert(pair11);
	assert(checkPair2.first->first == pair11.first && checkPair2.second == true);
	if(checkPair2.first->first == pair11.first && checkPair2.second == true){
		std::cout << "Correct pair<iter, bool> returned for inserting new key" << std::endl;
	}else{
		std::cout << "Error: Incorrect pair<iter, bool> returned for inserting new key" << std::endl;
	}

	cs540::Map<size_t, size_t>::Iterator eraseIt = map2.begin();
	size_t eraseKey = eraseIt->first;
	map2.erase(eraseIt);
	assert(map2.begin()->first != eraseKey);
	if(map2.begin()->first != eraseKey){
		std::cout << "Iterator erase erased first element of map correctly" << std::endl;
	}else{
		std::cout << "Error: Iterator erase did not erase first element of map correctly" << std::endl;
	}

	size_t eraseKey2 = map2.begin()->first;
	map2.erase(eraseKey2);
	assert(map2.begin()->first != eraseKey2);
	if(map2.begin()->first != eraseKey2){
		std::cout << "Key erase erased first element of map correctly" << std::endl;
	}else{
		std::cout << "Error: Key erase did not erase first element of map correctly" << std::endl;
	}

	map2.clear();
	assert(map2.empty());
	if(map2.empty()){
		std::cout << "Clear() works, map is empty" << std::endl;
	}else{
		std::cout << "Error: Clear() did not work, map is not empty" << std::endl;
	}

	//Comparison
	std::cout << std::endl;
	std::cout << "********   Comparison   ********" << std::endl;

	cs540::Map<size_t, size_t> smallMap;
	cs540::Map<size_t, size_t> bigMap;

	smallMap.insert(lpairs.begin(), lpairs.end());
	bigMap.insert(gpairs.begin(), gpairs.end());

	assert(!(smallMap == bigMap));
	if(smallMap == bigMap){
		std::cout << "equal: wrong" << std::endl;
	}else{
		std::cout << "not equal: correct" << std::endl;
	}

	assert(smallMap != bigMap);
	if(smallMap != bigMap){
		std::cout << "not equal: correct" << std::endl;
	}else{
		std::cout << "equal: wrong" << std::endl;
	}

	assert(smallMap < bigMap);
	if(smallMap < bigMap){
		std::cout << "small map less than big map: correct" << std::endl;
	}else{
		std::cout << "small map greater than big map: wrong" << std::endl;
	}	

	assert(!(bigMap < smallMap));
	if(bigMap < smallMap){
		std::cout << "bigger map less than small map: wrong" << std::endl;
	}else{
		std::cout << "bigger map greater than small map: correct" << std::endl;
	}

	std::cout << std::endl;
	std::cout << "********   Iterator member testing   ********" << std::endl;

	cs540::Map<size_t, size_t> map3;
	map3.insert(pairs.begin(), pairs.end());

	std::map<size_t, size_t> map3Comp;
	map3Comp.insert(pairs.begin(), pairs.end());

	cs540::Map<size_t, size_t>::Iterator myIt(map3.begin());
	std::map<size_t, size_t>::iterator stdIt = map3Comp.begin();
	assert(myIt->first == stdIt->first);
	myIt++;
	stdIt++;
	assert(myIt->first == stdIt->first);
	myIt++;
	stdIt++;
	assert(myIt->first == stdIt->first);
	myIt--;
	stdIt--;
	assert(myIt->first == stdIt->first);
	stdIt++;
	assert((++myIt)->first == stdIt->first);
	stdIt--;
	assert((--myIt)->first == stdIt->first);

	std::cout << "Passed all Iterator member tests" << std::endl;

	cs540::Map<size_t, size_t>::ConstIterator myConstIt(map3.begin());
	std::map<size_t, size_t>::const_iterator stdConstIt = map3Comp.begin();
	assert(myConstIt->first == stdConstIt->first);
	myConstIt++;
	stdConstIt++;
	assert(myConstIt->first == stdConstIt->first);
	myConstIt++;
	stdConstIt++;
	assert(myConstIt->first == stdConstIt->first);
	myConstIt--;
	stdConstIt--;
	assert(myConstIt->first == stdConstIt->first);
	stdConstIt++;
	assert((++myConstIt)->first == stdConstIt->first);
	stdConstIt--;
	assert((--myConstIt)->first == stdConstIt->first);

	std::cout << "Passed all ConstIterator member tests" << std::endl;

	cs540::Map<size_t, size_t>::ReverseIterator myReverseIt(map3.rbegin());
	std::map<size_t, size_t>::reverse_iterator stdReverseIt = map3Comp.rbegin();
	assert(myReverseIt->first == stdReverseIt ->first);
	myReverseIt++;
	stdReverseIt ++;
	assert(myReverseIt->first == stdReverseIt ->first);
	myReverseIt++;
	stdReverseIt ++;
	assert(myReverseIt->first == stdReverseIt ->first);
	myReverseIt--;
	stdReverseIt --;
	assert(myReverseIt->first == stdReverseIt ->first);
	stdReverseIt ++;
	assert((++myReverseIt)->first == stdReverseIt ->first);
	stdReverseIt --;
	assert((--myReverseIt)->first == stdReverseIt ->first);

	std::cout << "Passed all ReverseIterator member tests" << std::endl;

	
	myIt = map3.begin();
	myConstIt = map3.begin();
	myReverseIt = map3.rend();

	myReverseIt--; //Make it point to first element

	assert(myIt == myIt);
	assert(myConstIt == myConstIt);
	assert(myReverseIt == myReverseIt);

	assert(myIt == myConstIt);
	assert(myIt == myReverseIt);
	
	assert(myConstIt == myIt);
	assert(myConstIt == myReverseIt);

	assert(myReverseIt == myIt);
	assert(myReverseIt == myConstIt);

	std::cout << "All iterators passed == tests" << std::endl;

	assert(!(myIt != myIt));
	assert(!(myConstIt != myConstIt));
	assert(!(myReverseIt != myReverseIt));

	assert(!(myIt != myConstIt));
	assert(!(myIt != myReverseIt));
	
	assert(!(myConstIt != myIt));
	assert(!(myConstIt != myReverseIt));

	assert(!(myReverseIt != myIt));
	assert(!(myReverseIt != myConstIt));

	std::cout << "All iterators passed != tests" << std::endl;

	std::cout << std::endl;
	std::cout << "********  Stress testing   ********" << std::endl;

	if(RUNNING_ON_VALGRIND == 1){
		std::cout << "Valgrind detected: doubling assertion time to 5 seconds" << std::endl;
	}

	auto seed2 = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine gen2(seed2);
	std::uniform_int_distribution<unsigned int> dist(0, 10000);

	cs540::Map<int, int> stressMap;

	using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
	using Milli = std::chrono::duration<double, std::ratio<1,1000>>;
	using namespace std::chrono;

	TimePoint start, end;
	start = system_clock::now();

	for(int i = 0; i < 1000000; ++i) {
		auto g = dist(gen2);
		stressMap.insert({g,g});
	}

	int num_erases = gen2() % stressMap.size();
	for(int i = 0; i < num_erases; ++i) {
	//select a random element
	int choice = gen2() % stressMap.size();
	auto iter = std::begin(stressMap);
	for (int j = 0; j < choice; ++j) {
		++iter;
	}

	stressMap.erase(iter);
	}

	end = system_clock::now();
	Milli elapsed = end - start;
	//Gave twice as much time to allow valgrind testing wouldn't cause an assertion failure
	if(RUNNING_ON_VALGRIND == 1){
		assert(elapsed.count() < 5000);
	}
	else{
		assert(elapsed.count() < 2500);
	}

	std::cout << "Passed stress test" << std::endl;

	std::cout << std::endl;
	std::cout << "PASSED ALL TEST CASES" << std::endl;

	return 0;
}
