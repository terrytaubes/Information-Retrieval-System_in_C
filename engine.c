#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine.h"

#define MAX_STRING_LENGTH 20
#define MAX_TOKS 100
#define DELIMITERS " \t"

const int MAX_LINE_LENGTH = 1000;
struct LList** table;

void initializeTable(int bNum) {
  
  int h;
  table = malloc(sizeof(struct LList*) * bNum);
  
  for (h = 0; h < bNum; h++) {                         //allocate memory for each linked list
      table[h] = malloc(sizeof(struct LList));
      table[h]->root = malloc(sizeof(struct LNode));   //avoids pointers to the same location in memory
      table[h]->root->text = NULL;                     //set conditions for hash_table_insert
      table[h]->root->next = NULL;
      table[h]->size = 0;
  }
}

void print_list(buckets) {

  int i,j;
  struct LNode* travNode;
  
  printf("Format -- Word, Document Name, Count in Document |\n\n");
  for (j = 0; j < buckets; ++j) {
  	travNode = table[j]->root->next;
  	
  	printf("Bucket %d: ", j);
  	for(i = 0; i < table[j]->size; ++i) {
      printf("%s, %s, %d | ", travNode->text, travNode->documentID, travNode->count);
  		travNode = travNode->next;
  	}
  	printf("\n\n");
  }
}

int contains(int bNum, char* word, char* docID) {

  int i;
  struct LNode* travNode;
	travNode = table[bNum]->root;
	
	for (i = 0; i < table[bNum]->size; ++i) {
	  travNode = travNode->next;
	  if (!strcmp(travNode->text, word) && (!strcmp(travNode->documentID, docID))) {
	    (travNode->count)++;
	    return 0;
	  }
	}
	return 1;
}

int find(int bNum, char* word, char* docID) {

  int i;
  struct LNode* travNode;
	travNode = table[bNum]->root;
	
	for (i = 0; i < table[bNum]->size; ++i) {
	  travNode = travNode->next;
	  if (!strcmp(travNode->text, word) && (!strcmp(travNode->documentID, docID))) {
	    return 1;
	  }
	}
	return 0;
}

int hash_code(char* w, int buckets) {
  
  int S = 0;          //sum of ASCII values
  int b;              //bucket assignment
  int i;
  
  for (i = 0; w[i] != '\0'; i++) {
    S += w[i];
  }
  b = S % buckets;
  return b;
}

void hash_table_insert(struct LNode* insertNode, int bNum) {
    
  if (table[bNum]->size != 0) {
    insertNode->next = table[bNum]->root->next;
    table[bNum]->root->next = insertNode;
  } else {
    table[bNum]->root->next = insertNode;
  }
  
 	(table[bNum]->size)++;
 	(insertNode->count)++;
}

struct LNode* create_new_node(char* word, char* docID) {
	struct LNode* node;
 	node = malloc(sizeof(struct LNode));
	node->text = word;
	node->documentID = docID;
	node->count = 0;
	node->next = NULL;
	return node;
}

int getCount(char* word, char* docID, int bNum) {
  int i;
  struct LNode* travNode;
	travNode = table[bNum]->root;
	
	for (i = 0; i < table[bNum]->size; ++i) {
	  travNode = travNode->next;
	  if (!strcmp(travNode->text, word) && (!strcmp(travNode->documentID, docID))) {
//	    printf("%s -> Count: %d\n", docID, travNode->count);
	    return  travNode->count;
	  }
	}
	return 0;
}

void readFile(FILE* doc, char* docID, int buckets) {
  
    char* docWord;
    int bNum;
    int check;
    char line[MAX_LINE_LENGTH];
    int i = 0;
    int j;
    struct LNode* tempNode;


    while(fgets(line, sizeof(line), doc)) {
//      printf("%s\n", line);

	    docWord = strtok(line, " ");
		
    	while(docWord != NULL) {
    	  int len = strlen(docWord);
    	  
    	  for (j = 0; j < len; ++j)
          docWord[j] = tolower(docWord[j]);

    	  if (len > 0 && docWord[len-1] == '\n') {
				  docWord[len-1] = '\0';
				  --len;
    	  }
				
				if (len > 0 && (docWord[len - 1] == '.' 
				            ||  docWord[len - 1] == ','
				            ||  docWord[len - 1] == '!'
				            ||  docWord[len - 1] == '?'
				            ||  docWord[len - 1] == ':'
				            ||  docWord[len - 1] == ';')) {
				    docWord[len - 1] = '\0';
			  }
			  
			  
    	  char* temp = (char*) malloc((strlen(docWord)+1) * sizeof(char));
    	  strcpy(temp, docWord);

//				printf(">> %s\n", temp);
	      bNum = hash_code(temp, buckets);

	      if (table[bNum]->size != 0)
          check = contains(bNum, temp, docID);
	      else
	        check = 1;
	      
      if (check) {
  	    tempNode = create_new_node(temp, docID);
  	    hash_table_insert(tempNode, bNum);
    	}
	      docWord = strtok(NULL, " ");
	      ++i;
    }
  }
}