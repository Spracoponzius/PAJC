# 27 novembre

# Thread nell'applicazione dei frattali

Quando la viewport viene spostata, vengono calcolati a bassa risoluzione i frattali, nel momento in cui si ferma viene calcolato a piena risoluzione.  

Ogni volta che si sottometteva un task ad un future, si memorizzavano i numeri primi. Dovrei fare lo stesso anche per i frattali.  

Mentre faccio il pan della figura, non mi interessa di continuare il calcolo della figura in una vecchia posizione. Devo quindi cancellare i thread.  

## Ottimizzazione del pan con Futures e Thread.interrupted()

Creo un ArrayList di Futures chiamato pendingTask. Come faccio al thread di dirgli "shutdown"?

```
for(Future(?) f : pendingTask)
    f.cancel(true);
``` 
Il task viene tolto dalla coda. Se invece è in esecuzione in un thread, può mandare un messaggio al thread di terminarsi, con **true** succede questo.  

Ora voglio intercettare che il task è stato interrotto. Diversamente verrà eseguito e il risultato verrà buttato. Il messaggio *cancel* attiva un flag. Questo flag si chiama *Thread.interrupted()*. In un if-else con questa condizione si può mettere un return, ma questo si può fare solo in questo caso semplice con modifiche locali, se invece ci fosse una lettura/scrittura su disco, vanno prima chiusi gli stream. Per evitare di sporcare i dati, si mette la stessa condizione, ma negata, nel blocco synchronized come failsafe (anche se la probabilità è pressochè nulla, prima o poi capiterà di sicuro perchè siamo nel mondo dei thread).  

Uno shutdown corretto avviene quando si esce dal metodo *eval* senza perdere dati.

## Aggiorare un metodo di interfaccia

Quando chiamo un metodo della GUI, la chiamata può venire da un evento grafico (mouse click), ma un repaint() può arrivare da un thread non grafico. Java.awt ricorda che i thread grafici non sono sicuri, quindi si sincronizza con **isDispatchThread()**. Se è un thread grafico, va subito con run(), altrimenti si fa **SwingUtilities.invokeLater(task)**;

# Direttive in C: recap

