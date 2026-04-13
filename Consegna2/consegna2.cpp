/**
 * Scrivere le funzioni per ottenere i seguenti output (in input un albero binario):

    1. [X] Flip albero: per ogni nodo il sottoalbero destro diventa il sottoalbero sinistro e viceversa.
    2. [X] Calcolo profondità e altezza di ciascun nodo dell'albero
    3. [X] Funzione isBalanced: restituire un flag che indichi se l'albero è bilanciato o meno. Bonus: l'algoritmo è O ( n ) con n nodi
    4. [X] Funzione isComplete: restituire un flag che indiche se l'albero è completo secondo la definizione classica
    5. Funzione Lowest Common Ancestor: dati due valori presenti nell'albero, restituire il valore del nodo piu' basso che contiene entrambi nel suo sottoalbero

 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "node.h"
#include "queue.h"

using namespace std;

// compilazione: g++ consegna2.cpp

node_t *global_ptr_ref = NULL; /// usato per memorizzare il puntatore alla prima lista allocata

int ct_swap = 0;
int ct_cmp = 0;
int ct_op = 0; /// operazioni per la ricerca

int max_dim = 0;
int ntests = 1;
int ndiv = 1;
int details = 0;
int graph = 0;

int ct_visit = 0;
int n = 0; /// dimensione dell'array

ofstream output_graph;
int n_operazione = 0; /// contatore di operazioni per visualizzare i vari step

ofstream output_visit;
ifstream input_visit;

int max_nodes = 10;
int n_nodes = 0;

// Funzione per definire il parsing durante l'esecuzione
int parse_cmd(int argc, char **argv) {
    int ok_parse = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][1] == 'v') {
            details = 1;
            ok_parse = 1;
        }
        if (argv[i][1] == 'g') {
            graph = 1;
            ok_parse = 1;
        }
    }

    if (argc > 1 && !ok_parse) {
        printf("Usage: %s [Options]\n", argv[0]);
        printf("Options:\n");
        printf("  -verbose: Abilita stampe durante l'esecuzione dell'algoritmo\n");
        printf("  -graph: crea graph.dot e graph.pdf dell'albero\n");
        return 1;
    }

    return 0;
}


int max (int a, int b) {
    return a > b ? a : b;
}


int tree_max(int a, int b) {
    return a > b ? a : b;
}

int get_address(void *node) {
    if (node == NULL)
        return 0;
    return abs((int)((long)node - (long)global_ptr_ref));
}

void print_node_code(node_t *n) {
    output_graph << "node_" << get_address(n) << "_" << n_operazione;
}

void node_print_graph(node_t *n) {
    print_node_code(n);
    output_graph << "\n[label=<\n<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\" >\n<TR> <TD CELLPADDING=\"3\" BORDER=\"0\" ALIGN=\"LEFT\" bgcolor=\"#f0f0f0\" PORT=\"id\">";
    output_graph << get_address(n) << "</TD> </TR><TR>\n<TD PORT=\"val\" bgcolor=\"#a0FFa0\">";
    output_graph << n->val << "</TD>\n <TD PORT=\"L\" ";
    if (n->L == NULL)
        output_graph << "bgcolor=\"#808080\"> NULL";
    else
        output_graph << "> " << get_address(n->L);
    output_graph << "</TD>\n <TD PORT=\"R\" ";
    if (n->R == NULL)
        output_graph << "bgcolor=\"#808080\"> NULL";
    else
        output_graph << "> " << get_address(n->R);
    output_graph << "</TD>\n</TR></TABLE>>];\n";

    if (n->L != NULL && n->R != NULL) {
        output_graph << "rank = same; ";
        print_node_code(n);
        output_graph << ";";
        print_node_code(n->L);
        output_graph << ";\n";
    }

    if (n->L != NULL) {
        print_node_code(n);
        output_graph << ":L:c -> ";
        print_node_code(n->L);
        output_graph << ":id ;\n";
    }

    if (n->R != NULL) {
        print_node_code(n);
        output_graph << ":R:c -> ";
        print_node_code(n->R);
        output_graph << ":id ;\n";
    }
}

void tree_print_rec_graph(node_t *n) {
    if (n != NULL) {
        node_print_graph(n);
        tree_print_rec_graph(n->L);
        tree_print_rec_graph(n->R);
    }
}

void tree_print_graph(node_t *n) {
    tree_print_rec_graph(n);
    n_operazione++;
}

void export_tree_graph(node_t *root) {
    output_graph.open("graph.dot");
    output_graph << "digraph g\n";
    output_graph << "{\n";
    output_graph << "node [shape=none]\n";
    output_graph << "rankdir=\"TB\"\n";
    output_graph << "edge[tailclip=false,arrowtail=dot];\n";

    tree_print_graph(root);

    output_graph << "}\n";
    output_graph.close();

    system("dot graph.dot -Tpdf -o graph.pdf");
}

void node_print(node_t *n) {
    if (n == NULL)
        printf("Puntatore vuoto\n");
    else
        printf("allocato in %d [Val: %d, L: %d, R: %d]\n",
               get_address(n),
               n->val,
               get_address(n->R),
               get_address(n->L));
}

node_t *node_new(int elem) {
    node_t *t = new node_t;
    if (details) {
        printf("nodo creato\n");
    }

    t->val = elem;
    t->L = NULL;
    t->R = NULL;
    if (details) {
        printf("Imposto a NULL children\n");
    }

    return t;
}

void tree_insert_child_L(node_t *n, int elem) {
    n->L = node_new(elem);
}

void tree_insert_child_R(node_t *n, int elem) {
    n->R = node_new(elem);
}

void insert_random_rec(node_t *n) {
    printf("inserisco %d\n", n_nodes);

    if (n_nodes++ >= max_nodes)
        return;
    printf("inserisco %d\n", n_nodes);

    float probabilita = 0.8;

    if (rand() % 100 < probabilita * 100) {
        tree_insert_child_R(n, rand() % 100);
    }
    if (rand() % 100 < probabilita * 100) {
        tree_insert_child_L(n, rand() % 100);
    }
    if (n->L != NULL)
        insert_random_rec(n->L);
    if (n->R != NULL)
        insert_random_rec(n->R);
}

void preOrder(node_t *n) {
    if (details)
        printf("%d ", n->val);

    if (n->L != NULL) {
        preOrder(n->L);
    }
    if (n->R != NULL)
        preOrder(n->R);
}

void postOrder(node_t *n) {
    if (n->L != NULL) {
        postOrder(n->L);
    }

    if (n->R != NULL) {
        postOrder(n->R);
    }

    if (details)
        printf("%d ", n->val);
}

void EulerOrder(node_t *n) {
    if (details)
        printf("%d ", n->val);

    output_visit << n->val << "\n";

    if (n->L != NULL) {
        EulerOrder(n->L);
    }

    if (details)
        printf("%d ", n->val);
    output_visit << n->val << "\n";

    if (n->R != NULL) {
        EulerOrder(n->R);
    }

    if (details)
        printf("%d ", n->val);
    output_visit << n->val << "\n";
}

void print_tree_rec(node_t *n, int depth) {
    if (n == NULL)
        return;

    print_tree_rec(n->R, depth + 1);

    for (int i = 0; i < depth; i++)
        printf("    ");
    printf("%d\n", n->val);

    print_tree_rec(n->L, depth + 1);
}

void print_tree(node_t *root) {
    if (root == NULL) {
        printf("Albero vuoto\n");
        return;
    }

    print_tree_rec(root, 0);
    printf("\n");
}




void print_queue(queue_t* q) {
    printf("Coda: ");

    element_t* current = q->head;
    while (current != NULL) {
        printf("%d ", current->val->val);
        current = current->next;
    }

    printf("\n");
}




node_t* flip(node_t *n) {
    node_t *n1 = node_new(n->val);

    if (n->L != NULL)
        n1->R = flip(n->L);

    if (n->R != NULL)
        n1->L = flip(n->R);

    return n1;
}

int calculate_node_dept(node_t* n, node_t* current) {
    if (current == NULL)
        printf("Ho trovato un nodo null, torno indietro\n");

    if (current == NULL)
        return -1;

    if (current == n)
        return 0;

    int left_counter = calculate_node_dept(n, current->L);

    if (left_counter != -1)
        return 1 + left_counter;

    int right_counter = calculate_node_dept(n, current->R);

    if (right_counter != -1)
        return right_counter + 1;

    return -1;
}

void calculate_all_depts(node_t* current, node_t* root) {
    if (current == NULL)
        return;

    printf("Profondita' del nodo %i e' %i \n", current->val, calculate_node_dept(current, root));

    calculate_all_depts(current->L, root);
    calculate_all_depts(current->R, root);
}

int calculate_node_height(node_t* current) {
    if (current == NULL)
        return -1;

    return 1 + tree_max(calculate_node_height(current->L),
                        calculate_node_height(current->R));
}

void calculate_all_heights(node_t* current) {
    if (current == NULL)
        return;

    printf("Altezza' del nodo %i e' %i \n", current->val, calculate_node_height(current));

    calculate_all_heights(current->L);
    calculate_all_heights(current->R);
}

bool isComplete(node_t* current) {
    bool check_null = false;
    bool final = true;

    if (current == NULL)
        return true;

    queue_t q;
    queue_init(&q);

    enqueue(&q, current);

    while (!queue_is_empty(&q)) {
        node_t* node = dequeue(&q);

        if (node->L != NULL) {
            if (check_null) {
                final = true;
                break;
            }

            enqueue(&q, node->L);
        } else {
            check_null = true;
        }

        if (node->R != NULL) {
            if (check_null) {
                final = false;
                break;
            }
            enqueue(&q, node->R);

        } else {
            check_null = true;
        }
    }

    return final;
}

node_t* build_euler() {
    return NULL;
}


// Utilizzo di DFS POST-ORDER
int checkHeight(node_t* current) {
    if (current == NULL)
        return 0;

    // Calcolo altezza di tutto il sottoalbero sinistro
    int left_height = checkHeight(current->L);

    if (left_height == -1)
        return -1;

    // Calcolo altezza di tutto il sottoalbero destro
    int right_height = checkHeight(current->R);

    if (right_height == -1)
        return -1;

    if (abs(left_height - right_height) > 1)
        return -1;

    printf("Nodo corrente %i con altezza %i\n", current->val, 1 + max(left_height, right_height));
    return 1 + max(left_height, right_height);
}

bool isBalanced(node_t* root) {
    return (checkHeight(root) != -1) ? true : false;
}


int main(int argc, char **argv) {
    if (parse_cmd(argc, argv))
        return 1;

    srand((unsigned)time(NULL));

    node_t *root = node_new(1);
    global_ptr_ref = root;

    tree_insert_child_L(root, 2);
    tree_insert_child_L(root->L, 3);
    tree_insert_child_R(root, 4);
    tree_insert_child_L(root->R, 5);
    tree_insert_child_R(root->R, 6);

    print_tree(root);

    printf("Flip dell'albero:\n");
    node_t* root1 = flip(root);
    print_tree(root1);

    calculate_all_depts(root1, root1);
    calculate_all_heights(root1);

    if (graph)
        export_tree_graph(root1);

    node_t* non_complete_root = node_new(10);
    tree_insert_child_L(non_complete_root, 20);
    tree_insert_child_R(non_complete_root, 30);
    tree_insert_child_R(non_complete_root->L, 40);

    node_t* complete_root = node_new(1);
    tree_insert_child_L(complete_root, 2);
    tree_insert_child_R(complete_root, 3);
    tree_insert_child_L(complete_root->L, 4);
    tree_insert_child_R(complete_root->L, 5);

    printf("Albero non completo:\n");
    print_tree(non_complete_root);
    printf("%d\n", isComplete(non_complete_root));

    printf("Albero completo:\n");
    print_tree(complete_root);
    printf("%d\n", isComplete(complete_root));

    printf("Albero non bilanciato:\n");
    node_t* non_balanced_root = node_new(50);

    tree_insert_child_L(non_balanced_root, 51);
    tree_insert_child_L(non_balanced_root->L, 52);
    tree_insert_child_L(non_balanced_root->L->L, 53);
    tree_insert_child_L(non_balanced_root->L->L->L, 54);

    print_tree(non_balanced_root);

    printf("%d\n", isBalanced(non_balanced_root));
    printf("%d", isBalanced(root1));







    return 0;
}
