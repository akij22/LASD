#ifndef STATE_H
#define STATE_H

#include <fstream>

#include "code_token.h"
#include "legacy_types.h"

using namespace std;

extern int ct_swap;
extern int ct_cmp;
extern int ct_op; /// operazioni per la ricerca

extern int max_dim;
extern int ntests;
extern int ndiv;
extern int details;
extern int graph;

extern int n; /// dimensione dell'array

/// file di output per grafo
extern ofstream output_graph;
extern int n_operazione; /// contatore di operazioni per visualizzare i vari step

extern int ct_visit; // contatore durante visita

extern my_stack *s;

extern int idx;

//////////////////////////////////////////////////
/// Definizione della struttura dati grafo
//////////////////////////////////////////////////

extern int *V;          // elenco dei nodi del grafo
extern int *V_visitato; // nodo visitato?
extern int *V_lowlink;  //
extern int *V_onStack;

extern int **M;

// list_t* E;  /// array con le liste di adiacenza per ogni nodo
extern list_t **E; /// array di puntatori a le liste di adiacenza per ogni nodo
extern int n_nodi;

//////////////////////////////////////////////////
/// Fine Definizione della struttura dati grafo
//////////////////////////////////////////////////

extern list_t *global_ptr_ref; /// usato per memorizzare il puntatore alla prima lista allocata

extern int ct_fib;

// Strutture dati che ospiteranno i token prodotti dalla tokenizzazione (`tokenice_source_code`)
extern CodeToken tokenA[MAX_TOKEN];
extern CodeToken tokenB[MAX_TOKEN];

// Matrice dei predecessori, per ricostruire l'allineamento ottimo
// Non contiene più numeri indicanti la scelta fatta, bensì simboli (+, -, ~) che indicano la scelta fatta
// Ciò è fatto per stampare le operazioni finali, eseguendo semplicemente backtracking delle operazioni eseguite
extern char P_code[MAX_TOKEN + 1][MAX_TOKEN + 1];

// Matrice dei punteggi
extern int M_code[MAX_TOKEN + 1][MAX_TOKEN + 1];

// Sorgenti dei due file da confrontare
extern char sourceA[MAX_SOURCE];
extern char sourceB[MAX_SOURCE];

#endif
