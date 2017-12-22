#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "engine.h"

double ranking[3] = {0.0, 0.0, 0.0};
char command;
int comment;

//Ranking Algorithms + Main

void allowComment() {
    char input;
    printf("Allow Comments? Y/N: ");
    scanf(" %c", &input);
    if (input == 89 || input == 121)
        comment = 1;
    else comment = 0;
}

void resetRanking () {
    int i;
    for (i = 0; i < 3; ++i)
        ranking[i] = 0.0;
}

/*Computes tf-idf for word*/
double tf_idf (char* word, int docID, int bNum, double numberOfDocs) {
    int tf = getCount(word, docID, bNum);           //Term frequency
    int df = docFreq(word, bNum);                   //Document frequency
    double idf;                                     //Inverse Document Frequency
    if (df == 0)
        idf = log10(numberOfDocs / 1);
    else
        idf = log10(numberOfDocs / df);
        
    double tfIdf = (((double) tf) * (idf));
//    printf("tf: %d, df: %d, idf: %f, tfIdf: %f\n", tf, df, idf, tfIdf);
    return tfIdf;
}

/*Compute total rank score for all words in query*/
void rank(char* query, int b) {
    int i;
    int len = strlen(query);
    if (len > 0 && query[len - 1] == '\n')
		query[len-1] = '\0';
		
    char* travWord = strtok(query, " ");
    
    while (travWord != NULL) {
        for(i = 0; i < 3; ++i) {
//                printf("%s, %d\n", travWord, (i+1));
            ranking[i] += tf_idf(travWord, (i+1), hash_code(travWord, b), 3);
//            printf("Ranking for doc %d: %f\n", (i+1), ranking[i]);
        }
        travWord = strtok(NULL, " ");
    }
}

int docFreq(char* word, int bNum) {

  int i;
  int freq = 0;
  struct LNode* travNode;
  travNode = table[bNum]->root;
	
	for (i = 0; i < table[bNum]->size; ++i) {
	  travNode = travNode->next;
//	  printf("Trav: %s, %d\n", travNode->text, travNode->documentID);
	  if (!strcmp(travNode->text, word)) {
	    ++freq;
	  }
	}
	
	return freq;
}

void relevance() {
    int i;
    
    if (comment) {
        for (i = 0; i < 3; ++i) {
          printf("Ranking for doc %d: %f\n", (i+1), ranking[i]);
        }
    }
    printf("In Order of Relevance:\n");

    if (ranking[0] >= ranking[1] && ranking[0] >= ranking[2]) {
        if (ranking[1] >= ranking[2])
            printf("D1.txt\nD2.txt\nD3.txt\n");
        else if (ranking[2] > ranking[1])
            printf("D1.txt\nD3.txt\nD2.txt\n");
    }
    else if (ranking[1] >= ranking[0] && ranking[1] >= ranking[2]) {
        if (ranking[0] >= ranking[2])
            printf("D2.txt\nD1.txt\nD3.txt\n");
        else if (ranking[2] > ranking[0])
            printf("D2.txt\nD3.txt\nD1.txt\n");
    }
    else if (ranking[2] >= ranking[0] && ranking[2] >= ranking[1]) {
        if (ranking[0] >= ranking[1])
            printf("D3.txt\nD1.txt\nD2.txt\n");
        else if (ranking[1] > ranking[0])
            printf("D3.txt\nD2.txt\nD1.txt\n");
    }
}

void read_query(int b) {
    char *query = malloc(sizeof(char) * MAX_LINE_LENGTH);    //query must be less than MAX_LINE_LENGTH
    
    printf("Enter Search Query: ");
    getchar();
    rank(fgets(query, MAX_LINE_LENGTH, stdin), b);
}

/* TRAINING SEQUENCE*/
void training(int b) {
  FILE* doc1 = fopen("D1.txt", "r");
  FILE* doc2 = fopen("D2.txt", "r");
  FILE* doc3 = fopen("D3.txt", "r");

  readFile(doc1, 1, b);
  readFile(doc2, 2, b);
  readFile(doc3, 3, b);
}

void main() {
  
  int h,i,j;
  int b;      //Number of buckets

  allowComment();

  /* Input for buckets*/
  printf("Enter number of buckets: ");
  scanf("%d", &b);
  initializeTable(b);
  training(b);

  printf("Enter [S] to search, [P] to print table, [X] to exit: ");
  scanf(" %c", &command);
    
  if (command == 88 || command == 120) {
    free(table);   //free the hash table
    printf(">Exiting<\n");
    exit(0);
  }
  else if (command == 83 || command == 115) {
    read_query(b);
    relevance();
    resetRanking();
    main();
    free(table);
  }
  else if (command == 80 || command == 112) {
      printf("\n-> ");
      print_list(b);
      main();
  }
  else {
    printf("Invalid command.\n");
    fflush(stdin); //clear input buffer before reloading main()
    main();
  }
}