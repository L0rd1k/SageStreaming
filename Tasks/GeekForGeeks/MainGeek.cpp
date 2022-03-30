#include "MainGeek.h"



/* TASK1: You are given a list of n - 1 integers and these integers are in the range of 1 to n.
There are no duplicates in the list.One of the integers is missing in the list.
Write an efficient code to find the missing integer. 
Complexity - O(n)
*/
void MainGeek::findMissingNumber(int arr[], int arraySize) {
	int method = 1;
	switch (method) {
		case 1: { // Complexity - O(n)
			int sumNumber = (arraySize + 1) * (arraySize + 2) / 2;
			for (int i = 0; i < arraySize; i++) {
				sumNumber -= arr[i];
			}
			std::cout << sumNumber << std::endl;
			break;
		}
		case 2: {
			int i = 0, sumNumber = 1;
			for (i = 2; i <= (arraySize + 1); i++) {
				sumNumber += i;
				sumNumber -= arr[i - 2];
			}
			std::cout << sumNumber << std::endl;
			break;
		}
		case 3: {

		}
		default:
			break;
	}
}

MainGeek::MainGeek() {
	int arr[] = { 1, 2, 4, 5, 6 };
	int arraySize = (sizeof(arr) / sizeof(arr[0]));
	findMissingNumber(arr, arraySize);
}

MainGeek::~MainGeek() {

}