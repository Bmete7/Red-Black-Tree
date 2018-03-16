/*
Burak METE  - 	150140131
16.12.17
*/


#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <sstream>

using namespace std;

enum Color{ BLACK , RED };

class Node{
	public:
		string name;
		char gender;
		int age;
		bool color; // true if black, false if red.
		Node *left;
		Node *right;
		Node *parent;
		int num_woman;
		int num_man;
		int num_left_woman;
		int num_left_man;
		Node(string name = "",int age = 0,char gender= '\0' ) : name(name) , age(age) , gender(gender){ this->left = NULL; this->right = NULL; this->parent = NULL;};
};

class RBTree{
	Node* head;
	public:
		RBTree(){head = NULL;};
		void insertion(string name,int age,char gender);
		void inorder(Node *root, int depth, int whichChild);
		void display();
		void Recoloring(Node *cur);
		void RotateRight(Node *cur);
		void RotateLeft(Node *cur);
		int OS_SELECT_MAN(Node *cur);
		int OS_SELECT_WOMAN(Node *cur);
		void updateSizes(Node *cur);
		string nth_woman(Node*cur ,int n);
		string nth_man(Node *cur, int n);
		void emptyList(Node* cur);
	
		~RBTree();
		
		
};

RBTree::~RBTree(){
	emptyList(head);
}

void RBTree::emptyList(Node* cur){/// deallocate tree 
	if(cur->left != NULL) emptyList(cur->left);
	if(cur->right!= NULL) emptyList(cur->right);
	delete cur;
}

string RBTree::nth_woman(Node *cur, int n){
	if(n== cur->num_left_woman + 1 && cur->gender == 'F') return cur->name;	// if left size is n -1 and current node is female, than this is the nth node
	if(n>cur->num_left_woman && cur->gender == 'F') return nth_woman(cur->right, n - cur->num_left_woman - 1);	// if n is bigger than left size, and current node is female, go to right node
	if(n>cur->num_left_woman && cur->gender != 'F') return nth_woman(cur->right, n - cur->num_left_woman);
	else return nth_woman(cur->left, n);	// if n is less than left size, nth woman should be on the left. go to left node
}

string RBTree::nth_man(Node*cur, int n){
	if(n== cur->num_left_man + 1 && cur->gender == 'M') return cur->name; // if left size is n -1 and current node is male, than this is the nth node
	if(n>cur->num_left_man && cur->gender == 'M') return nth_man(cur->right, n - cur->num_left_man - 1);// if n is bigger than left size, and current node is male, go to right node
	if(n>cur->num_left_man && cur->gender != 'M') return nth_man(cur->right, n - cur->num_left_man);// if n is less than left size, nth man should be on the left. go to left node
	else return nth_man(cur->left, n);// if n is less than left size, nth man should be on the left. go to left node
}


void RBTree::updateSizes(Node *cur){
	if(cur == NULL) return;
	
	updateSizes(cur->left);
	updateSizes(cur->right);
	
	cur->num_man = OS_SELECT_MAN(cur);
	cur->num_woman = OS_SELECT_WOMAN(cur);
	
	cur->num_left_woman = OS_SELECT_WOMAN(cur->left);	// left sizes ( rank )
	cur->num_left_man = OS_SELECT_MAN(cur->left);
	
}

int RBTree::OS_SELECT_MAN(Node *cur){
	/// if a leaf is male, return 1, if 0, return 0. If the node is not a leaf, return size[left] + size[right]. +1 if node is male
	if(cur == NULL) return 0;
	else if(cur->left == NULL && cur->right == NULL){
		if(cur->gender == 'M' || cur->gender == 'm')return 1;
		return 0;
	} 
	
	if(cur->gender == 'M' || cur->gender == 'm'){
		return OS_SELECT_MAN(cur->left) + OS_SELECT_MAN(cur->right) + 1;
		
	}
	return OS_SELECT_MAN(cur->left) + OS_SELECT_MAN(cur->right);
}

int RBTree::OS_SELECT_WOMAN(Node *cur){
	/// if a leaf is female, return 1, if 0, return 0. If the node is not a leaf, return size[left] + size[right]. +1 if node is female
	if(cur == NULL) return 0;
	else if(cur->left == NULL && cur->right == NULL){
		if(cur->gender == 'F' || cur->gender == 'f')return 1;
		return 0;
	} 
	if(cur->gender == 'F' || cur->gender == 'f') return OS_SELECT_WOMAN(cur->left) + OS_SELECT_WOMAN(cur->right) + 1;
	return OS_SELECT_WOMAN(cur->left) + OS_SELECT_WOMAN(cur->right);
}


