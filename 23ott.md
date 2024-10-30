# 23 ottobre

# Sincronizzazione thread

Dobbiamo usare "Syncronized" per fare in modo che i thread sono sincronizzati, serve per sincronizzarlo a livello sistema operativo.

## Guardia

Si può usare sync o nella funzione o in tutta la parte di codice che gestisce la logica. **Mutex** è una "guardia" che agisce su un oggetto specifico e sincronizza tutti i thread che provano ad accedere ad un oggetto.

## Blocco sincronizzato

Al chiamante viene consentito l'accesso al blocco sincronizzato e non va in **deadlock**.

## Deadlock

Avviene quando due thread utilizzano le stesse risorse.  
Esempio:  
Ho la funzione inc e la funzione f2 entrambe sincronizzate, se non si chiamano tra loro non andrò mai in deadlock, ma se una chiama l'altra andrò sicuramente in deadlock.  
Inc chiama il thread t1, f2 chiama il thread t2. Se f2 chiama inc, t2 viene bloccato e viene messo in coda ad aspettare inc. Se inc chiama f2, viene bloccata perchè t2 è in coda.  
A questo punto il programma è morto e bisogna killare l'applicazione da taskmgr.  

Di solito le situazioni di deadlock sono più sottili da identificare, e possono appartenere anche a oggetti diversi. La possibilità di deadlock con 1000 thread è decisamente elevata.

## Cenni a Locks

I reentrantLock permettono di decidere quando attivare e togliere il lock. Richiede particolare attenzione

## ReadWriteLock

Lock dei saldi del conto: quando un conto è in modifica impedisce la lettura.  
Bisogna costruire un sistema di lock.  
WriteLock ha la stessa regola dei blocchi syncronized: uno per volta può eseguirlo.  
Funzione getCount: se non la proteggo, tutti possono fare questa lettura, ma non è consistente con lo stato del sistema. Spesso è accettata perchè la lettura è molto veloce.  
ReadLock impedisce a tutti i thread di eseguire il blocco di codice protetto dal read.

# Semafori

I lock sono fai/non fai, ma a volte il problema è che una certa operazione non può essere eseguita da 200mila.  
Con molte richieste un server può andare in crisi.  
"Una funzione può essere chiamata solo da 1000 thread in contemporanea, altrimenti mettile in coda."  
Voglio dare un limite al numero di thread che possono eseguire in contemporanea una certa funzione.  
Semaforo di 5 thread: sono accettati 5 thread di seguito, gli altri sono messi in coda. Quando si libera un posto, viene fatto entrare il prossimo nella coda.  

Syncronized: passa un thread per volta  
Semafoto: decido il numero di thread che eseguono in contemporanea (voglio limitare il sovraccarico delle risorse)  

# AtomicXX

Un'operazione atomica è un'operazione che si chiude all'interno dello stesso ciclo di lock.  
La CPU impiega più cicli di clock ad eseguire un ***count++***. Se un altro thread prova a fare la stessa cosa, potrebbe inserirsi in mezzo. Vorrei che ***count++*** fosse considerato come unico ciclo di clock.  
Le operazioni atomiche sono thread safe: la CPU garantisce che l'operazione verrà eseguita dall'inizio alla fine senza interruzioni.  

## AtomicInteger

Posso modificare l'integer solo con i metodi di AtomicInteger.  
incrementAndGet è un metodo che fa quanto dice il nome, senza che nessuno possa inserirsi nell'operazione: viene bloccata all'interno della CPU.  

Voglio creare una serie di numeri pari (con i suoi incrementi ecc...). O mi creo un oggetto sincronizzato o sfrutto gli oggetti atomici.  

AtomicInteger sono oggetti potenti ma sono meno efficienti della classe Integer e del tipo primitivo.  

Quando non c'è multithread, lasciamo perdere gli AtomicInteger.  

I frattali richiedono di usare i thread, si possono usare sia blocchi sync che oggetti atomici.  

# Scheduled thread pools

Per fare animazioni servono timer. Il timer di Swing è un oggetto che per ogni tick di tempo esegue un certo Runnable (in lambda expression). Il timer vive nel ciclo degli eventi di java Swing. Abbiamo un solo thread, il thread grafico.  
 Ogni tot secondi aggiorno lo schermo? Non serve il multithread.  

Se devo pianificare un'attività (ogni tot secondi controllo un server remoto), una possibilità è quella di creare thread che si attivano ogni tot di tempo.  
ScheduledExecutorService sono proprio questo, ed esistono due famiglie:  
- **ScheduledThreadPool** : Voglio eseguire il task tra x tempo. Crea un thread schedulato che viene eseguito x secondi dopo il tempo che l'ho creato. Mentre è schedulato posso interrogarne lo stato (con un future), con getDelay(time) ottengo quanto tempo rimane prima della sua esecuzione.  

Posso pianficare che un evento avvenga ogni tot secondi (rate fisso). In questo caso il task viene eseguito continuamente ogni period. Quando vado a pianificare eventi con il timer, ogni x secondi esegue il Timer.  

**fixedRate**: il task viene eseguito ogni x secondi. Non mi interessa la durata del task. Se per qualche motivo questo task ha una durata superiore al frame di delay, verrà eseguito sfalsato nel tempo. Hanno senso quando il task ha una durata temporale molto minore del delay. Garanzia di eseguire il task ogni x secondi.  

