# Lezione 9 ottobre

**Prima parte**: Le lambda expression ci permettono di processare stream di dati (array list di dati, di valori), sfruttando unita' computazionali aggregate.  
**Seconda parte**: nella GUI Swing come si possono creare componenti custom, e come gestire gli eventi.  

# Computational stream  

Prendo una lista di stringhe fatta in modo vario. Vorrei restituire una lista di stringhe convertita in maiuscolo. Normalmente farei un for-each e convertire ogni stringa in maiuscolo.  
```
for(String s : myList){
    String us = s.toUpperCase();  
    upperCaseList.add(us);  
}
```  
Nel mondo si lavora su qualcosa di enumerabile, processo non solo un elemento ma un flusso di dati. In termini Java, si dice che stiamo elaborando uno stream. Come costruisco una catena di montaggio per un processing stream? 

### ArrayList

Posso dire alla lista di essere converita in un oggetto stream. Gli oggetti di tipo stream portano con  se' metodi particolari, come map(). Accetta una lambda expression come input.  
`  myList.stream().map(x -> x.toUpperCase());`  
Questa sintassi e' equivalente al ciclo for-each scritto sopra. La lambda expression applica questa trasformazione ad ogni elemento della lista. E' molto piu' leggibile.  
Possiamo applicare dei metodi su tutto lo stream. Esiste anche un metodo for-each che pero' "consuma" lo stream e lo trasforma nel prodotto finale.  
`myList.stream().map(x -> x.toUpperCase()).forEach(v -> System.out.println(v));`  
Senza clausola for-each avrei costruito un monad. Costruire questi computational stream non richiede CPU time.  

N.B.: la seguente scrittura e' equivalente:  
`myList.stream().map(String::toUpperCase).forEach(System.out::println);`  

Il filtro upperCase trasforma la stringa in qualcosa di nuovo: una stringa maiuscola. Ogni volta creo dei nuovi oggetti nel mio flusso.  
Gli stream hanno dei metodi.

#### Filter

Filtro gli elementi della lista. Impongo il filtro che la stringa inizia con la lettera c. Se mi capita una stringa con la lettera a viene ignorata. Al contrario, la funzione map agisce su tutto.  

## Efficienza degli stream

Che tipo di operazioni facciamo? Filter, map e sorted sono operazioni intermedie: processano uno stream solo se esiste solo una funzione terminale che "consuma lo stream".  
```  
    .map(s -> {  
            System.out.printf("---- MAP: %s\n", s);  
            return s.toUpperCase();  
    })  
    .forEach(System.out:println);  
```  
Senza l'ultima riga, non vedo proprio niente: creo l'oggetto ma non lo consumo perche' manca la funzione terminale.  
In generale: le operazioni **intermedie** restituiscono uno stream come risultato, le operazioni **terminali** (forEach) non resistuiscono nulla.  
Le operazioni sono dette **non interfering** se non alterano lo stream sottostante.  
Le operazioni sono **stateless** se dipendono solo dai valori specificati dai parametri e non devono conoscere lo stato.  

Un esempio di operazione stateless e' quando trasformiamo stringhe in maiuscolo, non ci interessa cosa abbiamo fatto prima. Anche filter e' stateless.  

Sorted non e' stateless: se voglio ordinare 50 nomi devo conoscerli tutti.  

Le operazioni stateful consumano l'intero stream e sono impegnative computazionalmente, vanno usate con cautela.  

