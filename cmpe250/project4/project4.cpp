/*
Student Name: Abdullah Furkan İlısu
Student Number: 2013400201
Project Number: 4
Operating System: Ubuntu 16.04
Compile Status: OK
Program Status: OK
Notes: Anything you want to say about your code that will be helpful in the grading process.
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <vector>
#define pii pair<int, int>
#define ppi pair<pii, int>
using namespace std;

class bigger {
public:
	bool operator()(const ppi& x1, const ppi& x2) {
		return x1.second < x2.second;
	}
};

bool *markRed;
int N, D;
int result = 0;
priority_queue<ppi, vector<ppi>, bigger> edges;

class UF {

public:
	int *id, cnt, *sz;
	// Create an empty union find data structure with N isolated sets.
	UF(int N) {
		cnt = N;
		id = new int[N];
		sz = new int[N];
		for (int i = 0; i < N; i++) {
			id[i] = i;
			sz[i] = 1;
		}
	}
	~UF() {
		delete[] id;
		delete[] sz;
	}
	// Return the id of component corresponding to object p.
	int find(int p) {
		int root = p;
		while (root != id[root])
			root = id[root];
		while (p != root) {
			int newp = id[p];
			id[p] = root;
			p = newp;
		}
		return root;
	}
	// Replace sets containing x and y with their union.
	void merge(int x, int y) {
		int i = find(x);
		int j = find(y);
		if (i == j)
			return;

		// make smaller root point to larger one
		if (sz[i] < sz[j]) {
			id[i] = j;
			sz[j] += sz[i];
		} else {
			id[j] = i;
			sz[i] += sz[j];
		}
		cnt--;
	}
	// Are objects x and y in the same set?
	bool connected(int x, int y) {
		return find(x) == find(y);
	}
	// Return the number of disjoint sets.
	int count() {
		return cnt;
	}
};

void readInput(char* path) {
	ifstream in;
	in.open(path);
	in >> N >> D;
	markRed = new bool[N];
	for(int i=0;i<N;i++){
		markRed[i]=false;
	}
	int u, v, l;
	for (int i = 1; i <= N - 1; i++) {
		in >> u >> v >> l;
		edges.push(ppi(pii(u,v), l));
	}
	int r;
	for (int i = 0; i < D; i++){
		in >> r;
		markRed[r]=true;
	}
	in.close();
}

void algorithm() {
	UF uf(N);
	while (!edges.empty()) {
		ppi current = edges.top();
		edges.pop();
		int x = current.first.first;
		int y = current.first.second;
		int rootx = uf.find(x);
		int rooty = uf.find(y);
		//cout << x << "-" << y << " -> " << current.second << endl;
		if(markRed[rootx] && markRed[rooty]){
			//cout << "cut this edge: " << current.second << endl;
			result += current.second;
		} else{
			//cout << "merge => " << x << " and " << y << endl;
			bool f = (markRed[rootx] || markRed[rooty]);
			uf.merge(x, y);
			markRed[uf.find(x)] = markRed[uf.find(y)] = f;
		}
	}
}

int main(int argc, char* argv[]) {
	readInput(argv[1]);
	algorithm();
	ofstream out;
	out.open(argv[2]);
	out << result << endl;
	out.close();
	return 0;
}
