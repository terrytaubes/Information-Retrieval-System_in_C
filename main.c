#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include "engine.h"

const int MAX_PATH_LENGTH = 200;
const int MAX_DIR_LENGTH = 50;

double* ranking;        // Double Array for Ranking scores
int docs = 0;           // Number of Docs
char** docList;         // String Array for Document Names
char command;           // Command input
int comment;            // Enable/Disable comment var
int b;                  // Number of buckets
int first;              // First main call check

/*------------------------------------------*/
/* SET path EQUAL TO PATH TO TEXT DOCUMENTS */  // Or leave path = NULL; to enter at runtime
/*------------------------------------------*/
char* path = NULL;
/*--------------------------------------------------------------*/
/* SET dir EQUAL TO NAME OF DIRECTORY CONTAINING TEXT DOCUMENTS */  // Or leave dir = NULL; to enter at runtime
/*--------------------------------------------------------------*/
char* dir = NULL;

//Ranking Algorithms + Main

void allowComment() {
    char input;
    printf("Allow Comments? Y/N: ");
    scanf(" %c", &input);
    if (input == 89 || input == 121)
        comment = 1;
    else comment = -1;
}

void resetRanking () {
    int i;
    for (i = 0; i < docs; ++i)
        ranking[i] = 0.0;
}

/*Finds Document Frequency (# of Documents the word appears in)*/
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

/*Computes tf-idf for word*/
double tf_idf (char* word, int i, int bNum, double numberOfDocs) {
    int tf, df;
    double idf, tfIdf;
    
    if (find(bNum, word, docList[i]))
        tf = getCount(word, docList[i], bNum);              //Term Frequency
    else
        tf = 0;
    
//    printf("%s, %s, %d\n", word, docList[i], bNum);
    
    df = docFreq(word, bNum);                               //Document Frequency
    if (df == 0)
        idf = log10(numberOfDocs / 1);                      //Inverse Document Frequency

    else
        idf = log10(numberOfDocs / df);
        
    tfIdf = (((double) tf) * (idf));                        //Term Frequency-Inverse Document Frequency
//    printf("tf: %d, df: %d, idf: %f, tfIdf: %f\n", tf, df, idf, tfIdf);
    return tfIdf;
}

/*Compute total rank score for all words in query*/
void rank(char* query) {
    int i;
    int len = strlen(query);
    if (len > 0 && query[len - 1] == '\n')
		query[len-1] = '\0';
		
    char* travWord = strtok(query, " ");
    
    while (travWord != NULL) {
        for(i = 0; i < 3; ++i) {
//            printf("i: %d\n", i);
            ranking[i] += tf_idf(travWord, i, hash_code(travWord, b), docs);
//            printf("Ranking for doc %d: %f\n", (i+1), ranking[i]);
        }
        travWord = strtok(NULL, " ");
    }
}

void bubbleSort() {
    int i, j;
    double temp;
    char* tempS;
    
    // printf("Before:\n");
    // for (i = 0; i < docs; ++i)
    //     printf("%f\n", ranking[i]);
    
    for (i = 1; i < docs; ++i) {
      for (j = 0; j < docs-1; ++j) {
         if (ranking[j] < ranking[j+1]) {
            temp = ranking[j];
            tempS = docList[j];
            ranking[j] = ranking[j+1];
            docList[j] = docList[j+1];
            ranking[j+1] = temp;
            docList[j+1] = tempS;
         }
      }
    }
      
    // printf("After:\n");
    // for (j = 0; j < docs; ++j)
    //     printf("%f\n", ranking[j]);
}

void relevance() {
    int i;
    
    if (comment) {
        for (i = 0; i < docs; ++i) {
          printf("Ranking for %s: %f\n", docList[i], ranking[i]);
        }
    }
    
    bubbleSort();
    printf("In Order of Relevance:\n");

    for (i = 0; i < docs; ++i)
        printf("%s\n", docList[i]);

}

