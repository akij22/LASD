# AGENTS.md

Guida operativa per coding agent che lavorano su questo repository.

## Contesto del progetto

La codebase e' principalmente C/C++ didattico. Molti file sono programmi autonomi con logica, strutture dati, parsing, stampa e generazione DOT nello stesso sorgente. Alcune cartelle hanno gia' iniziato una riorganizzazione con `CMakeLists.txt`, header e file `.cpp` separati.

L'obiettivo delle modifiche future e' migliorare progressivamente struttura, leggibilita' e manutenibilita' senza cambiare il comportamento dei programmi.

## Regola fondamentale

Ogni refactoring deve essere behavior-preserving.

Prima di modificare codice esistente:

1. Capire cosa stampa il programma, quali file legge, quali file scrive e quali opzioni da riga di comando supporta.
2. Creare una baseline dell'output corrente quando possibile.
3. Effettuare modifiche piccole e verificabili.
4. Ricompilare ed eseguire di nuovo lo stesso scenario.
5. Confrontare output, file generati e valori calcolati.

L'output dopo il refactoring deve rimanere identico a quello precedente, salvo richiesta esplicita diversa.

## Commenti dell'autore

I commenti esistenti sono parte integrante del progetto.

E' vietato:

- eliminare commenti esistenti;
- riscrivere commenti esistenti;
- sintetizzare commenti esistenti;
- sostituire commenti esistenti con commenti generati automaticamente;
- correggere stile, grammatica o lingua dei commenti se non richiesto.

Durante un refactoring, i commenti possono essere spostati insieme al blocco di codice a cui si riferiscono. Il loro contenuto deve rimanere invariato.

Se un commento sembra obsoleto o impreciso, non modificarlo automaticamente. Segnalarlo nel riepilogo o aggiungere un nuovo commento solo se indispensabile e separato dal commento originale.

## Divieto di reinterpretare la logica

Non assumere che una soluzione sia errata solo perche' non segue pratiche moderne.

Se una funzione produce il risultato atteso:

- non riscriverla inutilmente;
- non sostituire l'algoritmo;
- non cambiare le strutture dati;
- non introdurre ottimizzazioni speculative;
- non "modernizzare" sintassi o stile senza necessita'.

La priorita' e' conservare la logica originale. Migliorare l'organizzazione del codice, non il comportamento algoritmico.

## Restrizioni sul linguaggio e sulle librerie

Non introdurre nuove dipendenze dalla STL.

Sono vietati nei nuovi refactoring:

- `std::vector`
- `std::map`
- `std::unordered_map`
- `std::set`
- `std::list`
- `std::queue`
- `std::stack`
- `std::algorithm`
- smart pointer STL
- container o algoritmi della Standard Template Library non gia' necessari al codice toccato

Alcuni file esistenti possono gia' contenere elementi STL. Considerarli debito storico, non un pattern da estendere. Se si tocca quel codice, preservare il comportamento corrente e non propagare l'uso della STL ad altri moduli.

Preferire gli strumenti gia' presenti nel progetto:

- array C/C++ allocati esplicitamente;
- `struct` semplici;
- puntatori;
- funzioni libere;
- `stdio.h`, `stdlib.h`, `string.h`, `math.h`, `time.h` quando coerenti con il file;
- `fstream`/`iostream` solo se gia' usati nel modulo o richiesti dal comportamento esistente.

Non introdurre framework o librerie esterne senza richiesta esplicita.

## Stile architetturale desiderato

Quando un file singolo mescola responsabilita' diverse, separarle gradualmente.

Favorire, quando utile, una struttura come:

```text
include/
src/
modules/
systems/
utilities/
tests/
```

Oppure una variante piu' piccola coerente con la cartella specifica, ad esempio:

```text
main.cpp
data_types.h
data_types.cpp
algorithms.h
algorithms.cpp
io.h
io.cpp
CMakeLists.txt
```

Non forzare questa struttura in ogni cartella. Per esercizi molto piccoli, una separazione semplice tra `main.cpp`, header e implementazioni e' sufficiente.

## Separazione delle responsabilita'

Usare questi confini quando si riorganizza codice:

- `main.cpp`: parsing degli argomenti, inizializzazione, chiamate alle funzioni principali, stampa finale.
- Moduli dati: definizione di `struct`, funzioni di inizializzazione, distruzione e operazioni basilari.
- Moduli algoritmo: sorting, BFS, DFS, Dijkstra, Kruskal, edit distance, visite, validazioni.
- Moduli I/O: lettura dataset, scrittura output, esportazione DOT, stampa formattata.
- Moduli utility: solo helper realmente condivisi, evitando file generici creati troppo presto.

Separare dichiarazioni e implementazioni:

- gli `.h` devono contenere dichiarazioni, `struct`, costanti e include minimi;
- i `.cpp` devono contenere implementazioni;
- evitare implementazioni inline negli header salvo funzioni piccole gia' presenti e necessarie.

## Refactoring consentiti

Sono incoraggiati se preservano il comportamento:

