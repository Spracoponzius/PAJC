# 20 novembre

# Stack

In Java lo stack contiene le reference (i puntatori) agli oggetti.

# Heap

## Free del blocco

Il pezzo di memoria allocato nell'heap viene buttato fuori. Nell'heap si crea come un buco e si frammenta (come negli HDD).  
In **C**:
- malloc di 15 interi: trovo il primo spazio vuoto e lo colloca. Si cerca di ottimizzare la memoria, ma ciò porta alla frammentazione della memoria: ho porzioni di memoria allocate con piccoli spazi vuoti (poche decine di byte) talmente piccoli che non possono essere utilizzati. Se allocare o meno, è a carico del programmatore.  

In **Java**:
- Come so che uno spazio è libero? Prendo le reference nello stack e verifica che sia referenziato a qualcosa. Se non referenzia niente, lo spazio è libero.  
Ciò è molto lento ad ogni new: quindi Java alloca in testa all'heap indipendentemente se ci sono spazi liberi in mezzo.  
- Entra in gioco il garbage collector: quando il framework Java si accorge che l'applicazione occupa molta RAM, parte la garbage collection.  
- Un'applicazione Java alloca memoria lentamente, fa un picco e poi crolla. Il picco è dovuto alla duplicazione dell'heap (heap di copia). Qui vengono copiati gli oggetti referenziati nello stack. Questo nuovo heap non ha frammenti, o spazi liberi. Successivamente il vecchio heap viene cancellato e l'uso della memoria crolla. L'utente non si rende conto di questo processo.  

