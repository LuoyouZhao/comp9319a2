#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <avl.h>
#define MAX 10000
#define strLen 100

typedef struct Node{
    int index;
    struct Node* next;
}Node;

Node* newNode(int index) {
    Node* node = (Node *)malloc(sizeof(Node));
    node->index= index;
    node->next = NULL;
    return node;
}

typedef struct Linklist{
    Node* head;
    Node* tail;
    Node* mid;
    int midPos;
    int length;
}Linklist;

Linklist* newLinklist() {
    Linklist* linklist = (Linklist*)malloc(sizeof(Linklist));
    linklist->head = NULL;
    linklist->tail = NULL;
    linklist->mid = NULL;
    linklist->midPos = 0;
    linklist->length = 0;
    return linklist;
}

typedef struct Dictionary{
    Linklist **linklist;
}Dictionary;

void addNode(Dictionary* dictionary, int c, int index) {
    Node* node = newNode(index);
    Linklist* p = dictionary->linklist[c];
    p->length++;
    if(p->head == NULL) {
        p->head = node;
        p->tail = node;
        p->mid = node;
        p->midPos = 1;
    }else {
        p->tail->next = node;
        p->tail = node;
        if(p->midPos < p->length/2){
            p->mid = p->mid->next;
            p->midPos++;
        }
    }
}

Dictionary* newDictionary(char* str) {
    Dictionary* dictionary = (Dictionary*)malloc(sizeof(dictionary));
    dictionary->linklist = (Linklist**)malloc(128*sizeof(Linklist*));
    for(int i=0; i<128; i++) {
        dictionary->linklist[i] = newLinklist();
    }
    int length = strlen(str);
    for(int i = 0; i<length; i++) {
        addNode(dictionary, str[i], i);
    }
    return dictionary;
}

int getIndex(Dictionary* dictionary, int c, int sNumber) {
    Linklist* p = dictionary->linklist[c];
    Node* node = NULL;
    if(sNumber < p->midPos) {
        node = p->head;
        int i = 1;
        while(i != sNumber) {
            i++;
            node = node->next;
        }
        return node->index;
    }else {
        node = p->mid;
        int i = p->midPos;
        while(i != sNumber) {
            i++;
            node = node->next;
        }
        return node->index;
    }
    
}

void outputString(char* str) {
    FILE *fp = fopen("output.txt","w");
    fputs(str, fp);
}

void reverse(char *str) {
    char temp;
    size_t i, j;
    for(j=0, i=strlen(str)-1; j<i; --i, ++j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

void getRecord(char* str, int* record) {
    int length = strlen(str);
    int buff[128] = {0};
    for(int i = 0; i<length; i++) {
        int c = str[i];
        buff[c]++;
        record[i] = buff[c];
    }
}

int main(int argc, char** argv) {
    char* fileName;
    fileName = argv[1];
    clock_t start, end;
    double runtime;
    start = clock();
    //get file length
    FILE *fp = fopen(fileName,"r");
    fseek(fp,0,SEEK_END);
    int fileLength = ftell(fp);
    fseek(fp,0,SEEK_SET);

    //prepare work
    int dic[128]={0};
    char* lastBwt = malloc((fileLength)*sizeof(char));
    char* firstBwt = malloc((fileLength)*sizeof(char));
    int* lastRecord = malloc((fileLength)*sizeof(int));
    //int* firstRecord = malloc((fileLength)*sizeof(int));

    //read file, get last BWT
    int i = 0;
    while(1) {
        int c = fgetc(fp);
        if(feof(fp)) {
            break;
        }
        dic[c]++;
        lastBwt[i] = c;
        i++;
    }
    fclose(fp);

    end = clock();
    printf("part-1:%d\n", end-start);

    start = clock();
    //read dic, get first BWT
    int dicIndex = 0;
    for(int i = 0; i<128; i++){
        if(dic[i]!=0) {
            for(int j = 0; j<dic[i]; j++) {
                firstBwt[dicIndex] = i;
                dicIndex++;
            }
        }
    }
    
    //decode bwt
    //getRecord(firstBwt, firstRecord);
    getRecord(lastBwt, lastRecord);
    end = clock();
    printf("part-2:%d\n", end-start);
    
    start = clock();
    Dictionary* firstDic = newDictionary(firstBwt);
    //Dictionary* lastDic = newDictionary(lastBwt);
    char* decodeStr = malloc((fileLength-1)*sizeof(char));
    decodeStr[fileLength-1] = '\0';
    decodeStr[fileLength-2] = lastBwt[0];
    char lastChar = lastBwt[0];
    int lastInt = lastRecord[0];

    end = clock();
    printf("part-3:%d\n", end-start);

    start = clock();
    for(int i = fileLength-3; i>=0; i--) {
        int index = getIndex(firstDic, lastChar, lastInt);
        decodeStr[i] = lastBwt[index];
        lastChar = lastBwt[index];
        lastInt = lastRecord[index];   
    }
    end = clock();
    printf("part-4:%d\n", end-start);
    outputString(decodeStr);
    // for(int i=0; i<fileLength; i++) {
    //     printf("%c", decodeStr[i]);
    // }
    // printf("\n");
}