- estrazione di funzioni da blocchi lunghi;
- separazione tra dichiarazioni e implementazioni;
- spostamento di funzioni in moduli coerenti;
- riduzione di duplicazione reale;
- isolamento di I/O, DOT output e parsing;
- riduzione dell'uso di globali quando il passaggio esplicito del contesto non cambia il comportamento;
- miglioramento dei nomi quando non rompe compatibilita', output, firme usate altrove o consegne.

Ogni spostamento deve portare con se' commenti, costanti, tipi e helper strettamente collegati.

## Refactoring vietati

Non eseguire:

- cambi di algoritmo;
- cambi di strutture dati che alterano ordine, memoria, output o prestazioni osservabili richieste;
- sostituzione completa di codice funzionante;
- ottimizzazioni speculative;
- conversioni massive a classi, template o pattern complessi;
- introduzione della STL;
- normalizzazione indiscriminata dello stile;
- cancellazione di file generati o binari se non richiesto;
- modifica di dataset, `graph.dot`, `graph.pdf`, CSV o file di input senza richiesta esplicita.

## Processo decisionale

Prima di creare nuovo codice:

1. Cercare una soluzione simile nella stessa cartella.
2. Cercare pattern equivalenti in altre cartelle del repository.
3. Riutilizzare nomi, stile e convenzioni gia' presenti.
4. Preferire consistenza locale a perfezione teorica.
5. Scegliere la modifica piu' piccola che migliora la struttura.

Se esistono guide locali, rispettarle. In particolare:

- `Consegna2/PROJECT_REORGANIZATION_GUIDE.md`
- `Consenga3/Project_Guideline.md`
- `Consenga3/Refactoring_Report.md`

Questi documenti indicano gia' una direzione: moduli piccoli, `struct` semplici, funzioni libere, CMake dove utile, niente astrazioni pesanti.

## Verifica del comportamento

Non esiste una suite di test unificata. Ogni cartella va trattata come programma separato.

Verifiche minime consigliate:

- compilare il programma toccato;
- eseguire lo stesso comando usato prima della modifica;
- confrontare output console e file generati;
- per gli algoritmi di ordinamento, verificare che l'array resti ordinato e che le metriche stampate non cambino se il refactoring non le riguarda;
- per grafi e alberi, verificare che visite, cammini, cicli, distanze, cluster e DOT generati restino equivalenti;
- per codice con dataset, non cambiare path, formato di lettura o ordine di elaborazione.

Comandi esistenti utili:

```sh
cd Consenga3
cmake -S . -B build
cmake --build build
./build/consegna3
./build/consegna3 -verbose
./build/consegna3 -graph
```

```sh
cd Consegna2
cmake -S . -B build
cmake --build build
./build/consegna2
./build/consegna2 -verbose
./build/consegna2 -graph
```

```sh
cd quicksort
make build
make run
```

Per programmi senza build system, compilare in modo locale e non introdurre cambiamenti globali non richiesti.

## Gestione dei file generati

Molti esercizi generano `graph.dot`, `graph.pdf`, eseguibili locali o output di visita.

Durante refactoring:

- non cambiare nomi dei file generati se il comportamento esistente li usa;
- non cambiare directory di output senza richiesta;
- non rimuovere artefatti esistenti dal repository senza richiesta;
- se si aggiunge un build system, mettere i nuovi artefatti in `build/`;
- non mescolare file sorgente e nuovi artefatti temporanei.

## Compatibilita' con la struttura esistente

Le cartelle sono esercizi indipendenti. Non creare una libreria globale condivisa tra tutte le consegne senza richiesta esplicita.

Evitare refactoring trasversali grandi. Se due cartelle duplicano una lista, una coda o un algoritmo, non unificarle automaticamente: potrebbero avere differenze didattiche o di output.

Procedere cartella per cartella, preservando i confini esistenti.

## Gestione della memoria

Il progetto usa allocazione manuale con `new`, `delete`, array e puntatori.

Quando si riorganizza:

- mantenere la stessa ownership osservabile;
- non convertire a smart pointer;
- non cambiare durata degli oggetti se puo' alterare indirizzi stampati o output DOT;
- introdurre funzioni `init`/`destroy` solo se il comportamento resta identico;
- verificare che eventuali deallocazioni aggiunte non cambino output, ordine o indirizzi usati prima della fine del programma.

## Output e formattazione

Le stampe fanno parte del comportamento.

Non modificare senza richiesta:

- testo dei messaggi;
- spazi, newline e punteggiatura;
- ordine delle righe;
- nomi di nodi DOT;
- formato CSV o parsing dei campi;
- valori di contatori come `ct_read`, `ct_cmp`, `ct_swap`, `ct_op`, `ct_visit`.

Quando si estrae una funzione, mantenere nello stesso punto logico le stampe e gli incrementi dei contatori.

## Quando si e' incerti

In caso di dubbio:

- conservare il codice esistente;
- ridurre la dimensione della modifica;
- non introdurre nuove astrazioni;
- non cambiare algoritmo;
- non cambiare strutture dati;
- non modificare commenti;
- chiedere chiarimento se la richiesta implica possibile cambio di comportamento.

La direzione corretta e' rendere il progetto piu' ordinato mantenendo intatta la logica originale.
