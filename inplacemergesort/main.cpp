#include <iostream>
#include "merge_list.h"

using namespace std;

void block_merge_forward_test_1(){
	merge_list arr({ 3, 5, 8 }, { 1, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 9, 10 });

	cout << "=== block_merge_forward_test_1 ===" << endl;
	cout << "before: " << arr << endl;
	arr.merge();
	cout << "after: " << arr << endl << endl;
}

void block_merge_forward_test_2(){
	merge_list arr({ 1, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 9, 10 }, { 3, 5, 8 });

	cout << "=== block_merge_forward_test_2 ===" << endl;
	cout << "before: " << arr << endl;
	arr.merge();
	cout << "after: " << arr << endl << endl;
}

void merge_test_papers(){
	merge_list arr({ 2,2,2, 4,4, 5,5, 6, 7, 8,8, 10 }, { 1,1,1, 2,2, 3,3, 5, 7,7, 8, 9,9 });

	cout << "=== block_merge_forward_test_2 ===" << endl;
	cout << "before: " << endl << arr << endl;
	arr.merge();
	cout << "after: " << endl << arr << endl << endl;
}

void merge_test_not_squared_count_1(){ // TODO: create a test which won't have the trailing section (skipping prepare 3-6)
	merge_list arr({ 2,2,2, 4,4, 5,5, 6, 7, 8,8 }, { 1,1,1, 2,2, 3,3, 5, 7,7, 8, 9,9 });

	cout << "=== merge_test_not_squared_count_1 ===" << endl;
	cout << "before: " << endl << arr << endl;
	arr.merge();
	cout << "after: " << endl << arr << endl << endl;
}

void merge_test_not_squared_count_2(){ // Random. F exists len(f) = 1. ABCD exist.
	merge_list arr({ 2,2,2, 4, 5,5, 6, 8, 10 }, { 1,1, 2, 3,3, 5, 7,7, 8, 9,9 });

	cout << "=== merge_test_not_squared_count_2 ===" << endl;
	cout << "before: " << endl << arr << endl;
	arr.merge();
	cout << "after: " << endl << arr << endl << endl;
}

void merge_test_not_squared_count_3(){ // D is empty.
	merge_list arr({ 2,2,2, 4, 5,5, 6, 8, 10 }, { 1,1, 2, 3,3, 5, 7,7, 9,9 });

	cout << "=== merge_test_not_squared_count_3 ===" << endl;
	cout << "before: " << endl << arr << endl;
	arr.merge();
	cout << "after: " << endl << arr << endl << endl;
}

int main(){
	block_merge_forward_test_1();
	block_merge_forward_test_2();
	merge_test_papers();
	merge_test_not_squared_count_1();
	merge_test_not_squared_count_2();
	merge_test_not_squared_count_3();

	system("pause");
	return 0;
}