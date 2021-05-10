#include <iostream>

using namespace std;

template <typename T>
struct obj{
	T i;
	obj (T j = 0) : i(j) { cout << '+'; }
	obj (obj<T>& o2) { this->i = o2.i; cout << '<'; }
	~obj() { cout << '-'; }
	void swap(obj<T> o){
		cout << "s";
		T temp = i;
		i = o.i;
		o.i = temp;
	}
	T operator/(obj<T>& t) { cout << '/'; return this->i/t.i; }
	T operator/(int j) { cout << 'i'; return this->i/j; }
};

template <typename T>
void swap(obj<T> o1, obj<T> o2){
	cout << "w";
	T temp = o1.i;
	o1.i = o2.i;
	o2.i = temp;
}

int main(int argc, char *argv[]){
	int x = 5;
	obj<int> o1(3.7);
	obj<int> o2(5);
	obj<int> o3(o1);
	swap(o1, o2);
	cout << o1.i << "_" << o2.i;
	o1.swap(o2);
	cout << o1.i << "_" << o2.i;
	cout << o2 / o1;
	cout << o3 / x;
	return 0;
}
