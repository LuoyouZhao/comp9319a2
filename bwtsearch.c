#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "avl.h"
#define MAX 10000
#define strLen 100


typedef struct Dictionary{
    PNode *dic;
}Dictionary;

Dictionary* newDictionary(char* str, int* record) {
    Dictionary* dictionary = (Dictionary*)malloc(sizeof(dictionary));
    dictionary->dic = (PNode*)malloc(128*sizeof(PNode));
    for(int i=0; i<128; i++) {
        dictionary->dic[i] = NULL;
    }
    int length = strlen(str);
    int higher[128] = {0};
    for(int i = 0; i<length; i++) {
        char c = str[i];
        InsertKeyValue(&dictionary->dic[c], record[i], i, &higher[c]);
    }
    return dictionary;
}


void outputString(char* str, char* filename) {
    FILE *fp = fopen(filename,"w");
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
    char* mode;
    char* match;
    if(argc == 4) {
        fileName = argv[1];
        match = argv[3];
    }else {
        mode = argv[1];
        fileName = argv[2];
        match = argv[4];
    }
    // printf("mode is %s\n", mode);
    // printf("match is %s\n", match);
    // printf("fileName is %s\n", fileName);

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
    int* firstRecord = malloc((fileLength)*sizeof(int));

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
    getRecord(firstBwt, firstRecord);
    getRecord(lastBwt, lastRecord);

    Dictionary* firstDic = newDictionary(firstBwt, firstRecord);
    //Dictionary* lastDic = newDictionary(lastBwt);
    char* decodeStr = malloc((fileLength)*sizeof(char));
    decodeStr[fileLength-1] = firstBwt[0];
    decodeStr[fileLength-2] = lastBwt[0];
    char lastChar = lastBwt[0];
    int lastInt = lastRecord[0];

    for(int i = fileLength-3; i>=0; i--) {
        //printf("i is %d | lastChar is %d | lastint is %d\n", i, lastChar, lastInt);
        int index = SearchTree(firstDic->dic[lastChar], lastInt);
        //printf("    >>>index is %d\n", index);
        decodeStr[i] = lastBwt[index];
        lastChar = lastBwt[index];
        lastInt = lastRecord[index];   
    }
    // free
    free(firstBwt);
    free(firstRecord);
    free(lastBwt);
    free(lastRecord);
    free(firstDic);

    //search part
    if(strcmp(mode, "-o")==0) {                         // -o mode
        printf("-------check-------\n");
        outputString(decodeStr, match);
        return 0;
    }
    int* decodeRecord = malloc((fileLength)*sizeof(int));
    getRecord(decodeStr, decodeRecord);
    Dictionary* decodeDic = newDictionary(decodeStr, decodeRecord);
    int c = match[0];
    int length = strlen(match);
    int number = 1;
    i = SearchTree(decodeDic->dic[c], number);
    int output = 0;
    if(strcmp(mode, "-m")==0){                    // -m mode
        while(i != -1) {
            for(int j = 1; j<length; j++) {
                if(match[j] != decodeStr[i+j])
                    break;
                if(j == length-1)
                    output++;
            }
            number++;
            i = SearchTree(decodeDic->dic[c], number);
        }
        printf("%d\n", output);
    }else if(strcmp(mode, "-n")==0){                    // -n mode
        int nextLine;
        int check = 0;;
        while(i != -1) {
            check = 1;
            for(int j = 1; j<length; j++) {
                if(match[j] != decodeStr[i+j]) {
                    check = 0;
                    break;
                }
                check = j;
            }
            if(check) {
                output++;
                nextLine = i+check;
                while(decodeStr[nextLine] != 10) {
                    nextLine++;
                }
                do {
                    number++;
                    i = SearchTree(decodeDic->dic[c], number);
                }while( i<nextLine && i!=-1);
            }else {
                number++;
                i = SearchTree(decodeDic->dic[c], number);
            }
        }
        printf("%d\n", output);
    }else {
        //no mode part
        int nextLine;
        int check = 0;;
        while(i != -1) {
            check = 1;
            for(int j = 1; j<length; j++) {
                if(match[j] != decodeStr[i+j]) {
                    check = 0;
                    break;
                }
                check = j;
            }
            if(check) {
                nextLine = i;
                while(decodeStr[nextLine-1] != 10 && nextLine!=0) {
                    nextLine--;
                }
                while(decodeStr[nextLine] != 10) {
                    printf("%c", decodeStr[nextLine]);
                    nextLine++;
                }
                printf("\n");
                do {
                    number++;
                    i = SearchTree(decodeDic->dic[c], number);
                }while( i<nextLine && i!=-1);
            }else {
                number++;
                i = SearchTree(decodeDic->dic[c], number);
            }
        }
    }

}