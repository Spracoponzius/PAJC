# 4 dic

# Segmentazione interi a 32 bit

Unsigned int significa intero a 32 bit.  
Nei microcontrollori si possono avere messaggi rappresentati da 4 byte, ognuno dei quali ha qualche significato.  
Nei sistemi unix ogni file ha dei flag che indicano se la lettura/scrittura abilitata. Fatto sta, l'intero a 32 bit può essere segmentato e assegnare a ogni byte un significato.  

Come faccio p.e. a estrarre il byte 0?  

Così come per estrarre unità, decine, ... da un numero intero normale faccio il resto della divisione per 10, ecc., il codice potrebbe essere:

```
int cifra = value % 256;
System.out.printf("\n%x",cifra);
value /= 256;
``` 

Così ottengo i valori esadecimali che rappresentano quell'intero.

## Operazione di shift (`<<` `>>`)

Molto comoda perchè il processore lo implementa a livello hardware: impiega un ciclo di clock. Dividere per un numero significa occupare diversi cicli di clock. (**Preservare le risorse!**)  

In Java voglio estrarre gli 8 bit meno significativi: come posso fare?  

Gli operatori AND, OR e NOT binari confrontano i singoli bit. Facendo l'AND tra il valore e 0xFF (tutti i bit da 0 a 7 sono 1), passano solo i primi 8 bit.

```
int cifra = value & 0xFF;
System.out.printf("\n%x",cifra);
value = value >> 8;
``` 

> **N.B.: 0xFF e 0x00000FF vogliono dire la stessa cosa: i bit a sinistra messi a 0 sono ignorati.** 

## Selezionare un byte particolare

Voglio prendere il secondo byte.  

### Opzione zero: ciclo while

Si potrebbe, ma assolutamente no, perchè mai vorresti?

### Prima opzione (non ottimale): puntatori

Prendo un puntatore e lo faccio puntare all'indirizzo di value (quindi al bit più o meno significativo, dipende dal processore). Supponendo che punti al bit meno significativo, se eseguo p++ punta ad un bit più significativo o altrove?  
Nessuno dei due: p++ va avanti di tanti bit quanti sono quelli del tipo dato.
```
int value = 123456;

unsigned char *p_byte = (unsigned char *)&value;
printf("\n%x",*(p_byte+1);
``` 
Ma non è un modo "economico".

### Seconda opzione: `>>` (Operatori bit a bit)

L'operatore & è un **operatore di mascheramento**. Per far passare solo il secondo byte posso fare **[valore] & 0xFF00**.  
La maschera fa passare i bit solo dove la maschera è attiva, ovvero dove c'è FF, dove ci sono tutti 1.

```
int value = 123456;
//int res = (value & 0xFF00) >> 8;
int res = (value >> 8) & 0xFF;      //Le due righe sopra e sotto sono equivalenti
printf("\n%x", res);
``` 

### Terza opzione: uso delle union

E' uno strumento presente in pochi linguaggi, tra cui il C, ma molto potente.

# Union in C

Le Union in C sono particolari struct, in cui lo stesso blocco di memoria viene fatto condividere per tutte le entità. In questo modo posso fare la decodifica dei dati in maniera diretta. La dimensione della union è quella del tipo dati più grande.
  
Attraverso questa proprietà, posso sfruttarla per decodificare e anche modificare. (vedi test_union.c)

```
union byte_decoder{
    unsigned int value;
    unsigned char byte[4];
};
```
La condivisione della memoria è comoda per costruire tipi dati complessi per risparmiare risorse.

## Campi di bit

La union può essere anche molto articolata. Supponiamo un intero in cui ogni byte abbia i seguenti significati: HEADER, TYPE, LENGTH, CHECKSUM.  
Nulla mi vieta di dichiarare nella union tipi dati complessi.

```
union byte_decoder{
    unsigned int value;
    unsigned char byte[4];
    struct{
        unsigned char header;
        unsigned char type;
        unsigned char length;
        unsigned char checksum;
    } msg;
};
```
msg è una struct composta da una proprietà composta da un byte. Sono gli stessi byte del value. Posso accedere alle stesse proprietà allo stesso modo. (decoder.msg.length)  
Posso anche specificare di quanti byte è il tipo.  
Per esempio, i primi 7 bit dell'ultimo byte sono il vero checksum e l'ultimo bit rappresenta un errore. Per verificare se ho errore:

```
decoder.msg.checksum & 0x1;
```
Modifico la struct per avere un bit di errore:

