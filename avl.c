#include "avl.h"
#include<stdio.h>
#include<stdlib.h>
 
 
typedef int dataType;
 
#define EH 0       //左右子树一样高
#define LH 1       //左子树比右子树高
#define RH -1      //右子树比左子树高
 
 
 
//为新建一个节点
PNode createNode(int keyValue, int index) {
	PNode newNode = (PNode)malloc(sizeof(struct Node));
	newNode->keyValue = keyValue;
	newNode->index = index;
	newNode->BalanceFactor = EH;
	newNode->leftChild = NULL;
	newNode->rightChild = NULL;
	return newNode;
}
 
//右旋 顺时针旋转
void R_Rotate(PNode* node) {
	PNode tmp = (*node)->leftChild;
	(*node)->leftChild = tmp->rightChild;
	tmp->rightChild = (*node);
    (*node) = tmp;
}
 
//左旋，逆时针旋转
void L_Rotate(PNode* node) {
	PNode tmp = (*node)->rightChild;
	(*node)->rightChild = tmp->leftChild;
	tmp->leftChild = (*node);
	(*node) = tmp;
}
 
//左边失衡调整
void leftBalance(PNode* node) {
	PNode leftchild = (*node)->leftChild;
	PNode tmpRightChild = NULL;
	switch (leftchild->BalanceFactor) 
	{
	case LH:                                                                     //LL型失衡
		(*node)->BalanceFactor = leftchild->BalanceFactor = EH;
		 R_Rotate(node);
		 break;
	case RH:                                                                    //LR型失衡
		tmpRightChild = leftchild->rightChild;
		switch (tmpRightChild->BalanceFactor)
		{
		case LH:
			(*node)->BalanceFactor = RH;
			leftchild->BalanceFactor = EH;
			break;
		case EH:
			(*node)->BalanceFactor = leftchild->BalanceFactor = EH;
			break;
		case RH:
			(*node)->BalanceFactor = EH;
			leftchild->BalanceFactor = LH;
			break;
		}
		tmpRightChild->BalanceFactor = EH;
		L_Rotate(&(*node)->leftChild);
		R_Rotate(node);
		break;
	}
}
 
 
//右边失衡调整
void rightBalance(PNode* node) {
	PNode rightchild = (*node)->rightChild;
	PNode tmpChild = NULL;
	switch (rightchild->BalanceFactor)
	{
	case RH:                                                                          //RR inbalance
		(*node)->BalanceFactor = rightchild->BalanceFactor = EH;
		L_Rotate(node);
		break;
	case LH:                                                                         //RL inbalance
		tmpChild = rightchild->leftChild;
		switch (tmpChild->BalanceFactor)
		{
		case LH:
			(*node)->BalanceFactor = EH;
			rightchild->BalanceFactor = RH;
			break;
		case EH:
			(*node)->BalanceFactor = rightchild->BalanceFactor = EH;
			break;
		case RH:
			(*node)->BalanceFactor = EH;
			rightchild->BalanceFactor = LH;
			break;
		}
		tmpChild->BalanceFactor = EH;
		R_Rotate(&(*node)->rightChild);
		L_Rotate(node);
		break;
	}
}
 
 
//插入新值,higher用于判定是否需要调整平衡因子
int InsertKeyValue(PNode* node, int keyValue, int index, int* higher) {
	if ((*node) == NULL) {                                    //树中不包含此键值，则新建一个节点，
		(*node) = createNode(keyValue, index);
		*higher=1;
	}
	else if ((*node)->keyValue == keyValue) {                //树中已经包含此键值，则不需要插入
		*higher = 0;
		return 0;
	}
	else if (keyValue < (*node)->keyValue) {                  //插入到左子树中
		if (!InsertKeyValue(&(*node)->leftChild, keyValue, higher))   //如果左子树中存在该节点
			return 0;
		if (*higher) {   
			switch ((*node)->BalanceFactor)
			{
			case LH:
				leftBalance(node);
				*higher = 0;
				break;
			case RH:
				(*node)->BalanceFactor = EH;
				*higher = 0;
				break;
			case EH:
				(*node)->BalanceFactor = LH;
				*higher = 1;
				break;
			}
		}
	}
	else {
		if (!InsertKeyValue(&(*node)->rightChild, keyValue,higher))   //如果右子树中存在该节点
			return 0;
		if (*higher) {
			switch ((*node)->BalanceFactor)
			{
			case LH:                                                  
				(*node)->BalanceFactor = EH;
				*higher = 0;
				break;
			case RH:
				rightBalance(node);
				*higher = 0;
				break;
			case EH:
				(*node)->BalanceFactor = RH;
				*higher = 1;
				break;
			}
		}
	}
	return 1;
}
void printfTree(PNode root) {
    if (root) {
        if (root->leftChild) {
            printf("%d is %d's left child\n", root->leftChild->keyValue, root->keyValue);
            printfTree(root->leftChild);
        }
        if (root->rightChild) {
            printf("%d is %d's right child\n", root->rightChild->keyValue, root->keyValue);
            printfTree(root->rightChild);
        }
    }
}

int SearchTree(PNode root, int key) {
	if (root->keyValue == key) {
		return root->index;
	}
	else if (key > root->keyValue && root->rightChild) {
		return SearchTree(root->rightChild, key);
	}
	else if(key < root->keyValue && root->leftChild) {
		return SearchTree(root->leftChild, key);
	}
	else {
		return -1;
	}

int main()
{
	int i, dataArr[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14 };
	PNode treeRoot = NULL;
	int higher;
	for (i = 0; i < 14; i++) {
		InsertKeyValue(&treeRoot, dataArr[i], 5, &higher);
	}
	printfTree(treeRoot);
	return 0;
}