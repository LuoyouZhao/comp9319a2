//#include <avl.h>
//定义节点结构
typedef int dataType;
typedef struct Node {
	dataType keyValue;             //数据
	int BalanceFactor;             //平衡因子
	struct Node *leftChild, *rightChild;
}*PNode;

PNode createNode(dataType keyValue);
void R_Rotate(PNode* node);
void L_Rotate(PNode* node);
void leftBalance(PNode* node);
void rightBalance(PNode* node);
int InsertKeyValue(PNode* node, dataType keyValue,int* higher);
int SearchTree(PNode root, int pos);
void printfTree(PNode root);