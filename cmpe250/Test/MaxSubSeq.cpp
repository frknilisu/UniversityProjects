#include <iostream>
#include <cmath>

using namespace std;

int main(){
	int ar[] = {-2, -3, 4, -1, -2, 1, 5, -3, 11};
	int n = sizeof(ar)/sizeof(ar[0]);
	int dn[n];
	dn[0] = ar[0];
	int result = INT_MIN;
	for(int i = 1; i < n; i++){ 
		dn[i] = max(dn[i-1] + ar[i], ar[i]);
		if(dn[i] > result)
			result = dn[i];
	}
	printf("%d", result);
	return 0;
}
