#include <avl.h>

typedef struct Node {
	int index;             
	int pos;
	int BalanceFactor;            
	struct Node *leftChild, *rightChild;
}*PNode;
PNode createNode(int index, int pos);
void R_Rotate(PNode* node);
void L_Rotate(PNode* node);
void leftBalance(PNode* node);
void rightBalance(PNode* node);
void InsertKeyValue(PNode* node, int index, int pos, bool* higher);
int SearchTree(PNode root, int pos);