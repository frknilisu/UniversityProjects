#include <iostream>

using namespace std;

typedef struct Node{
	int data;
	struct Node* next;
}Node;

void ReverseLinkedList(Node** head){
	if(head == NULL)
		return;
	
	Node *prev = NULL;
	Node *current = *head;
	while(current != NULL){
		Node *next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	*head = prev;
}

int main(){
	
	Node* head = new Node();
	Node* x = new Node();
	Node* y = new Node();
	Node* z = new Node();
	Node* w = new Node();
	x->data = 5;
	y->data = 1;
	z->data = 3;
	w->data = 8;
	
	x->next = y;
	y->next = z;
	z->next = w;
	w->next = NULL;
	
	head = x;
	
	Node* current = new Node();
	for(current = head; current != NULL; current = current->next)
		cout << current->data << " ";
	cout << endl;
	
	ReverseLinkedList(&head);
	
	for(current = head; current != NULL; current = current->next)
		cout << current->data << " ";
	cout << endl;
	return 0;
}
