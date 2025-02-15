# 18 settembre

# PROGRAMMA HELLO WORLD

### JAVA

```
package ...

public class HelloWorld{
	public static void main(String[] args){
		
		System.out.println("Hello world from JAVA");
	}
}
```

In Java il metodo main è l'entry point del programma in una struttura più complessa del C.
In Java non posso assolutamente dichiarare il metodo main se non in una classe.

Un metodo statico è associato alla classe, non ad un oggetto. Tutti gli oggetti lo condividono
Posso chiamarli sulla classe stessa.

Affinchè sia un entry point, deve essere pubblico e statico.

Cosa rappresenta System.out? System è una classe che ha una proprietà (out) pubblica e statica.
 
### C

```
#include `<`stdio.h>

int main(){
	printf("Hello world from C!\n");
	return 0;
}
```

In C il main è l'entry point del programma: in un file oggetto deve essere dichiarato un main


## COSTRUIAMO UN RETTANGOLO
Voglio costruire 25 rettangoli di dimensioni tutte diverse, affinchè possa visualizzare una
lista?

Come risolvo il problema in C?
Posso creare una struct. In particolare, un array di struct.

```
typedef struct Rectangle_ST{
	int width;
	int height;
} Rectangle_T;

....

int main(void){
	Rectangle_T r;
	r.height = 5;
	r.width = 8;
	...
}
```

Non posso dichiarare il rettangolo solo con Rectangle_ST r;  
Manca qualcosa:
*struct Rectangle_ST r;*

questo è il nome completo del tipo.

typedef è una parola chiave che permette di dichiarare degli alias.

```
typedef struct mystruct{
	...
}mystruct_t;
```

struct mystruct assumerà un nome alternativo che viene specificato dopo le graffe.
E' comodo avere degli alias così posso scrivere mystruct_t al posto di scrivere sempre
struct mystruct....

mystruct_t è una variabile. Eseguendo il codice scritto sopra, compila.

In C una stringa non è solo un array di caratteri, ha alla fine un terminatore null. Senza
quello abbiamo un array di caratteri.

Come risolvo in problema in JAVA?
L'equivalente delle strutture in Java è la classe.

Esempio brutto forte:
```
public class Rectangle{
	public int width;
	public int height;
}

public class Main{
	public static int perimeter(Rectangle r){
		return(r.height + r.width)*2;
	}

	public static void main(String[] args){
		Rectangle r  = new Rectangle();
		r.height = 5;
        ...
}
```

**Problemi:** 
- Non ho inizializzato width e height;
- new Rectangle crea una classe
- troppi static!

In realtà non mi serve inizializzare. Viene messo a 0.

In C le variabili sono memorizzate nello stack, con l'allocazione dinamica viene allocato nell'heap.

In Java finchè non scrivo new non alloca nell'heap gli oggetti su cui stiamo andando a lavorare.
Java ha un vantaggio enorme nella gestione di memoria dinamica.
In C è molto più complicata e il programmatore si occupa della gestione dinamica.

I troppi static sono una schifezza. Quei metodi static non fanno parte del paradigma della programmazione ad oggetti.
I metodi static sono dei casi limite.

## OPERATORE NEW

L'operatore new crea un'istanza della classe, chiamando il costruttore.
Se una classe non dichiara costruttori, Java mette un costruttore implicito che mette a 0 tutte le variabili.
Il costruttore implicito esiste SOLO se non esistono costruttori.
Ma posso creare un altro public Rectangle() che non fa nulla.
Posso utilizzare entrambi grazie all'overloading (in C non esiste overloading).

Posso usare la parola chiave this per riferirmi all'oggetto stesso.

### QUADRATI

Il quadrato è un rettangolo con proprietà particolari. Posso creare una classe derivata.

Con la parola chiave super chiamo il costruttore della superclasse.
Molto molto utile per la manutenzione del codice. Incapsulare è fondamentale.

