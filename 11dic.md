# 11 dicembre

Ultima lezione; cosette che servono per corsi successivi:
- aspetti architetturali (per mondo sviluppo software)
- test case 
- implementazione di un gioco / sistema completo client-server

# Architetture logiche e fisiche

Cosa stiamo facendo con Java, ecc?

In generale, qualsiasi sistema informatico si compone di un sistema di blocchi logici. I dati stanno in uno o più database (Database SQL, DB strutturati e DB non strutturati).  
I dati evito di metterli in un file che costruisco io, devo metterli in un sistema che mi garantisca stabilità.  
Secondo layer: business logic. L'applicazione non interagisce direttamente con i dati, ma passa per un layer di logica. Poi c'è la rappresentazione.  

### Applicazioni standalone

Modello diffuso negli anni 1980. Client, presentation, BL e dati sono fusi assieme.

### Architetture WEB/Mobile

La logica applicativa è totalmente centralizzata al server. Il client è solamente un "player" che sta sull'oggetto client. La parte server prepara le informazioni per lo specifico target. Va distribuito su diverse infrastrutture.  

AS/400 e S/390 sono macchine dedicate a processare dati con interfaccia a caratteri ancora usate nel mondo bancario. Usano architetture con BL+Dati su server e client+presentation su client. 

Nelle architetture web i layer non solo vivono su server diversi, ma c'è **continous integration**: c'è un team che continua a migliorare la gestione delle risorse. Serve fare test.

## Unit test

Dobbiamo essere sicuri che quello che facciamo non rompa la retrocompatibilità del sistema. Gli Unit test di Java sono un approccio semplice al mondo dei test.  

L'obiettivo dei test di unità è trovare un modo per standardizzare l'esecuzione di una serie di controlli, in modo che quello che vado a rilasciare sia completo, corretto e compatibile con quanto potevo fare prima.  

**Regressioni**: errori introdotti con una nuova versione

Pararsi dalle regressioni è fondamentale. Quando ho un sistema complesso, va creato un sistema di controllo che impedisca agli errori di uscire. Sono sistemi automatici. Ormai i LLM (Large Language Model) come Copilot sono molto bravi a fare unit test.  

### Creazione test di unità

Sono creati a partire da una classe appoggiata su un framework. Si usa la reflection: quando si trova una classe marchiata con *@test* JUnit, JUnit entra nel metodo. Controllo che le funzioni siano coerenti.  

Si possono marcare con @Test, @Before, @After,...  

I metodi assertTrue(condition), ... servono proprio a controllare condizioni. assertException(...) è importante perchè a volte **vogliamo** entrare in condizione di errore.  

La classe ha il suffisso Test. La classe parte con un metodo base test() e con un metodo fail() che fallisce in automatico. Poi posso eseguire tutti i test nella classe oppure qualcuno nello specifico.  
Posso avere un metodo generale test oppure molti metodi specifici, per esempio per andare a controllare che l'oggetto sia stato creato correttamente.  
In un creationTest() potrei inserire tanti assert, utile nel caso di costruttori complessi.   
Per testare altri metodi, è opportuno creare un nuovo metodo @Test: non andrò a testare l'addizione nel metodo di test del costruttore.  

Se si incontrano errori durante i test, bisogna debuggare.  
Se tutti i test passano, tutti i casi sono stati verificati. E' quindi buona norma testare quanti più casi possibili. Se devo confrontare oggetti, è bene che confronti diverse tipologie di oggetti, senza che il sistema vada in panico.  

Se dopo una modifica non tutti i test passano ma ho errori, vuol dire che si è rotta la retrocompatibilità e devo tornare risovlere questo problema.  

Il test deve essere **completo**. Nel mondo della sicurezza, è meglio un sistema non protetto che un sistema protetto male (non affidarsi). Dobbiamo essere confidenti che passare quel test dia sicurezza.  

Un test può passare anche se il metodo è **completamente sbagliato**: un metodo equals che restituisce sempre *true*? Follia!  
Vanno controllate anche le condizioni avverse. Il metodo equalsTest() deve passare gli *assertTrue()* per oggetti che ci aspettiamo che siano uguali e deve passare gli *assertFalse()* per oggetti che ci aspettiamo che siano diversi.

> equals() deriva da Object: accetterà Object. Devo essere sicuro che andrà rispetto all'implementazione del metodo. Devo verificare che passi anche questo caso. Va anche verificato il caso in cui i parametri sono null (e altri casi limite) e in caso generare un'exception.

