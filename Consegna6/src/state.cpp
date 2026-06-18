#include "state.h"

int ct_swap = 0;
int ct_cmp = 0;
int ct_op = 0; /// operazioni per la ricerca

int max_dim = 0;
int ntests = 1;
int ndiv = 1;
int details = 0;
int graph = 0;

int n = 0; /// dimensione dell'array

/// file di output per grafo
ofstream output_graph;
int n_operazione = 0; /// contatore di operazioni per visualizzare i vari step

int ct_visit = 0; // contatore durante visita

my_stack *s;

int idx = 0;

int *V;          // elenco dei nodi del grafo
int *V_visitato; // nodo visitato?
int *V_lowlink;  //
int *V_onStack;

int **M;

// list_t* E;  /// array con le liste di adiacenza per ogni nodo
list_t **E; /// array di puntatori a le liste di adiacenza per ogni nodo
int n_nodi;

list_t *global_ptr_ref = NULL; /// usato per memorizzare il puntatore alla prima lista allocata

int ct_fib = 0;

// Strutture dati che ospiteranno i token prodotti dalla tokenizzazione (`tokenice_source_code`)
CodeToken tokenA[MAX_TOKEN];
CodeToken tokenB[MAX_TOKEN];

// Matrice dei predecessori, per ricostruire l'allineamento ottimo
// Non contiene più numeri indicanti la scelta fatta, bensì simboli (+, -, ~) che indicano la scelta fatta
// Ciò è fatto per stampare le operazioni finali, eseguendo semplicemente backtracking delle operazioni eseguite
char P_code[MAX_TOKEN + 1][MAX_TOKEN + 1];

// Matrice dei punteggi
int M_code[MAX_TOKEN + 1][MAX_TOKEN + 1];

// Sorgenti dei due file da confrontare
char sourceA[MAX_SOURCE];
char sourceB[MAX_SOURCE];
