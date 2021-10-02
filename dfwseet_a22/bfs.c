#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#define BASE 10
/*
A22. (4 puncte) Implenetati tipul arbore binar cu varfuri numere intregi alocat
inlantuit (cu pointeri). Scrieti o functie care primeste ca parametru un
arbore (pointer la radacina sa) si afisaza varfurile sale parcurgandu-l pe
nivele. In general algoritmul de parcurgere pe nivele foloseste o coada in
care initial se incarca radacina, apoi intr-un ciclu care tine cat timp coada
e nevida se extrage un varf, se afisaza, apoi se introduc in coada fii lui
(pointeri la radacinile lor). Pe post de coada functia va folosi un fisier 
tub fara nume (care va exista doar pe perioada apelului). Scrieti un program
ilustrativ pentru aceasta functie.
*/

struct TreeNode {
    int value;
    struct TreeNode* left;
    struct TreeNode* right;
};

struct QueueNode {
    struct TreeNode* tree_node;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* head;
    struct QueueNode* tail;
    int empty;
};

typedef struct QueueNode QueueNode;
typedef struct TreeNode TreeNode;
typedef struct Queue Queue;

TreeNode* new_tree_node(int value) {
    TreeNode* node = (TreeNode*) malloc(sizeof(TreeNode));
    node->value = value;
    node->left = node->right = NULL;
    return node;
}

QueueNode* new_queue_node(TreeNode* tree_node) {
    QueueNode* node = (QueueNode*) malloc(sizeof(QueueNode));
    node->tree_node = tree_node;
    node->next = NULL;
    return node;
}

Queue* create_queue() {
    Queue* q = (Queue*) malloc(sizeof(Queue));
    q->head = q->tail = NULL;
    q->empty = 0;
    return q;
}

void append(Queue* q, TreeNode* node) {
    
    QueueNode* tmp = new_queue_node(node);
    
    if (q->tail == NULL) {
        q->head = q->tail = tmp;
        q->empty = 1;
        return;
    }
    
    q->tail->next = tmp;
    q->tail = tmp;
} 

TreeNode* pop_left(Queue* q) {
    
    if (q->head == NULL) return NULL;
    
    QueueNode* tmp = q->head;
    TreeNode* node = tmp->tree_node;
    q->head = q->head->next;
    
    if (q->head == NULL) {
        q->empty = 0;
        q->tail = NULL;
    }
    
    free(tmp);

    return node;

}

void insert(TreeNode* root, int value) {
    
    Queue* q = create_queue();
    append(q, root);
    
    while (q->empty) {

        TreeNode* tmp = pop_left(q);

        if (tmp->left == NULL) {
            tmp->left = new_tree_node(value);
            break;
        } else append(q, tmp->left);
        
        if (tmp->right == NULL) {
            tmp->right = new_tree_node(value);
            break;
        } else append(q, tmp->right);
    }
}

void traversal(TreeNode* root) {
    
    if (root == NULL) return;
    
    printf("%d ", root->value);
    traversal(root->left);
    traversal(root->right);
}

void bfs(TreeNode* root) {
    int p[2];
    if (pipe(p) != 0) {
        return;
    }
    int size = 1;
    if (root == NULL) size = 0;
    write(p[1], &root, sizeof(TreeNode*));
    TreeNode* node;
    while (size != 0 && read(p[0], &node, sizeof(node)) == sizeof(node)) {
        --size;
        printf("%d ", node->value);
        if (node->left) {
            write(p[1], &node->left, sizeof(TreeNode*));
            ++size;    
        } 
        if (node->right) {
            write(p[1], &node->right, sizeof(TreeNode*));
            ++size;    
        } 
    }
    close(p[0]);
    close(p[1]);
    printf("\n");
}

int main(int argc, char** argv) {
    
    int arr[argc - 1];
    char* endptr = NULL;
    int i = 1;
    int j = 0;
    while(i < argc) {
        int number = strtol(argv[i], &endptr, BASE);
        if (errno != 0) {
            perror(argv[i]);
            return EXIT_FAILURE;
        }
        if (*endptr != 0) {
            fprintf(stderr, "%s: not a valid number\n", argv[i]);
            return EXIT_FAILURE;
        } else {
            arr[j] = number;
        }
        ++i;
        ++j;
    }

    TreeNode* root;

    if (argc >= 2) {
        root = new_tree_node(arr[0]);
        for (i = 1; i < argc - 1; ++i) {
        insert(root, arr[i]);
        }
    } else {
        root = NULL;
    }

    traversal(root);
    printf("\n");
    bfs(root);

    return EXIT_SUCCESS;
 
}