/*Takes in Search Query*/
void read_query() {
    char *query = malloc(sizeof(char) * MAX_LINE_LENGTH);   //query must be less than MAX_LINE_LENGTH
    
    printf("Enter Search Query: ");
    getchar();
    rank(fgets(query, MAX_LINE_LENGTH, stdin));
}

/* TRAINING SEQUENCE*/
void training() {
    
    DIR* FD;
    struct dirent* in_file;
    FILE* common_file;
    FILE* entry_file;
    char buffer[BUFSIZ];
    char temp[50];
    char* toNull = "";
    char* fileName;
    
    docList = malloc(sizeof(char*) * docs);
    
    if (!dir) {                                             // If dir = NULL; (Manual input)
        while (!dir) {
            dir = malloc(sizeof(char) * MAX_DIR_LENGTH);
            printf("Enter Directory Name: ");               // "tdseText"
            scanf("%s", dir);
    //        printf("%s\n", dir);
        
            if (NULL == (FD = opendir (dir))) {
                fprintf(stderr, "Error : Failed to open INPUT DIRECTORY - %s\n", strerror(errno));
                dir = NULL;
            }
        }
    } 
    
    if (!path) {                                            // If path = NULL; (Manual input)
        path = malloc(sizeof(char) * MAX_PATH_LENGTH);
        printf("Enter Path to Directory: ");                // "tdseText/"
        scanf("%s", path);
//        printf("%s\n", path);
    
    /* Scanning the in directory */
    while ((in_file = readdir(FD))) {
        strcpy(temp, toNull);                               // Reinitalizes temp
        
        if ((!strcmp (in_file->d_name, ".")) ||             // if statements omit "." and ".." files
            (!strcmp (in_file->d_name, "..")))
            continue;
        else
            ++docs;                                         // Increases global var docs (number of docs in folder)
            
        fileName = in_file->d_name;                         // fileName = name of current file in directory
        docList[docs-1] = malloc(strlen(fileName) + 1);
        docList[docs-1] = fileName;
//        printf("%s, %d\n", docList[docs-1], docs-1);      // PRINT: prints Document Name and index in Document List
        
        strcat(temp, path);                             // temp = path/
        strcat(temp, fileName);                         // temp = path/fileName
        
        if (comment)
            printf("%s\n", temp);                         // PRINT: prints full path to Document
        
        entry_file = fopen(temp, "r");

           if (entry_file == NULL) {
                while (entry_file == NULL) {
                    fprintf(stderr, "Error : Failed to open ENTRY FILE - %s\n", strerror(errno));
                    path = malloc(sizeof(char) * MAX_PATH_LENGTH);
                    printf("Enter Path to Directory: ");            // "tdseText/"
                    scanf("%s", path);
//                    printf("%s\n", path);
                    strcpy(temp, toNull);                           // Reinitalizes temp
                    strcat(temp, path);                             // temp = path/
                    strcat(temp, fileName);                         // temp = path/fileName
                    
                    if (comment)
                        printf("%s\n", temp);                         // PRINT: prints full path to Document
                    
                    entry_file = fopen(temp, "r");
                }
            }
            
        readFile(entry_file, fileName, b);
        }
            
        fclose(entry_file);
    }
    ranking = malloc(sizeof(double) * docs);                // Allocate space for Ranking score array
} 

void main() {
  
  if (!first) {
    allowComment();

    /* Input for buckets*/
    printf("Enter number of buckets: ");
    scanf("%d", &b);
    initializeTable(b);
    training();
    first = 1;
  }

  printf("Enter [S] to search, [P] to print table, [X] to exit: ");
  scanf(" %c", &command);
    
  if (command == 88 || command == 120) {
    free(table);   //free the hash table
    printf(">Exiting<\n");
    exit(0);
  }
  else if (command == 83 || command == 115) {
    read_query();
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