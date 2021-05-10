#include <iostream>
#include <algorithm>

using namespace std;

int current = 1;
int ar[100];

void percolate_up(int num){
	ar[current] = num;
	
	for(int temp = current; temp > 1;){
		int parent = (temp + 1) / 3;
		
		if(ar[temp] > ar[parent]){
			int gec = ar[temp];
			ar[temp] = ar[parent];
			ar[parent] = gec;
		}
		temp = parent;	
	}
	current++;
}

int percolate_down(){
	int ret = ar[1];
	int ar[1] = ar[current - 1];
	current--;
	
	for(int i = 1;;){
		int leftChild = 2 * i;
		int rightChild = 2 * i + 1;
		// if leftChild does not exist => also rightChild does not exist
		if(leftChild >= current)
			break;
		// if just rightChild not exist
		else if(rightChild == current){
			if(ar[leftChild] > ar[rightChild]){
				swap(ar[i], ar[leftChild]);
				i = leftChild;
			}
			break;
		}
		// both exist
		else if(rightChild < current){
			// if parent already larger than its children
			if(ar[i] > ar[leftChild] && ar[i] > ar[rightChild])
				break;
			// if parent smaller
			else{
				// if leftChild bigger than rightChild
				if(ar[leftChild] > ar[rightChild]){
					swap(ar[i], ar[leftChild]);
					i = leftChild;
				} else{
					swap(ar[i], ar[rightChild]);
					i = rightChild;
				}
			}
		}
	}
	return ret;
}

int main(){
	int arr[] = {10, 12, 1, 14, 6, 8, 15, 3, 9, 7, 4, 5, 11, 13, 2};
	for(int i = 0; i < sizeof(arr) / sizeof(int); i++)
		percolate_up(arr[i]);
	
		
	
	for(int i = 1; i <= 15; i++)
		cout << ar[i] << " ";
}
