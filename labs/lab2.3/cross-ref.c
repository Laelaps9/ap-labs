#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_WORD_COUNT 1000
#define MAX_WORD_SIZE 50
#define DICTIONARY_SIZE 10000
#define MAX_WORDS 32000
#define REPORT_FILE "words.txt"

// Struct that will act as node for a linked list of words.
typedef struct word {
    // Value of the word itself
    char word[MAX_WORD_SIZE];
    // Array of ints to store the lines in which the word was found
    int lines[MAX_WORD_COUNT];
    // Iterations of the words
    int count;
}word;

struct word word_dict[DICTIONARY_SIZE];

int hasWord(char *word) {
    for(int i = 0; word_dict[i].word[0] != '\0'; i++) {
        if(strcmp(word_dict[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void push(char *word, int currentLine) {
    int index = 0;
    char *copy = (char *)calloc(sizeof(char), MAX_WORD_SIZE);
    strcpy(copy, word);
    if((index = hasWord(word)) == -1) {
        // If word isn't found, create a node with that word
        int i = 0;
        while(word_dict[i].word[0] != '\0') {
            i++;
        }
        strcpy(word_dict[i].word, copy);
        word_dict[i].lines[0] = currentLine;
        word_dict[i].count = 1;
    }

    // If word is found, add the new line it was found on and increase word iteration
    else {
        word_dict[index].lines[word_dict[index].count] = currentLine;
        word_dict[index].count++;
    }
}

// Check to see if the word is
int wordCheck(char *word) {
    int size = 0;
    //printf("%s\n", word);
    while(word[size] != '\0') {
        size++;
    }
    if(size <= 2) {
        return 0;
    }

    // List of noise words
    if(strcmp(word, "and") == 0 || strcmp(word, "the") == 0 || strcmp(word, "for") == 0 || strcmp(word, "from") == 0 || strcmp(word, "but") == 0 || strcmp(word, "with") == 0 || strcmp(word, "like") == 0) {
        return 0;
    }
    else {
        return 1;
    }
}

void write_doc() {
    char str[5];
    char tmp[5];
    tmp[0] = '\t';
    int report = open(REPORT_FILE, O_RDWR | O_CREAT, 0666);
    for(int i = 0; word_dict[i].word[0 != '\0']; i++) {
        write(report, strcat(word_dict[i].word, ":\n"), strlen(word_dict[i].word) + 2);
        for(int j = 0; word_dict[i].lines[j] != 0; j++) {
            sprintf(str, "%d", word_dict[i].lines[j]);
            write(report, " ", 1);
            write(report, strcat(str, ", "), strlen(str) + 1);
        }
        write(report, "\n", 1);
    }
    close(report);
}

void wordCount(char *file) {
    int currentLine = 1,
        fd = open(file, O_RDONLY),
        // Find size of file
        file_size = lseek(fd, 0, SEEK_END);
    char data[file_size + 1],
         word[MAX_WORD_SIZE];

    // Go back to the beginning of the file
    lseek(fd, 0, SEEK_SET);
    read(fd, data, file_size + 1);
    close(fd);

    for(int i = 0; i < file_size; i++) {
        int j = i;
        // Stop when there's a symbol that doesn't belong to a word
        while(data[i] != ' ' && data[i] != '\n' && data[i] != ',' && data[i] != '.' && data[i] != ';' && data[i] != '"') {
            word[i - j] = tolower(data[i]);
            i++;
        }
        if(wordCheck(word)) {
            push(word, currentLine);
            memset(word, 0, MAX_WORD_SIZE);
        }
        if(data[i] == '\n') {
            currentLine++;
        }
    }
    write_doc();
}

int main(int argc, char **argv) {

    if(argc < 2) {
        printf("Usage: ./cross-ref file.txt\n");
        return 1;
    }
    wordCount(argv[1]);

    return 0;
}
