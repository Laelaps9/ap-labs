#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_WORD_COUNT 1000
#define MAX_WORD_SIZE 50

// Struct that will act as node for a linked list of words.
typedef struct node {
    // Value of the word itself
    char *word;
    // Array of ints to store the lines in which the word was found
    int *lines;
    // Iterations of the words
    int count;
    // Pointer to the next node
    struct node *next;
} node_t;


void printlist(node_t *head) {
    node_t *current = head;

    while(current != NULL) {
        printf("[%s] -> ", current->word);
        current = current->next;
    }
    printf("NULL\n");
}

node_t * hasWord(node_t *head, char *word) {
    node_t *current = head;
    while(current != NULL) {
        if(strcmp(current->word, word) == 0) {
            return current;
        }
    }
    return NULL;
}

void push(node_t **head, char *word, int currentLine) {

    if(hasWord(*head, word) == NULL) {
        // If word isn't found, create a node with that word
        node_t *node = malloc(sizeof(node_t));
        printf("No2\n");
        node->next = *head;
        node->word = word;
        node->lines = (int *)calloc(1000, sizeof(int));
        node->lines[0] = currentLine;
        node->count = 1;
        *head = node;
    }

    // If word is found, add the new line it was found on and increase word iteration
    else {
//        node->lines[node->count] = currentLine;
//        node->count++;
        printf("Found\n");
    }
}

int wordCheck(char *word) {
    int size = 0;
    while(word[size] != '\0') {
        size++;
    }
    if(size <= 1) {
        return 0;
    }
    return 1;
}

void wordCount(char *file) {
    int currentLine = 1,
        fd = open(file, O_RDONLY),
        // Find size of file
        file_size = lseek(fd, 0, SEEK_END);
    char data[file_size + 1],
         word[MAX_WORD_SIZE];
    node_t *head = NULL;

    // Go back to the beginning of the file
    lseek(fd, 0, SEEK_SET);
    read(fd, data, file_size + 1);
    close(fd);

    for(int i = 0; i < 100; i++) {
        int j = i;

        // Stop when there's a symbol that doesn't belong to a word
        while(data[i] != ' ' && data[i] != '\n' && data[i] != ',' && data[i] != '.' && data[i] != ';' && data[i] != '"') {
            word[i - j] = data[i];
            i++;
        }
        if(wordCheck(word)) {
            push(&head, word, currentLine);
            memset(word, 0, MAX_WORD_SIZE);
        }
        if(data[i] == '\n') {
            currentLine++;
        }
    }
    printlist(head);
}

int main(int argc, char **argv) {

    if(argc < 2) {
        printf("Usage: ./cross-ref file.txt\n");
        return 1;
    }
    wordCount(argv[1]);

    return 0;
}