**DRY: Don't Repeat Yourself**  
Non copia-incollare il codice. Genera solo problemi.

**Single inheritance**: tutti gli oggetti sono derivati da Object.

**Reflection**: tecnica di programmazione che interroga gli oggetti nel programma

In una classe derivata è possibile scrivere una nuova versione di un metodo
della superclasse con lo stesso nome, facendo così un override.

```
Rectangle s = new Square(s);
s.perimeter();
```

Se a perimeter viene fatto un override, s.perimeter chiama il metodo perimeter di Square.
La risoluzione dei metodi avviene a runtime.

### DIAMOND

Voglio costruire oggetti di tipo rombo.  
Non sono nè rettangoli nè quadrati, non posso derivare.

Se modifico una catena di derivazione, ho ripercussioni su tutto il resto della catena.

Si usano le interfacce: aderenza ad un protocollo.

Se incontro tante cose diverse ma rispondono a delle caratteristiche comuni
(tutti hanno un perimetro, un'area, un metodo per disegnare), dico che le classi
hanno un'interfaccia. I miei oggetti hanno tutti quei metodi.

A differenza della derivazione, una classe può implementare più interfacce.

Posso avere dei container generici (come ArrayList) che dicono che dato sarà inserito all'interno della lista.

```
ArrayList`<`Shape2D> oggettiTrovati = new ...
//ciò significa che posso inserire nella lista:
oggettiTrovati.add(new Square(5));
```

Come mi relaziono con altri oggetti?
Ho un'astronave che condivide solo il metodo draw con le forme.
Anche le interfacce si possono derivare. Posso far derivare Shape2D da un'interfaccia Drawable.

```
Ora posso fare ArrayList`<`Drawable> oggettiTrovati = new ...
```
Ora voglio fare un for each in questa lista che per ogni forma mi dice il suo perimetro.

```
Object o = new Square(5);
((Square) o).area();		//Così forzo il tipo Square (casting)

/*
 *  Prima di fare un casting, posso chiedere a runtime a che interfaccia 
 *  corrisponde con la parola chiave instanceof.
 */

if(s instanceof Shape2d){
	perimetro = ((Shape2d)s.perimeter);
}

//Così proteggo ancora a compiletime.
```

### CONFRONTO RUNTIME

Ci permette di verificare se i due oggetti a runtime sono derivati dalla stessa classe.

### ESERCIZI SU SLIDE

## COLLEZIONI

- LISTE: elenchi di oggetti in cui l'ordine viene rispettato (ArrayList, Vector)
- STACK: Strutture LIFO					    (Stack)
- SET: insiemi di oggetti (senza ripetizioni)		    (TreeSet)
- MAPPE: dizionari chiave-oggetto con associazioni univoche  (HashMap)

Come posso contare quanti rettangoli ho in una lista?
Evita assolutamente di cablare tutto con indici diversi per ogni tipo nel for-each.

Posso creare una HashMap in cui le chiavi sono i tipi e i valori le occorrenze di quel tipo.
```
HashMap`<`String, Integer> gruppi = new HashMap`<`>();
```
Object ha il metodo getClass(). Dato un oggetto qualsiasi, restituisce il tipo di oggetto
in forma di riferimento. Però posso farne il toString.

```
for(Shape2d s : lista){
	tipoOggetto = s.getClass();
	if(gruppi.containsKey(tipoOggetto){
		noggetti = gruppi.get(tipoOggetto);
		gruppi.put(tipoOggetto, noggetti+1);
	}
}
```

PROVA A SCRIVERE UN PROGRAMMA JAVA CHE IMPLEMENTA CIRCLE E RECTANGLE, COMPLETA IL CICLO SOPRA
VISUALIZZANDO IL NUMERO DI FORME E IL LORO TIPO.

PROVA A SCRIVERE UN PROGRAMMA CHE USA LE HASHMAP PER SIMULARE UN COMPORTAMENTO (???)