In C questo meccanismo è molto più complesso perchè il puntatore punta comunque al vecchio heap: dovrei modificare tutti i puntatori. In Java questo puntatore **punta ad un altro puntatore** (in una tabella la cui chiave è la reference e il valore è l'indirizzo di memoria) che a sua volta punta all'area di memoria dell'oggetto. La reference non cambia.  
In C non si usa lo stesso meccanismo di Java di GC, si usano meccanismi più semplici, come contatori che incrementano o decrementano a seconda degli oggetti allocati o distrutti.  

## Forza entrata GC

System.gc() forza l'entrata del GC. In realtà non si usa mai chiamare direttamente la GC, perchè il framework è già ottimizzato. Può essere utile per fare debugging.  

In C++ esistono i distruttori (la distruzione di un oggetto è prevedibile), in Java esiste un metodo finalize() che viene chiamato dal GC, ma non andrebbe usata perchè il GC viene chiamato solo in condizioni di memoria piena, non si sa mai quando viene chiamato finalize().

# Multipiattaforma

Come Java gestisce il suo runtime?  
In **C** passo da un sorgente, lo compilo (il compilato è comprensibile dallo specifico processore ed è un file .o, .object), metto insieme i file oggetti con le librerie già compilate (linking) e passo ad un eseguibile. Ciò va fatto per diverse piattaforme, con diverse librerie, ...  
Questo è il limite del C. Alcune librerie sono legate al sistema operativo, come quella di I/O per Windows è diversa da quella di MacOS.  

In **Java**, il sorgente viene compilato in un linguaggio "bytecode", uno pseudo linguaggio macchina che non è legato ad uno specifico processore o SO, ma ad una macchina virtuale. Creo un framework di base che installo su ogni SO. In questo modo i programmi non funzionano interagendo con il SO, ma funzionano interagendo con la macchina virtuale. La macchina virtuale è mantenuta omogenea tra sistemi operativi. Sposto il collaudo del funzionamento dal collaudare l'applicazione al colluadare la macchina virtuale.  

Java introduce un livello di astrazione che sposta dal sistema operativo, introducendo potenziali ritardi. La VM Java non lavora su codice Java. Lavora su bytecode, quindi potrei anche partire da linguaggi diversi (es. Ruby). Ma non è un'idea che si è mai veramente sviluppata e Java resta dominante come codice che viene compilato in bytecode da eseguire in macchina virtuale.  

Come in C, esiste una mappatura 1:1 tra i listati e i file compilati (.class).  

## JIT compiling

I programmi interpretati sono molto più lenti dei programmi compilati in caso di calcoli ad alta complessità computazionale. C però non supera di molto Java in velocità al giorno d'oggi: la JVM non **solo** interpreta il bytecode, fa **Just In Time compiling**, ottimizzando il bytecode per il particolare sistema. Le performance si avvicinano a quelle di un linguaggio nativo. 

## Reverse engineering di un eseguibile

In C è un'operazione molto complessa passare da .o a .c, dipende dalle ottimizzazioni del compilatore. E' sempre però possibile passare ad un listato assembly.  

In Java si può passare facilmente da .class a .java, esistono programmi appositi (jdec.app). **Il Bytecode non è un modo sicuro per proteggere il codice, servono degli offuscatori, ma non è detto che funzionino sempre bene, anzi, sono anche facili da bypassare**. Se voglio proteggere il mio codice Java, non lo distribuisco. Si accede solo da API esterna.

# Code, stack e liste in C

In C l'astrazione sale da interi, a float, a strutture, fino ad arrivare ai **motori di elaborazione**. Che oggetti di base andiamo a utilizzare?

## Struttura dati in C

Voglio creare una struttura dati per gestire matrici. Le matrici possono contenere un solo tipo dati, con un certo numero di righe e un certo numero di colonne.

```
print_matrix(int *p, int nr, int nc);
```
Questo codice stampa una matrice. Esso implementa due cicli for. Vorrei implementare il metodo senza passare una sfilza di parametri, che accetti solo la matrice e a prescindere dalla taglia. Uso le struct.  

```
struct matrix{
    int rows;
    int cols;
    double *data;      //puntatore al tipo base, MA voglio che il tipo dati sia un parametro
};
```
**File "matrix.c":**
```
#include (stdio.h>                  //direttiva al compilatore: non viene compilata, viene trasformata
                                    //prima di passare al compilatore.
                                    //Sostanzialmente è una macro che si espande
#ifndef MATRIX_ELEMENT_TYPE
#define MATRIX_ELEMENT_TYPE int     
#endif

/**  verifica che qualcuno abbia già definito un MATRIX_ELEMENT_TYPE: se no, allora esegue
  *  quanto si trova nel blocco (se verifica la condizione opposta, si usa #ifdef)
  */

typedef MATRIX_ELEMENT_TYPE matrix_type;

typedef struct matrix{
    int rows;
    int cols;
    matrix_type *data;     
}matrix;                                //E' una normalissima dichiarazione C, matrix è scritto due volte 
                                        //ma vogliono dire due cose diverse
//E' MOLTO consigliabile creare dei costruttori per questi tipi dati complessi, specialmente se si lavora
//con puntatori

matrix* mtx_create(int rows, int cols){
   
    /* matrix m = {rows, cols, NULL);
    m.data = malloc(rows * cols * sizeof(matrix_type));
    return m;                                             return &m; NON E' LA CORREZIONE GIUSTA! 
                                                            LO STACK VIENE DISTRUTTO APPENA USCITO 
                                                            DA MTX_CREATE,  RUNTIME ERROR
    */
    matrix *pm = malloc(sizeof(matrix));
    pm->rows = rows;
    pm->cols = cols;
    pm->data = malloc(rows * cols * sizeof(matrix_type);
    return pm;
}

void mtx_print(matrix* m){
    matrix_type *p = m->data;
    for(int i=0;i (min)m->rows;i++){
       printf("\n");
       for(int j=0;j(min) m->cols;j++){
           //printf("%2d\t",m.>data[i*m->cols + j]);        //N.B. moltiplicazioni e addizioni sono costose
           printf("%2d\t", *p++);                           //usa il post-incremento per scorrere l'array
    }
 }
}
```
Ricorda: passare una struttura significa passare **una copia** della struttura. Al posto di passare inutilmente dati, passo puntatori.  
**File "matrix-test.c":**
```
#include <stdio.h>
#define MATRIX_ELEMENT_TYPE double          //specifico che voglio il double **prima** dell'include
#include "matrix.c"

int main(){
    matrix m = mtx_create(3,3);

    for(int i=0;i (min)m.rows;i++){
       for(int j=0;j(min) m.cols;j++){
            m->data[i*m->cols + j] = i*m->cols + j;
        }
    }

    mtx_print(m);
    return 0;
}

```
#define è **molto** più potente del semplice dichiarare costanti. In questo caso, è utile per generalizzare tipo dati.

Ho usato due volte il malloc (allocazione dinamica). Per consentire la gestione efficiente, bisogna creare dei metodi distruttori. Così com'è, il debugger segnala che non ho deallocato memoria. Scrivere:
```
free(m);
return 0;
```

Porta a runtime error. Quindi si fa:
```
void mtx_free(matrix *m){
    free(m->data);
    m->data = NULL;
    m->rows = -1;
    m->cols = -1;
    free(m);
}

mtx_free(m);
```
Annullo i dati così se rimane un puntatore che punta ancora a quell'area di memoria, ho almeno un errore esplicito.  

IMPLEMENTA SOMMA DI MATRICI