### Best practices

1. **Un test, un comportamento**  
Conviene raggruppare per comportamenti generici
2. **Nomi descrittivi**
3. **AAA: Arrage, Act, Assert**  
Il test deve essere scomposto in tre blocchi logici per essere comprensibile. Non è buona norma cambiare i parametri durante il test.
4. **Assert delle eccezioni**  
Utile laddove *vogliamo* eccezioni e la dobbiamo gestire

### Test parametrici

Una annotation utile è *@ValueSource*: permette di iniettare nella suite di test dei valori.

```
@Test
void testNumeriPositivi(int numero){
    assertTrue(numero > 0);
}
```
Questo metodo può essere invocato da un altro metodo della suite di test. Oppure posso specificare i valori come sequenza:

```
@ParameterizedTest
@ValueSource(ints = {1,2,3,4,5})
void testNumeriPositivi(int numero){
    assertTrue(numero > 0);
}
```

A questo punto il sistema inietta direttamente i numeri 1,2,3,4,5 e li verifica. Utile al posto di usare un ciclo for. Fornisce 5 report al posto di uno.

### Test Suite

Posso organizzare un test in classi contenitori. Voglio eseguire tutti questi test sotto una suite di test. All'interno di questa suite dichiaro l'elenco di classi da andare a invocare.

```
@RunWith(Suite.class)
@SuiteClasses({
    CalcolatriceTest.class,
    ContoCorrenteTest.class   
}
```

### Limiti di JUnit Test

I test possono essere molto costosi, ma al mondo delle librerie servono parecchio. Un enorme aiuto può venire dall'AI generativa, ma i test generati vanno controllati per essere sicuri che funzionino.  

Cosa è difficile testare con i test di unità?  
Prendiamo per esempio il gioco della dama / Asteroid con architettura MVC.  

Il model può essere testato perchè ha una logica applicativa, anche il controller con la sua logica può essere testato.  
Testare la **View** nei test di unità è **molto difficile**. Eventuali tentativi di testare se la scacchiera appaia a video sono costosissimi. Nel mondo moderno questi tentativi sono fallimentari.  
Non si può testare che l'utente veda correttamente a schermo ciò che si aspetta. L'AI potrebbe aiutare (riconoscimento immagini: mi appare una scacchiera con 64 cellette B/N?), ma i tempi non sono ancora maturi...  

### Definire requisiti

Un test di unità sviluppato correttamente può definire requisiti: per esempio nell'ambito dei protocolli di comunicazione, devo verificare che il meccanismo sia conforme alle specifiche. Scrivo all'inizio o alla fine i test di unità? Dipende dal contesto.  
In ogni caso, i test di unità sono **necessari** per ogni sistema critico.

# Tris client / server

Gioco a turni: i giocatori giocano sulla stessa tastiera. Per trasformarlo in un gioco con client-server, devo scegliere che controller usare.  
La versione standalone del gioco usa un controller locale. Altrimenti, usa un controller client-server e si connette a un localhost.

## Differenze tra standalone e client-server

In modalità **standalone**, avremo una rappresentazione grafica (View) collegata ad un model di gioco che al suo interno contiene lo stato della partita. Collegando direttamente la View al model mi precludo la difficoltà di trasformarlo in gioco MP.  
Se il controller applicativo si preoccupa di intercettare il cambiamento di stato del model tramite eventi: quando l'utente fa click nella view in una certa posizione, il model processa l'azione ma non resituisce niente al controller: se cambia lo stato, il controller deve intercettare l'evento di cambio stato e passare alla View.  

App intercetta l'evento e forza il repaint della View. A questo punto la View chiede al controller lo stato del sistema (la View non lo deve sapere).  

Dall'altra parte l'altro giocatore ha una sua View, Controller e Model. Tra i due giocatori rimane in comune il model (lo stato del gioco).  
Se io sono in grado di tenere una copia del model di gioco sul server, sono apposto. Il controller deve invocare il model remoto, passandogli p.e. che un utente ha cliccato a x,y coordinate. Una volta che è stato cambiato lo stato, viene generato un evento che viene restituito al controller che ha invocato il metodo e anche a **tutti** i controller collegati.  
A questo punto i controller possono aggiornare la copia locale del model, e conseguentemente forzare il repaint della View.  

Cosa è cambiato? Il controller, invece di compiere azioni locali, compie azioni remote.  

E' importantissimo che gli oggetti comunichino tramite eventi. Diversamente si ha un monolita standalone ed è più semplice riscrivere da zero.
