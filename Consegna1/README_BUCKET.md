# Bucket Sort
## idea
1. Viene diviso l'intervallo dei valori in 614 bucket
2. Per ogni elemento dell'array `A[i]`, viene stabilito in che bucket inserirlo
3. All'interno di ogni bucket viene assicurato un ordinamento crescente, mediante una variante un algoritmo di ordinamento dedicato
4. Infine, i bucket vengono concatenati in ordine crescente

Non è necessario un Merge, in quanto è assicurato che i valori di `bucket[i] <= bucket[i + 1]`
* In particolare, è assicurato che `max(bucket[i]) <= min(bucket[j])`, per ogni `i < j`



## Funzionamento (lato codice)

### Dichiarazioni iniziali
`#define NUM_BUCKET 614` --> ci sono 614 bucket
`#define BUCKET_CAP 35` --> ogni bucket può contenere al più 35 elementi

`int buckets[NUM_BUCKET][i]` --> contiene *l'i-esimo* elemento del bucket numero *NUM_BUCKET*
`int bucket_count[NUM_BUCKET]` --> memorizza quanti elementi ci sono nel bucket *NUM_BUCKET*


### Analisi sul numero e range dei bucket
La **distribuzione degli elementi** all'interno dei bucket **non è uniforme**:
* Parte densa nel dataset --> si utilizzano più buckets, **così che ogni bucket di quella zona contenga meno elementi rispetto ad una distribuzione classica**
	* (E' proprio questo il principale vantaggio)
* Parte poco densa nel dataset --> **si utilizzano meno buckets**

Quindi, ci saranno alcuni bucket con un range più stretto ed altri con range più ampi.

#### Calcolo del bucket per ogni elemento (16-segment)
La funzione `map_to_bucket(v)` divide il range dei valori in **16 segmenti non lineari**.
Ogni segmento usa:
1. un sotto-intervallo di valori
2. un blocco dedicato di bucket
3. una larghezza diversa (`/66`, `/25`, `/24`, ...), scelta in base alla densità locale del dataset

Schema usato nel codice:
1. `v <= 939` --> bucket `0..39` con passo `66`
2. `940..1876` --> bucket `40..77` con passo `25`
3. `1877..2803` --> bucket `78..116` con passo `24`
4. `2804..3708` --> bucket `117..155` con passo `24`
5. `3709..4586` --> bucket `156..194` con passo `23`
6. `4587..5430` --> bucket `195..233` con passo `22`
7. `5431..6228` --> bucket `234..272` con passo `21`
8. `6229..6976` --> bucket `273..311` con passo `20`
9. `6977..7667` --> bucket `312..349` con passo `19`
10. `7668..8288` --> bucket `350..388` con passo `16`
11. `8289..8805` --> bucket `389..426` con passo `14`
12. `8806..9218` --> bucket `427..465` con passo `11`
13. `9219..9515` --> bucket `466..504` con passo `8`
14. `9516..9743` --> bucket `505..542` con passo `6`
15. `9744..10037` --> bucket `543..581` con passo `8`
16. `>= 10038` --> bucket `582..613` con passo `15`

Per evitare overflow agli estremi, ogni formula viene "clampata" con `support_map(..., lo, hi)`.

### Ordinamento locale degli elementi in ogni bucket
Viene utilizzato l'algoritmo **Gnome Sort** in versione di inserimento locale (`gnome_sort(bucket, count, value, stats)`).

**Funzionamento**:
1. Il nuovo valore viene inserito in coda al bucket: `bucket[count] = value`
2. Si confronta con l'elemento a sinistra (`bucket[pos - 1]`)
3. Se a sinistra c'è un valore più grande, lo si sposta a destra e si continua a retrocedere
4. Appena `left <= value`, significa che abbiamo trovato la posizione finale dell'elemento corrente e quindi ci si ferma
5. Il valore viene scritto nella posizione finale `bucket[pos]`

**Perché è adatto qui**:
* I bucket contengono pochi elementi (`BUCKET_CAP = 35`), quindi il costo locale resta basso
* Non serve ordinare grandi blocchi: ogni inserimento mantiene già ordinato il singolo bucket
* Si riducono le letture globali, perché si lavora su sotto-array piccoli invece che sull'intero array
