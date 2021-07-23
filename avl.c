#include "avl.h"
#include<stdio.h>
#include<stdlib.h>

#define EH 0       //equal
#define LH 1       //left higher
#define RH -1      //right higher
 

//create node
PNode createNode(int index, int pos) {
	PNode node = (PNode)malloc(sizeof(struct Node));
	node->index = index;
	node->pos = pos;
	node->BalanceFactor = EH;
	node->leftChild = NULL;
	node->rightChild = NULL;
	return node;
}
 
 
//r_rotate
void R_Rotate(PNode* node) {
	PNode tmp = (*node)->leftChild;
	(*node)->leftChild = tmp->rightChild;
	tmp->rightChild = (*node);
    (*node) = tmp;
}
 
//l_rotate
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
	case RH:                                                                          //RR型失衡
		(*node)->BalanceFactor = rightchild->BalanceFactor = EH;
		L_Rotate(node);
		break;
	case LH:                                                                         //RL型失衡
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
void InsertKeyValue(PNode* node, int index, int pos, int* higher) {
    //create new node
	if((*node) == NULL) {
		(*node) = createNode(index, pos);
		*higher=1;
	}else if (pos < (*node)->pos) {                  //插入到左子树中
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
}

int SearchTree(PNode root, int pos) {
	if (root->pos == pos) {
		return root->index;
	}
	else if (pos > root->pos && root->rightChild) {
		return SearchTree(root->rightChild, pos);
	}
	else if(pos < root->pos && root->leftChild) {
		return SearchTree(root->leftChild, pos);
	}
	else {
		return -1;
	}
}

void printfTree(PNode root) {
    if (root) {
        if (root->leftChild) {
            printf("%d is %d's left child\n", root->leftChild->pos, root->pos);
            printfTree(root->leftChild);
        }
        if (root->rightChild) {
            printf("%d is %d's right child\n", root->rightChild->pos, root->pos);
            printfTree(root->rightChild);
        }
    }
}

int main()
{
	int i, dataArr[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14 };
	PNode treeRoot = NULL;
	int higher;
	for (i = 0; i < 14; i++) {
		InsertKeyValue(&treeRoot, dataArr[i], 3, &higher);
		printfTree(treeRoot);
		printf("\n\n");
	}
	return 0;
}
