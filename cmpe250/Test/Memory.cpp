#include <iostream>

using namespace std;

struct Node{
	public:
		int x, y;
		
		Node(int a = 3, int b = 5) : x(a), y(b){
			cout << "Node Constructor" << endl;
			cout << x << " - " << y << endl;
		}
		
		Node(Node& cpy){
			cout << "Node Copy Constructor" << endl;
			x = cpy.x * 2;
			y = cpy.y * 3;
		}
		
		~Node(){
			cout << "Node Destructor" << endl;
		}
};

struct MyList{
	public:
		int q, w;
		Node* head;
		
		MyList(int a = 18, int b = 99) : q(a), w(b){
			cout << "MyList Constructor" << endl;
			cout << q << " <> " << w << endl;
			Node xx = new Node(q,w);
			Node yy(3,5);
		}
		
		MyList(MyList& cpy){
			cout << "MyList Copy Constructor" << endl;
			q = cpy.q;
			w = cpy.w + 1;
			head = new Node(cpy.w);
		}
		
		~MyList(){
			cout << "MyList Destructor" << endl;
		}
};

int main(){
	MyList lst(7, 13);
	cout << lst.head->x << " " << lst.head->y << endl;
	return 0;
}