```
union byte_decoder{
    unsigned int value;
    unsigned char byte[4];
    struct{
        unsigned char error : 1;
        unsigned char header : 7;
        unsigned char type;
        unsigned char length;
        unsigned char checksum;
    } msg;
};
```
I due punti così indicano quanti bit quel campo occupa. Sto specificando i singoli gruppi di bit.  

### Memorandum sulle regole di dichiarazione delle struct

Anche per le union valgono le regole delle struct: quando scrivo il tipo devo scrivere **"union byte_decoder"**.  
Mettere byte_decoder alla fine della graffa senza typedef significa dichiarare una variabile.  
Mettere byte_decoder alla fine della graffa con typedef e solo "typedef union" all'inizio è valido: specifico il tipo dati.

# Funzioni in C come tipo dati

In C non esistono Lambda function, non si usa dichiarare blocchi di codice inline (a meno di usare macro... ma va oltre lo scopo del corso).  
Voglio ugualmente passare alla funzione un comportamento. Per l'elevamento a potenza c'è una parte comune: un ciclo.  
In Java si usano le lambda function, che sono anche oggetti che rispettano un'interfaccia. Passo un oggetto con un certo comportamento.  
In C, non avendo oggetti, non posso farlo.  
In Java la parte semplice era con l'interfaccia operatore a cui veniva passata l'operazione.  

I nomi delle funzioni chiamate nel main sono puntatori ad un'area di memoria in cui risiedono le istruzioni della funzione dichiarata. Istruisco l'Instruction Pointer a saltare a quell'indirizzo.  

Di conseguenza, l'unica differenza tra due chiamate sono il puntatore alla data area di memoria.  

Come salvo un puntatore?  

```
(funzione descritta normalmente)

int (*operatore)(int, int);
operatore = somma;

int res = operatore(a,b);
printf("\nOperatore: %d", operatore(a,b));
```
Operatore ha in sè il comportamento che voglio far implementare al mio sistema. Mima abbastanza il concetto di interfaccia, ma è solo la locazione in memoria dove inizia il blocco di codice.

## Un esempio più articolato

Dichiaro una funzione elabora. Non voglio solo fare somme, ma voglio passare ad elabora un qualsiasi blocco di codice. Si tratta solo di giocare con la sintassi.
> N.B. sizeof(...) restituisce il valore intero dei byte occupati dalla variabile.  
Quindi per la lunghezza del singolo elemento in un array si farà: sizeof(array) / sizeof(array[0]);


```
void elabora(int *a, int *b, int len, int (*operatore)(int,int)){
    for(int i=0;i`<`len;i++){
        int res = operatore(a[i],b[i]);
        printf("\n%d op %d = %d, a[i],b[i], res);
    }
}

int main(){
    int a[] = {1,2,3,4,5};
    int b[] = {10,20,30,40,50};

    elabora(a,b,5);
}
```
Non ho fatto altro che passare il puntatore del blocco di codice. Sono riuscito a portare in una callback esterna una funzione (un comportamento). Ciò mi permette di fare un bel salto in flessibilità del linguaggio C. Bisogna stare attenti comunque ai puntatori per evitare dei crash.  

### Uso di typedef per semplificare la dichiarazione del puntatore a funzione

```
typedef int (*operatore_type)(int,int);
```
Utile per evitare tutte le parentesi nelle dichiarazioni delle funzioni. **Tutte** le funzioni che restituiscono lo stesso tipo dati e accettano gli stessi parametri sono valide.

### Passare anche altro

Ho dovuto usare "op" perchè non so che operazione andrò a fare. Vorrei scrivere il simbolo dell'operazione, quindi vorrei passare operatore_type e un char che mi rappresenta il simbolo.  
Potrei inventarmi che al posto di passare due parametri, ne passo uno solo: uso le struct.  

```
typedef struct{
    operatore_type operatore;
    char symbol;
}operatore;

void elabora(int *a, int *b, int len, operatore operatore)){
    for(int i=0;i`<`len;i++){
        int res = operatore(a[i],b[i]);
        printf("\n%d %c %d = %d, a[i],operatore.symbol, b[i], res);
    }
}
```
Non è in realtà nulla di nuovo: è un incapsulamento.

## HashMap in C

In Java implementare una HashMap è banale, in C è più complesso. Ma se voglio fare p.e. una calcolatrice in cui ad ogni operatore è associato un simbolo una HashMap non serve.  

**Prova ad associare ad ogni operatore un simbolo con strutture dati semplici, in modo che passo ad una funzione i dati e il simbolo. Serve abbinare l'elemento dell'array all'operatore.**