Una direttiva (cose che inizano con #) è una costante che viene elaborata dal precompilatore, che viene elaborata prima della compilazione. E' una sorta di macro. #define sostituisce nel codice una costante ogni volta che appare. La direttiva definisce una costante o un comportamento.  

Se voglio far funzionare il programma su diverse CPU, voglio avere blocchi di codice che si compilano in funzioni di queste direttive.  

Direttive comuni:

- **#include**: permette di includere un file header. Il contenuto del file p.e. stdio.h viene appiccicato prima dell'inizio del mio file.
- **#define**: permette di dichiarare una macro o una costante. Alla compilazione del codice, ogni istanza della costante dichiarata viene sostituita con quanto scritto nella define. E' sostanzialmente un trova-sostituisci. L'espansione delle macro può cambiare completamente il comportamento del programma. 
- **#undef -nome macro-**: elimina la definizione di una macro precedentemente definita.

## Macro parametrica

```
#define SQUARE(X) ((X)*(X))
```
Posso fare il quadrato di un intero nella stessa linea di codice (int q = x*x), una funzione square(int x) dedicata, MA non ho l'overloading dei metodi come Java. Non posso farla per i double. Dedicare una funzione (quindi caricare nello stack i parametri, spostare Instruction Pointer, ...)  per un'operazione semplice come un quadrato è un enorme spreco. Per standardizzare uno squadre o un elevamento a potenza si fa una macro parametrica come scritta sopra.  

x è l'espansione della macro. Ci può essere un numero o una qualsiasi espressione anche molto complessa. x viene sostituito da x*x.  

L'espansione prevede anche comportamenti diversi da quelli che assume un metodo. Per esempio, SQUARE(3+2) viene espanso come 3+2*3+2 se la define è SQUARE(X) X*X, che fa 11 e non 25 come ci aspetteremmo. Bisogna mettere ((X)*(X)). 

N.B. Non ci vanno spazi dopo SQUARE. Le macro sono solitamente scritte tutte in maiuscolo. E' indipendente dal tipo, ma ***non c'entra niente*** con il polimorfismo. Le macro **non** sono funzioni. Una macro può essere dichiarata nel codice anche all'interno di un metodo.

## Compilazione condizionata con le macro

Voglio eseguire delle particolari istruzioni quando sono in debug, e altre quando la versione è di release.  
- **#ifdef -NOME-**: se è stata definita una macro che si chiama NOME, viene eseguita quella porzione di codice. Di solito viene fatto per iniettare del codice di test.
- **#else**: passa ad un altro ramo di codice dopo il blocco *#ifdef*
- **#endif**: chiude il blocco iniziato con *#ifdef*
- **#ifndef**: da usare **sempre** quando si usano file include personalizzati. Si chiude con *#endif*

## Direttive particolari

- **#error**: inietta nel sorgente un'istruzione che **non fa compilare**
- **#pragma -parametro-**: passo particolari istruzioni al compilatore

## Assert

Verifica se una condizione è vera. Se è falsa, genera un qualche tipo di errore.
```
//#define NDEBUG
#define DEBUG
#include (assert.h>
...
...
...
assert(1==1);
assert(1==0);
```
Le assertion vere non interagiscono con il sistema. Le assertion false generano errori a runtime e restituiscono informazioni. E' un metodo molto potente che può informare dove è fallita un'istruzione e a quale riga di codice. E' molto comodo nel debugging e viene attivata **solo** se si è in modalità DEBUG. Per non debuggare, si usa **#define NDEBUG**. In questo caso, assert punta ad una funzione che non fa niente e gli assert non vengono espansi.  

## Macro predefinite

Esistono direttive predefinite utili caratterizzati da doppi underscore all'inizio e alla fine:
- **__FILE __**
- **__DATE __**
- **__LINE __**: restituisce il numero di riga

Plugin consigliato per VSCode: Code Runner

# Code in C

In Java le code sono oggetti specifici. In C li devo costruire. Una coda è un buffer che ha un insieme di elementi in cui il primo inserito è anche il primo a essere estratto (FIFO). In C esistono tipi base, struct, array e union. I buffer contengono tanti elementi dello stesso tipo, quindi un array va bene. La dimensione è predefinita. Come riempio e svuoto questo buffer? Per accedere agli elementi del buffer devo avere un paio di indici o puntatori, uno all'ultimo elemento inserito e l'altro al primo elemento in coda.  
Le operazioni sono c_ret() e e c_ins(). 

Quando gli indici di insert e retrieve coincidono, il buffer è vuoto.  

N.B. Non ho accesso diretto agli elementi della coda.

## Implementazioni

```
#include < stdio.h>
.
.
.

```
Coda circolare: quando arrivo alla fine del buffer, allora posso ripartire dall'inizio stando attendo che l'indice di scrittura non sovvrascriva quello di lettura.

# Stack in C

Uno stack, oltre a essere il meccanismo base di passaggio dati, è un tipo di organizzazione memoria di tipo LIFO. Estraggo l'ultimo elemento a essere inserito. Dal punto di vista di cosa memorizzo, è sempre un array, ma cambia la logica con cui inserisco ed estraggo.

Per convenzione, si usa la nomenclatura push e pop.  
Prendiamo uno stack di interi. Se nello stack volessi registrare altre informazioni, come delle matrici, non devo modificare la gestione dello stack, ma il tipo dati definito.  
Ma se voglio gestire tipologie di dati diversi, uso i puntatori. Il puntatore è un elemento di dimensione fissa (oggi 64 bit).  
Nelo stack inserisco puntatori ad aree di memoria. Il tipo dati a questo punto può essere qualsiasi cosa. Cambia solo il dato estratto.  
I puntatori generici in C sono i puntatori void*.

# Liste concatenate

I puntatori generici sono molto utili con le liste concatenate. A differenza di code e stack, la dimensione non è fissa (tipo ArrayList).  
Come faccio a variare la dimensione?  

Voglio creare un ArrayList di matrici. Le matrici sono oggetti, che risiedono nell'heap. Mi riferisco a questi oggetti tramite puntatori. Voglio creare una lista di queste matrici, ma non posso sapere quante matrici posso contenere.  
Un modo: creo un array dinamico, alloco un array di 3 elementi, aggiungendo un elemento creo un array con 4 elementi e faccio puntare il quarto elemento alla matrice.  

Ciò fa frammentare incredibilmente la memoria ed è molto lento.  

**Modo più furbo**: A seconda di come è configurato il sistema, espando di un blocco di elementi. Sprecherò un po' di spazio, ma ottimizzo.  

**Altro sistema: Lista concatenata**: permette di non sprecare troppa memoria. Creo un elemento lista che contiene un puntatore all'elemento desiderato e un puntatore al prossimo blocco che contiene i dati della lista.   
Nelle liste concatenate semplici, l'elemento trattato contiene i dati, un puntatore all'elemento successivo, in quelle doppie contiene anche un puntatore all'elemento precedente. In questo modo, la matrice che volevo memorizzare è il *payload* della lista.  

Voglio distinguere il payload dalla gestione della lista.
-  Inserire il nuovo elemento in testa alla lista: il puntatore start punta a questo nuovo elemento e questo elemento punta a quello che era puntato da start.  
- Inserire in fondo alla coda: aggiorno il puntatore dell'ultimo elemento
- Inserire in un punto qualsiasi: aggiorno il puntatore successivo dell'elemento precedente e il puntatore precedente dell'elemento successivo

Quello che faccio è giocare con dei puntatori. Non devo copiare indirizzi, devo solo organizzarli correttamente.  

La gestione della cancellazione è molto simile.  

Possiamo gestire anche liste lunghe centinaia di migliaia di elementi, non impiega troppe risorse se non memoria.
