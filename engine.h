#ifndef ENGINE_H_
#define ENGINE_H_
#include <stdio.h>

/*Linked List*/
typedef struct LList {
    struct LNode* root;
    int size;
} linked;

/*List Node (word)*/
struct LNode {
    char* text;
    char* documentID;
    int count;
    struct LNode* next;
};

void initializeTable(int bNum);
void print_list(int bNum);
int contains(int bNum, char* word, char* docID);
int hash_code(char* w, int buckets);
void hash_table_insert(struct LNode* insertNode, int bNum);
struct LNode* create_new_node(char* word, char* docID);
int getCount(char* word, char* docID, int bNum);
void readFile(FILE* doc, char* docID, int buckets);

extern const int MAX_LINE_LENGTH;
extern struct LList** table;


#endif