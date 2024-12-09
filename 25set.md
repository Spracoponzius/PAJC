# 25 settembre

# STREAM I/O

## LETTURA

Perchè mi serve un incapsulamento di oggetti BufferedReader r = new BufferedReader(new FileReader = new File(....?

Per il sistema operativo sto scrivendo dei dati, se cambio l'oggetto target cambio la direzione del flusso.
E' un incapsulamento di stream che via via a specializzare le attività.

C'è una differenza tra stream di testo e stream binari.


## COMPITO

Crea una classe FileUtil che visualizza a schermo il contenuto di un file e il suo dump in hex

# ENUM

Quando viene creato un enumerativo il costruttore viene automaticamente chiamato.

Possono essere usati per costruire macchine a stati sfruttando il costruttore degli enumerativi.

Esempio di macchina a stati:
E' un sistema che si comporta in modo dinamico e il suo funzionamento varia a seconda delle condizioni. Quando le condizioni variano, il sistema decide come variare
il proprio funzionamento.

Il termostato è una macchina a stati. Gli stati sono misura temperatura, termostato spento e acceso. Parte dallo stato misura temperatura. Le transizioni
sono le "frecce" tra misura temperatura e risc. spento/acceso.

--------------------------
Il linguaggio Java ha una sintassi molto complessa. Laddove C ha solo 7 keywords, Java offre molte più potenzialità.


## CLASSI ANONIME

In una lista di interfacce, quello che cambia è l'implementazione degli oggetti.
Posso dire al compilatore di estendere l'interfaccia in line senza costruire una classe.
In Java se non ho bisogno di tante istanze di un oggetto (come degli operatori aritmetici) senza stato interno, tanto vale
usare una sintassi molto più diretta che mi crea un'istanza di un oggetto nel punto in cui io lo dichiaro.
A questo punto devo specificare i metodi che devo implementare.
L'apertura della parentesi graffa dopo il tipo si chiama "derivazione inline dell'interfaccia".

In sostanza, sposto la dichiarazione della classe nel file.
new "nomeInterfaccia"() fa in modo che il compilatore crei una classe "anonima" (con nome a caso) e la usa per creare un'istanza.

esempio:
```
OperatoreBinario opSomma = new OperatoreBinario(){
	
	public double eval(double a, double b){
		return a+b;
	}
}
```
Una classe anonima consiste di:
-operatore new
-interfaccia da implementare o classe da estendere
-coppia di parentesi per passare i parametri al costruttore (nulla se si tratta di un'interf)
-corpo

La classe anonima fa parte del contesto in cui è dichiarata: le dichiarazioni inline catturano il contesto. Quindi posso accedere alle variabili locali del contesto.

L'apertura delle parentesi graffe vuole dire: voglio ridefinire questa classe.

Se creo due classi anonime con stessa implementazione, ho derivato due volte, quindi il compilatore crea ex novo una nuova classe. getClass() restituirà quindi valori diversi.

Alla fine della derivazione inline c'è ; perchè è un'istruzione, e come tutte le istruzioni vanno chiuse con ;

### VARIABILE EFFECTIVE FINAL

Nella realtà le classi in line catturano il contesto, ma realmente catturano variabili considerate costanti. Anche se non sono dichiarate final.
Se viene modificata la variabile nel contesto, abbiamo un compile-time error.


**LE CLASSI ANONIME POSSONO:**  
- accedere ai metodi della classe che la contiene
- dichiarare nuovi metodi
- dichiarare classi locali (annidare classi anonime)
- definire blocchi di inizializzazione
- catturare le variabili effective final

**NON POSSONO:**  
- dichiarare blocchi static o intefacce locali
- dichiarare costruttori (usandola una volta e non creando un'istanza direttamente, perchè dovrei?)

Fuori dallo scope della classe, non posso chiamare metodi non dichiarati nella classe da cui avviene la derivazione, a meno che non usi reflection...

Codice boiler plate: parti copiate e incollate con variazioni minime.

Un'interfaccia con un unico metodo astratto è detta interfaccia funzionale: mappo un dominio in un codominio. E' sostanzialmente la funzione di analisi.
Possiamo usare una sintassi funzionale:

(a,b) -> {return a+b;};

Fa la stessa cosa che prima più righe di codice avrebbero fatto.

Ora il nostro codice (implementabile nella calcolatrice) non ha più boiler plate, classi esterne, ...

N.B. non deve andare in conflitto con variabili locali.
L'implementazione può avere dentro tutta la logica che voglio all'interno delle graffe, anche impegnando più righe.
Il codice può essere complesso, quindi va usata una sintassi diversa.
Se ho invece un unico statement, posso eliminare le graffe, il primo ; e anche return. Il sistema dà per scontato che il valore risultato che verrà restituito 
un valore.

La notazione funzionale permette ulteriormente di semplificarmi la vita. E' solo un modo più compatto per scrivere codice, passa sempre per la derivazione inline.

Per arrivare qui abbiamo dovuto introdurre classi anonime, derivazione inline e sintassi funzionale.


# LAMBDA EXPRESSION

(parametri) -> {codice/valore di ritorno}
BinaryOperator somma = (a, b) -> a+b;

Vanno usate con unico statement senza graffe oppure con graffe (allora va specificato return) per espressioni più complesse.

Una closure permette di passare comportamenti tra un metodo e un altro. E' un oggetto che implementa qualche comportamento.

## LAMBDA EXPRESSION: INTERFACCE PRONTE ALL'USO

Runnable: esegue un blocco di codice, non prende in input niente. E' una black box. Racchiude in se stesso un comportamento. Ma se lo chiamo, eseguirà qualcosa.
Fondamentale nel mondo dei thread.

Voglio visualizzare "hello world" o "*" riga dopo riga. Come può un'interfaccia funzionale aiutarci?
```
static void ripeti(int n, Runnable azione){
	for(int i=0;i<`n;i++){
		azione.run();
	}
}
```
Sto passando alla funzione un comportamento come se fosse una black box: voglio eseguire un certo numero di volte una certa azione.

Posso permettere a un terzo di eseguire un blocco di codice n volte. Vuol dire che ho un oggetto che implementa una certa interfaccia.
E' un interfaccia che non ha parametri in input e non ha valori di ritorno.

```
public Interface Task{
	void esegui();
}

public void ripeti(int n, Task task){
	for(int i=0;i`<`n;i++){
		task.esegui();
	}
}
```

Mi aspetto che dall'esterno qualcuno mi dica quello che devo fare ad ogni iterazione del ciclo.

Nel main:
```
public static void main(String[] args){
	ripeti(10, () -> System.out.println("Hello world"));
	ripeti(10, () -> System.out.print("*"));
	//ora vorrei sapere a quale iterazione del ciclo sono: aggiungo un parametro k nell'interfaccia. Non è comunque detto che debba utilizzarlo nella lambda expression.
	//esegui() ha ugualmente bisogno di un parametro. Metto i. Se del parametro non me ne faccio niente, funziona nè più nè meno.
	ripeti(10, (x) -> System.out.printf("-> %d\n",x));	//visualizza a che indice sono

	//posso fare una lambda expression complicata quanto voglio
	ripeti(10, (x) -> {
		for(int k=0;k`<`x;k++){
		 	System.out.printf("*");
		}
		System.out.println();
	});	
}
```

Metodo di elevamento a potenza nella calcolatrice:
operazioni.put("^", (a, b) -> Math.pow(a,b));

Questa riga ha un problema sematico: quando io uso ^, chiamo Math.pow, tutto il resto è boiler plate. Posso scrivere questo:

```
operazioni.put("^", Math::pow);
```

La funzione che implementa quell'interfaccia esiste e si chiama Math.pow. Possiamo immaginare che Math.pow si espande ecc. ecc.
Dal punto di vista semantico è immediato vedere che stiamo mappando Math.pow

## METHOD REFERENCES

Ha una sintassi che è nomeClasse::nomeMetodo
nomeClasse.nomeMetodo invoca il metodo su una certa istanza

La sintassi viene risolta come se fosse una lambda expression. Mappa il metodo su un'istanza.

```
object::instanceMethod		//System.out::println 		x -> System.out.println(x)
Class::staticMethod		//Math::sqrt 			x -> Math.sqrt(x)
Class::instanceMethod		//String::compare 		(x,y) -> x.compare(y)
```

E' sostanzialmente una sintassi particolarmente compatta. Costruire tutto è un processo puramente meccanico, non ha senso scriverlo.