Esistono operazioni comuni come average e sum. (Average e' stateful).  

Le funzioni di mappatura non necessariamente trasformano lo stream mantenendo il tipo originale.  

### Ordine degli operatori

Impatta sulle performance del sistema. E' piu' sensato prima filtrare e poi mappare piuttosto che il contrario.  
E' importante che le funzioni che diminuiscono il numero di oggetti dello stream siano eseguite prima, dato che gli stream sono processati in verticale. In questo modo ho a che fare con un set di dati piu' contenuto.  

Se non c'e' operazione terminale gli operatori intermedi non vengono neanche elaborati: **laziness**.  

La laziness e' un modello che mi attiva un riferimento per accedere alla risorsa in un secondo momento, ma senza fare nient'altro se non viene specificato, cosi' risparmio tempo. Negli stream si trova normalmente.  

```
//Markdown litiga con le parentesi angolari, sistema
Stream(Object) myStream =
    .stream()
    .filter(s -> {
        System.out.printf("--- FILTER: %s\n", s);
        return s.startsWith("c") ? true: false;
    })
    .sorted((s1, s2) -> {
        System.out.printf("--- SORT: %s, %s\n",s1,s2);
        return s1.compareTo(s2);
    .map(s -> {
        System.out.printf("--- MAP: %s\n",s);
        return s.toUpperCase();
    });
stream.forEach(System.out::println);
``` 

## Riutilizzo degli stream

Una volta che uno stream e' stato consumato, non si puo' riusare. Devo stare molto attento a verificare che non sia gia' stato usato, altrimenti fa crashare il programma.  
Questo limite viene superato passandoci un oggetto supplier:  
Voglio generare un IntStream e mettere una lambda expression che costruisce un nuovo oggetto. Questo tipo di lambda expression, che in input non vuole niente e in output restituisce un nuovo oggetto. Questo e' un' espressione di tipo **supplier**.  
Supplier\<T> e' un'interfaccia.
```
//Markdown litiga con le parentesi angolari, sistema
Supplier(IntStream) generatore_sequenza =
        () -> IntStream.iterate(2, k-> k+2+k*3);
```

Creo un supplier che genera il mio stream e lo chiamo tutte le volte che voglio.  
```
generatore_sequenza.get().anyMatch(k -> {
        System.out.println(k);
        return k > 100;
        });
```

**Compito: genera una serie di numeri primi**  

Gli stream sono molto utili nel mondo dei thread: passo attraverso gli stream per coordinare i thread.

#### Serie di fibonacci

In una serie di Fibonacci, ogni elemento dipende dai due precedenti.

# Componenti grafici custom

Possiamo creare dei nostri componenti. Esempio: un componente bottoniera. Due approcci:
- Composizione di componenti esistenti
- Design da zero del componente (con grafica vettoriale o bitmap)

## Componenti per composizione

Voglio riusare componenti che esistono e disporli in modo particolare.  
Per raggruppare componenti, possiamo utilizzare dei container (oggetto panel). Permettono di creare strutture annidate.  
Vogliamo creare un tastierino. Se inserisco bottoncini in un JPanel, ho creato un componente custom che posso riusare tutte le volte che voglio.  

Choose component (nella tab System) mi permette di visualizzare il mio componente custom nella lista.  
Voglio creare 10 oggetti piu' o meno uguali. Li trascino nell'interfaccia oppure faccio un ciclo for nel costruttore e creo un oggetto:
```
for(int i=0;i<10;i++){
    Jbutton btn = new JButton(String.format("%d",i));
    this.add(btn);
}
```
Ora voglio premere il bottone 0 per visualizzare 0 nella textbox. Quando interagisco con il componente panel, dovrei immaginare l'intero pannello che manda un evento che indica che e' stato premuto il bottone 1 p.e.  

E' abbastanza facile costruire il componente, ma quando gli eventi escono dallo stesso, devo generare degli eventi da quel componente.  

Ogni bottone genera un suo evento. Ora vorrei, alla pressione di un bottone, qualcosa del componente (un handler) manda all'esterno. Devo registrare un actionHandler a questo pannello. Di base un pannello non genera eventi. Ma possiamo creare comunque un metodo addActionListener che permetta dall'esterno di dire ad un altro componente di registrare quel listener.  

Vuol dire fare un metodo:
```
private ArrayList(ActionListener) listenerList = new ArrayList();

public void addActionListener(ActionListener l) {
    listenerList.add(l);    
}
```
Posso registrare qualche listener. Prendo il panelKeypad, aggiungo un actionListener, prende l'evento e passato dall'esterno e stampa il getActionCommand.
```
pnlKeypad.addActionListener(
    e -> System.out.println(e.getActionListener().getActionCommand())
    );
```
Qualcuno mi ha passato dei listener. Ora voglio che vengano eseguiti gli actionListener che mi sono stati passati. Vuol dire chiamare il metodo actionPerformed.
1. aggancio un actionListener al bottone (cio' che voglio eseguire).
2. metto un metodo fireActionPerformed

```
public void fireActionPerformed(ActionEvent e){
    System.out.println(e.getActionCommand()); 
}
```

```
for(int i=0;i<10;i++){
    Jbutton btn = new JButton(String.format("%d",i));
    this.add(btn);
    btn.addActionListener(e -> fireActionPerformed(e));
}
```

Cosi' stampo solo cio' che e' nel mio pnlKeypad. Se ora il fireActionPerformed fa per ogni actionListener nella lista prende il listener e aggiunge e:
```
public void fireActionPerformed(ActionEvent e){
    System.out.println(e.getActionCommand());
    for(ActionListener l : listenerList){
        l.actionPerformed(e);
    } 
}
```
Abbiamo creato un proxy per gli eventi dei 10 bottoni. Posso comunque aggiungere un altro actionListener al pnlKeypad. Il ciclo for-each li chiamera' tutti.  
Ho costruito un meccanismo a eventi in cui passo ad un oggetto una lista di elementi da eseguire e deleghiamo al nostro componente di eseguire gli oggetti in un momento terzo.

### Lista di cose da fare

1. Aggancia actionListener
2. fireActionPerformed(s)
3. addActionListener
4. (se vogliamo essere compliant) removeActionListener

La gestione interna dei componenti swing hanno una lista di eventi da gestire, nel framework gestisce una lista di qualsiasi tipo di listener. L'implementazione a basso livello usa gli array statici per performance.
