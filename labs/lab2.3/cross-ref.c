#include <stdio.h>

typedef struct node {
    char *word;
    int *lines;
    struct node *next;
} word;

void push(node_t **head, int val) {
    node_t *new_node;
    new_node = malloc(sizeof(node_t));
    new_node->val = val;
    new_node->next = *head;
    *head = new_node;
}

int main(int argc, char **argv) {
    int line = 1;
    word *head = NULL;
}