void RBTree::RotateLeft(Node * cur){
	Node* child = cur->right;
	if(child == NULL ) return; 	// if no right child exist, no need to rotate left.
	else{
		
		if(child->left != NULL){
			cur->right = child->left;
			child->left->parent = cur;
		}
		else{
			cur->right = NULL;
		}
		if(cur->parent){
			child->parent = cur->parent;
		}
		if(cur==head){// if rotated value's parent is the root
			head = child;
		}
		else{
			if(cur == cur->parent->left){
				cur->parent->left = child;
			}
			else{
				cur->parent->right = child;
			}
			child->left = cur;
			cur->parent = child;
		}
	}

}

void RBTree::RotateRight(Node *cur){
 	Node *child = cur->left;
	if(!child) return;
	
	else{
		if(child->right){
			cur->left = child->right;			
			child->right->parent = cur;
		}
		
		else cur->left = NULL;
		
		if(cur->parent){
			child->parent = cur->parent;
		}
		if(cur == head){ // if rotated value's parent is the root
			head = child;
		}
		else{
			if(cur == cur->parent->left){
				cur->parent->left = child;
			}
			else	cur->parent->right = child;
		}
		child->right = cur;
		cur->parent = child;
	}


		
}


void RBTree::insertion(string name = "",int age = 0,char gender= '\0'){
	
	Node* newNode = new Node(name,age,gender);
	newNode->color = RED;
	if(!head){
		head = newNode;
	}
	else{
		Node * temp = head;
		Node* last = NULL;
		while(temp != NULL){
			last = temp;
			if(temp->name.compare(newNode->name) < 0 )
				temp = temp->right;
			else 
				temp = temp->left;
			}
			newNode->parent = last;
			if(last->name.compare(newNode->name) < 0 )
				last->right = newNode;
			else 
				last->left = newNode;
	}

	Recoloring(newNode);	
	updateSizes(head);	// update num_woman and num_man
}


void RBTree::Recoloring(Node *cur){
	if(head == cur){
		cur->color = BLACK;	// root is always black
		return;
	}
	while(cur->parent!= NULL && cur->parent->color == RED){
		Node *uncle;
		Node *grandParent = cur->parent->parent;
		if(cur->parent == grandParent->left){ /// if the parent of the inserted node is a left child.. 
			if(grandParent->right){ /// .. which has a sibling ...
				if(grandParent->right->color == RED){	/// ... that is red
					//// This is case 1. where uncle of the inserted node is also red. Only Recoloring Required, not rotation.
					uncle = grandParent->right;
					uncle->color = BLACK;
					cur->parent->color = BLACK;
					grandParent->color = RED;
					cur = grandParent;
				}
			}
			else{
				if(cur->parent->right == cur){

				// Case 2. Uncle is black and the inserted node is right child, Left-rotation is required. then apply left-left conditions      left-right
					cur = cur->parent;
					RotateLeft(cur);
				}
				// case 3. I.N is the left child of its parent, uncle is black. Right rotate is required 		left-left
				cur->parent->color = BLACK;
				grandParent->color = RED;
				RotateRight(grandParent);


			}
		}
		else{			// if the parent of the i.s is a right child. 
			if(grandParent->left){
				
				if(grandParent->left->color == RED){
					//case 1					uncle is red
					uncle = grandParent->left;
					uncle->color = BLACK;
					cur->parent->color = BLACK;
					grandParent->color = RED;
					cur = grandParent;
				}
			}
			else{
				if( cur == cur->parent->left){
					//case 2				right-left
					cur = cur->parent;
					RotateRight(cur);
				}
				//case 3					right-right
				cur->parent->color = BLACK;
				grandParent->color = RED;
				RotateLeft(grandParent);
			}
		}		
		head->color = BLACK;
	}
}


void RBTree::display(){
	inorder(head,0,0);
	cout << "\n\n\n";
	cout << "3th woman:	" << nth_woman(head,3) << endl;
	cout << "4th man:	" << nth_man(head,4) << endl;;
}


void RBTree::inorder(Node *root , int depth , int whichChild){
	if(root){
		inorder(root->left , depth + 1 , 1); // ident as much as the depth
		for(int i = 0 ; i < depth ; ++i) cout << "\t";
		if(whichChild == 1) cout << "/---";
		else if(whichChild == 2) cout << "|___";
		root->color == BLACK ? cout << "(B)" :  cout << "(R)";
		cout << root->name << "-" << root->age << "-" << root->gender << endl;
		inorder(root->right, depth + 1 , 2);
	}
}

int main(int argc, char* argv[]){
/* 	if(argc < 1){
 		cout << "Some arguements are missing" << endl;
 		return -1;
	}*/
	RBTree r1;
	ifstream ii;
	const char* file_name = "input.txt";
	ii.open(file_name);
	
	while(ii.good()){
		string name,gender_str,age_str;
		char gender;
		int age;
		getline(ii,name,'\t');
		getline(ii,gender_str,'\t');
		getline(ii,age_str,'\n');
		gender = gender_str[0];
		istringstream(age_str) >> age;
		r1.insertion(name,age,gender);
	}
	r1.display();


	ii.close();
	return 0;
} 