**fixedDelay**: La durata di un task può non essere determinabile, ma va eseguito continuamente, ma allo stesso tempo non lo posso inserire in un while. Mantiene un ritardo fisso tra i diversi task. Garanzia che passa un tempo fisso tra un'esecuzione del task e l'altra.  

Un retry di comunicazione richiederà un fixedDelay. Per controllare i sensori di un'infrastruttura uso fixedRate.  
Dipende dallo scheduling della CPU, non è una garanzia al nanosecondo.

# MVC (Model View Controller)

E' un pattern di programmazione che governano le applicazioni di tutti i giorni. Separa in modo chiaro l'interfaccia dalla modellazione dalla logica applicativa.  

Per fare una scacchiera abbiamo creato un paio di file, GraphicsApp e PnlGraphics. Voglio implementare il gioco della dama.  

Abbiamo 12 pedine con una logica di movimento: una pedina può muoversi solo sulle caselle di un certo colore, mangiare altre pedine in un certo modo...  

Dove metto le informazioni di queste pedine? Cos'è una pedina nel mondo Java?  
- Posso metterla in un ArrayList (con 12 pedine bianche e 12 pedine nere)
- E' un oggetto con una coppia di coordinate, un colore e uno stato (classe Pedina)

Colore può essere anche un booleano, se rappresentarle in B/N o in giallo/viola è una scelta grafica. La logica di gioco dipende dall'appartenenza ad un gruppo.  
Lo stato comprende anche i damoni. La pedina logica è solo parte delle regole di gioco.  Se implemento la Dama in singleplayer o in multiplayer la logica non deve cambiare. L'ArrayList di pedine non va messo in PnlGraphics.  

Come separo la logica applicativa dalla parte grafica?

## Macroblocchi

In Java posso separare le classi in file, differenzio ciò che è:
- **Model**: ciò che l'applicazione è ma non viene visualizzata (regole della Dama)
- **View**: la rappresentazione grafica, è serva del Controller (PnlGraphics). Non c'è una distinzione netta tra le classi View e le classi Controller in Java Swing. In altri linguaggi è separata. 
- **Controller**: sistema di coordinamento delle attività (come il model interagisce con l'utente o i diversi attori)

Le applicazioni vanno immaginate scomposte in questi 3 blocchi. Diamo delle regole per far interagire questi oggetti.

**Model e View non si devono mai parlare.** Il controller fa da coordinatore del mondo delle View e dei Model.  
I Controller parlano con le View attraverso degli "outlet". btnOk è una outlet.  

I controller sono sempre in grado di parlare con i rispettivi Model. Governa anche lui il Model.  

La View non può conoscere il Controller, ma può mettere a disposizione degli Handler o eventi.  
Il controller definisce un target (EventHandler) su se stesso che verrà attivato quando si verifica un determinato evento. Al verificarsi di certe situazioni (mouse moved) il controller è in grado di registrare un target sull'azione.  

**Sottoscrizione di eventi esterni**: nella view non posso avere riferimenti espliciti ad un controller o un model.  
La View può generare un evento "qualcuno ha cliccato a 0,0" e lo mette a disposizione del mondo esterno.  
Il controller intercetta questo evento e chiede al model se la mossa è valida.  
Allora il controller comunica alla View di aggiornare la griglia.  

Il controller può anche registrare se stesso come target (flusso informazioni tra View e Controller): pannello grafico come target degli eventi del mouse.  

```
public PnlGraphics(){
    this.addMouseMotionListener(this);
}
```
Sto dicendo di collegare gli eventi del mouse (View) a un handler, che è il Controller stesso.  
Posso avere tanti handler, e in questo modo ho separato le mie funzioni.

## Eventi del Model

Il modello ha un problema: le View e il Controller non contengono i dati, se il Model cambia stato, come faccio a saperlo? Il Model non può parlare con nessuno.  
In un Model semplice (scacchiera), il Controller può gestire tutto.  
Se prendiamo qualcosa di più complesso, come Asteroids, la fisica che governa l'universo è parte del Model. Ogni tot millisecondi devo aggiornare l'universo. (evolve per step temporali molto piccoli). Allora posso prendere un controller che dice: ogni 10 ms aggiorna la View(Rendering a 100hz).  

Nel mondo dei frattali è ancora più complesso. Quando rinfresco la View?  
Gli eventi possono essere generati anche dai Model quando diventa molto complicato. Gli eventi generati dal Model sono modellizzati in modo analogo agli eventi di una View.  

In un sistema semplice si separano la logica Model dalla logica View.  
Non è detto che Model, Controller e View vivano nello stesso thread. Se non vivono nello stesso thread, bisogna far parlare i thread (sincronizzarli).

### Datasource

Possiamo far sì che un oggetto grafico faccia riferimento ad una sorgente di dati, che diventa il datasource di un oggetto (la lista di valori di una comboBox è un datasource). Oggi hanno un uso molto limitato in componenti molto specifici.  

## Nel mondo reale

Il modello MVC si applica a tutto ciò che facciamo (il bottone è un suo universo MVC, il pannello è un MVC, l'applicazione è un MVC). Creerò un controller applicativo che coordina tutto il sistema. Come faccio a far dialogare tra loro tutti questi oggetti?  

Tutto il sistema deve essere disegnato in modo che i componenti di pari grado parlino tra loro. 
