#include <iostream>
#include <cmath>

using namespace std;

int main(){
	int N = 7;
	int W = 15;
	int value[N + 1] = {0, 7, 9, 5, 12, 14, 6, 12};
	int wt[N + 1] = {0, 3, 4, 2, 6, 7, 3, 5};
	
	int dn[W + 1] = {0};
	int parent[W + 1][2] = {{-1,-1}};
	dn[0] = 1;
	
	for(int i = 1; i <= N; i++){
		for(int w = W - wt[i]; w >= 0; w--){
			if(dn[w]){
				if(dn[w] + value[i] > dn[w + wt[i]]){
					dn[w + wt[i]] = dn[w] + value[i];
					parent[w + wt[i]][0] = w;
					parent[w + wt[i]][1] = i;
				}
			}
		}
	}
	
	for(int i = 0; i <= W; i++){
		printf("%d ", dn[i] - 1);
	
	}
	printf("\n");
	for(int i = 0; i <= W; i++){
		printf("%d = (%d,%d)\n", i, parent[i][0], parent[i][1]);
	}
	
	return 0;
}
