#include"avl.h"
#include<stdio.h>
#include<stdlib.h>

#define EH 0       //equal
#define LH 1       //left higher
#define RH -1      //right higher

PNode createNode(int keyValue, int index) {
	PNode newNode = (PNode)malloc(sizeof(struct Node));
	newNode->keyValue = keyValue;
	newNode->index = index;
	newNode->balanceFactor = EH;
	newNode->leftChild = NULL;
	newNode->rightChild = NULL;
	return newNode;
}

void LRotate(PNode* node) {
	PNode tmp = (*node)->rightChild;
	(*node)->rightChild = tmp->leftChild;
	tmp->leftChild = (*node);
	(*node) = tmp;
}

void RRotate(PNode* node) {
	PNode tmp = (*node)->leftChild;
	(*node)->leftChild = tmp->rightChild;
	tmp->rightChild = (*node);
    (*node) = tmp;
}

void leftBalance(PNode* node) {
	PNode leftchild = (*node)->leftChild;
	PNode tmpRightChild = NULL;
	switch (leftchild->balanceFactor) 
	{
	case LH:                                                                     //LL imbalance
		(*node)->balanceFactor = leftchild->balanceFactor = EH;
		 RRotate(node);
		 break;
	case RH:                                                                    //LR imbalance
		tmpRightChild = leftchild->rightChild;
		switch (tmpRightChild->balanceFactor)
		{
		case EH:
			(*node)->balanceFactor = leftchild->balanceFactor = EH;
			break;

		case LH:
			(*node)->balanceFactor = RH;
			leftchild->balanceFactor = EH;
			break;
		
		case RH:
			(*node)->balanceFactor = EH;
			leftchild->balanceFactor = LH;
			break;
		}
		tmpRightChild->balanceFactor = EH;
		LRotate(&(*node)->leftChild);
		RRotate(node);
		break;
	}
}

void rightBalance(PNode* node) {
	PNode rightchild = (*node)->rightChild;
	PNode tmpChild = NULL;
	switch (rightchild->balanceFactor)
	{
	case RH:                                                                          //RR imbalance
		(*node)->balanceFactor = rightchild->balanceFactor = EH;
		LRotate(node);
		break;
	case LH:                                                                         //RL imbalance
		tmpChild = rightchild->leftChild;
		switch (tmpChild->balanceFactor)
		{
		case EH:
			(*node)->balanceFactor = rightchild->balanceFactor = EH;
			break;
			
		case LH:
			(*node)->balanceFactor = EH;
			rightchild->balanceFactor = RH;
			break;
		
		case RH:
			(*node)->balanceFactor = EH;
			rightchild->balanceFactor = LH;
			break;
		}
		tmpChild->balanceFactor = EH;
		RRotate(&(*node)->rightChild);
		LRotate(node);
		break;
	}
}
 
 
int InsertKeyValue(PNode* node, int keyValue, int index, int* higher) {
	if((*node) == NULL) {                                    //create new node
		(*node) = createNode(keyValue, index);
		*higher=1;
	}

	else if(keyValue < (*node)->keyValue) {                  //insert to left
		if(!InsertKeyValue(&(*node)->leftChild, keyValue, index, higher))   //if left exists
			return 0;
		if(*higher) {   
			switch ((*node)->balanceFactor)
			{
			case EH:
				(*node)->balanceFactor = LH;
				*higher = 1;
				break;

			case LH:
				leftBalance(node);
				*higher = 0;
				break;
			case RH:
				(*node)->balanceFactor = EH;
				*higher = 0;
				break;
			}
		}
	}
	else {
		if(!InsertKeyValue(&(*node)->rightChild, keyValue, index, higher))   //if right exists
			return 0;
		if(*higher) {
			switch ((*node)->balanceFactor)
			{
			case EH:
				(*node)->balanceFactor = RH;
				*higher = 1;
				break;

			case LH:                                                  
				(*node)->balanceFactor = EH;
				*higher = 0;
				break;

			case RH:
				rightBalance(node);
				*higher = 0;
				break;
			}
		}
	}
	return 1;
}
void printfTree(PNode root) {
    if (root) {
        if(root->leftChild) {
            printf("%d is %d's left child\n", root->leftChild->keyValue, root->keyValue);
            printfTree(root->leftChild);
        }
        if(root->rightChild) {
            printf("%d is %d's right child\n", root->rightChild->keyValue, root->keyValue);
            printfTree(root->rightChild);
        }
    }
}

int SearchTree(PNode root, int key) {
	if (root->keyValue == key) {
		return root->index;
	}
	else if(key > root->keyValue && root->rightChild) {
		return SearchTree(root->rightChild, key);
	}
	else if(key < root->keyValue && root->leftChild) {
		return SearchTree(root->leftChild, key);
	}
	else{
		return -1;
	}
}

// int main()
// {
// 	int i, dataArr[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14 };
// 	PNode treeRoot = NULL;
// 	int higher;
// 	for (i = 0; i < 14; i++) {
// 		InsertKeyValue(&treeRoot, dataArr[i], i, &higher);
// 	}
// 	printfTree(treeRoot);
// 	printf("5 is %d\n", SearchTree(treeRoot, 5));
// 	return 0;
// }