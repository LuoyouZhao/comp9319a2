//#include <avl.h>
//定义节点结构
#include<stdio.h>
typedef int dataType;
typedef struct Node {
	int keyValue;             //key value
	int index;
	int balanceFactor;        //平衡因子
	struct Node *leftChild, *rightChild;
}*PNode;

PNode createNode(int keyValue, int index);
void RRotate(PNode* node);
void LRotate(PNode* node);
void leftBalance(PNode* node);
void rightBalance(PNode* node);
int InsertKeyValue(PNode* node, int keyValue, int index, int* higher);
int SearchTree(PNode root, int pos);
void printfTree(PNode root);