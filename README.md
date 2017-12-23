# Information-Retrieval-System_in_C
### Basic Information Retrieval System using TF-IDF, implemented in C.

CSCI 2461 Computer Architecture: Project 5 (Final) <br>
Professor Bhagirath Narahari <br>
The George Washington University, Fall 2015 <br><br>

* Note - Made this for computer architecture during my sophomore year before I even knew what information retrieval was.
Stoked I still have this, definitely want to play with this in the future.
* Description below comes from original README.txt



*****************************
*Text Document Search Engine*


##### -Description-

The Text Document Search Engine (TDSE) works by going through the files within a designated folder of documents and by parsing through each file word by word. Before the documents are opened and parsed, we create and initialize a
hash table, implemented using a _ of Linked List pointers. The number of
buckets that the hash table will have is entered by the user.

##### Training

Each word is then passed through the checking method that checks to see if
the word from the current document is already in the hash table. If not, then a
new node is created for the word and inserted in the hash table. If the 

Hash Function:




-----------------------
Features/Things to Know

> You can either enter the path to the directory of the text documents manually in (main.c -> char* path [Global Variable])
    or you will be prompted to enter at runtime if you leave path = NULL.
    
    ex: char* path = "C:/Users/Terry/Desktop/tsdeText/";
        
    Note for prompted input: MAX_PATH_LENGTH = 200;
    -> You will be reprompted if error occurs when trying to access Directory
        
> You can either enter the name of the directory containing the text documents manually in (main.c -> char* dir [Global Variable])
    or you will be prompted to enter at runtime if you leave dir = NULL.
        ex: char* dir = "tsdeText";
        
    Note for prompted input: MAX_DIR_LENGTH = 50;
    -> You will be reprompted if error occurs when trying to access Directory

        
-> Comment Prompt appears only once each time you run the program
-> Capitalization and punctuation is ignored when creating new nodes
-> Documents with equivalent ranking scores will be displayed in the order they are in the directory

-----
To-Do
<!-- - Testing on Shell + Testing with actual paths -->

--------------
Additions Made
