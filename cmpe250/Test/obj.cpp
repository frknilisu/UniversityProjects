#include <iostream>

using namespace std;

struct obj {
	int i;
	obj() {cout << '+';}
	obj(obj& o2) {this->i=o2.i; cout << '<';}
	~obj() {cout << '-';}
	obj& operator=(obj& o2) {this->i=o2.i; cout << '='; return o2;}
};

void fun1(obj& o) {o.i=1; cout << '1'; return;}
void fun2(obj o) {o.i=2; cout << '2'; return;}

int main() {
	obj o; //Output:+
	fun1(o); cout << o.i; //Output:11
	fun2(o); cout << o.i; //Output:<2-1
	obj o2 = o; //Output:<
	o2 = o; //Output:=
	return 0; //Output:--
}
