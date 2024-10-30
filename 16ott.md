# 16 ottobre

# Continuo su stream (fibonacci)

TODO  

# Thread (codice: vedi ThreadApp)

Quando si lancia un'applicazione, il SO mette a disposizione un'area di memoria in cui vengono messi i dati. Il SO deve poi eseguire il codice su quell'area di memoria.  
L'applicazione per il SO si chiama processo, i SO Unix-like offrono un livello di isolamento in modo che l'applicazione possa lavorare solo sui propri dati senza interagire con altre applicazioni.  
Quanto viene eseguito si chiama thread e ad ogni processo viene associato un thread.  

Alcuni processi possono avere più thread che lavorano in parallelo. (Voglio eseguire contemporaneamente più istanze(non proprio-**correggi**) della mia applicazione).  

I thread non hanno ognuno una parte di memoria dedicata, ma condividono tutti la stessa area di memoria.  
Ho bisogno di parallelizzare le attività. Come gestirle viene lasciato al SO.

- Come costruire un thread
- Problemi che il mondo dei thread porta

## Un esempio

Runnable è un'interfaccia funzionale (unico metodo) void con nessun parametro. E' l'interfaccia del mondo dei thread: semplicemente esegue qualcosa.

Posso creare un Runnable con un pezzo di codice: eseguirà quel codice finchè finisce il blocco (nel caso di un System.out si ferma lì). 

Creo un oggetto Thread passando come argomento il Runnable.

Quando viene fatto partire un thread si parallelizzano le attività. Nel processo lineare (o ideale?) le istruzioni vengono eseguite una dopo l'altra.  

Gli eventi che avvengono in un thread main e in un altro thread dovrebbero essere sincronizzate. Due thread possono usare lo stesso oggetto Runnable senza creare problemi (task è una variabile locale in thread main).

I computer moderni sono multicore con degli scheduler interni.  

I problemi di un thread si hanno quando si prova a lavorare sulla stessa variabile.  

Allocare un Thread è un'operazione complessa per un SO, non è come allocare un intero, può impiegare del tempo. Creiamo una classe che fa un task abbastanza semplice da riutilizzare.

## Un task più articolato

Creo una classe MyTask che deve implementare Runnable. Le variabili saranno:  

- Una stringa tName che dà il nome al thread;
- Un intero nstep che specifica il numero di ripetizioni del task;
- Un intero delay che specifica il ritardo.

Creo un costruttore che prende il ingresso tre variabili e le assegna a quelle sopra.  
Creo un metodo run che esegue il codice nstep volte. Introduco un ritardo nel blocco try-catch. Il SO mette a riposo per un numero di secondi specificato il thread.  
```
try{
    Thread.currentThread().sleep(delay);
}catch(InterruptedException e);
```
Lo sleep genera un'eccezione interna quando viene catturato che va catturata e quindi ignorata. (**sempre**)  

Cosa succede se creo due Thread che eseguono lo stesso task, magari con ritardo diverso?  

Fine viene eseguito dopo alcuni cicli di T1 e T2 perchè il thread main non è riuscito sufficiente in fretta ad arrivare all'istruzione di stampare a fine. T1 e T2 si alternano in caso di ritardo uguale.

**Chiamare due volte thread.start() genera un'eccezione, ma il primo thread chiamato viene ugualmente eseguito.** Viene interrotto il main: non verrà mai eseguita la linea con System.out "fine". Gli altri thread ancora in vita e già chiamati continuano a essere eseguiti come se niente fosse.  

## Executors

Di solito per creare i thread ci si appoggia agli Executors.  
Le CPU moderne hanno un grosso limite a 5 GHz. Sono rese più potenti aggiungendo molti più core. Le applicazioni moderne possono utilizzare anche centinaia di thread.

Gli Executor service lasciano il framework di Java a decidere come i task saranno eseguiti dai thread.  
A ognuno dei task può essere abbinata una linea temporale (Thread). Se si trova un altro task da eseguire, si aspetta che un task venga smaltito.  
Non dobbiamo preoccuparci di inseguire il SO, ma lasciamo all'Executor service questo compito.  

L'Executor service consente anche di conoscere il risultato dell'esecuzione di un task. Normalmente vorrebbe dire intercettare un evento ecc... ecc...  

### Come si trasforma il codice

```
    ExecutorService exec= Executors.newSingleThreadExecutor();
        /**
          *  newSingleThreadExecutor è particolare:
          *  
         */
    exec.submit(new MyTask("A",25,50);
    exec.submit(new MyTask("B",25,75);
    exec.submit(new MyTask("C",25,30);
```
Verranno eseguiti in sequenza i task A,B e C.  
**N.B.** Il programma non termina, viene bloccato in memoria ed è necessario uccidere il processo.  
 E' rimasto un thread di coordinamento dell'ExecutorService, che va chiuso.

```
    exec.shutdown();
```
Fai shutdown, attendono il momento della disattivazione (finchè tutti i task non sono terminati).  

Ora voglio avere due linee di esecuzione. Tre task per due linee di esecuzione. 

```
    ExecutorService exec = Executors.newFixedThreadPool(2);
```
Piuttosto che distruggere e ricreare thread, si riutilizzano thread già creati in quanto creare un thread consuma tempo.

Possiamo fare in modo che il sistema determini quanti thread per pool si crei, in modo bilanciato.  
**Non ho visto cosa ha scritto. E' da completare, sostituire newSingleThread...**
```
    ExecutorService exec= Executors.newSingleThreadExecutor();
```

### Timeout

Fai shutdown, dopodichè aspetta un certo lasso di tempo. Una volta che quel tempo è passato, se non si è ancora spento, esegui il blocco finally, se è ancora vivo fai ***executor.shutdownNow()***, che equivale ad uccidere il processo.

## Valori di ritorno e Callable

Non mi basta più scrivere a video un valore.  
Voglio determinare i primi da 1 a 100M. Creo un metodo isPrime(long k). Faccio 4 blocchi e in ogni blocco controllo quanti primi cercare. Sommo il numero di primi trovati e ottengo i primi. Usando i thread parallellizzo la ricerca.  

Quando ho un valore di ritorno, è importante che mi restituisca un valore. Runnable non va più bene, serve l'interfaccia Callable. La classe MyTask ora implementerà Callable\<Integer> con il metodo call che restituirà Integer.

Al posto di gestire eventi, ecc. l'ExecutorService fornisce un future, che è una "promessa": restituirà il valore in un oggetto.

```
    //[] sono le diamond brackets
    Future[Integer] fA = exec.submit(new MyTask("A",25,50));
    
    try{
        System.out.printf("...",fA.get());
    }
    catch(InterruptedException e){
    
    }
    catch(ExecutionException ex){
    
    }finally{
        exec.shutdown();
    }
```

fA.get() restituisce i valori. Va messa in un try-catch.  
Quando i future vengono interrogati, se il task è completato restituiscono il risultato, altrimenti bloccano il task.  
Ora sysout fine comparirà alla fine. Poichè è bloccato nell'esecuzione dei thread sopra, non lo raggiungerà (il thread main è fermato).  
E' comune creare delle liste di Future o Task.  

### Concorrenza

Su un'area di memoria fare andare tante cose assieme si creano casini, non ci si accorge che quanto scritto potrebbe andare in conflitto. Un esempio che porta ad un risultato inaspettato:

```
    public class ConcurrentApp{
        public static void main(String[] args) throws Exception{
            ExecutorService exec = Executors.newFixedThreadPool(2);
            Counter counter = new Counter();

            for(int i=0;i<10000; i++){
                exec.submit(counter::inc);                
            }
            
            exec.shutdown();
            exec.awaitTermination(50,TimeUnit.SECONDS);

            System.out.println(counter.count);
        }
    }

    class Counter{
        int count = 0;

        void inc() {
            count++;
        }
    }
```

Chiamando 10000 volte il metodo inc, ci aspettiamo di avere 10000. E questo è vero finchè abbiamo un solo thread. Aumentando i thread, ottengo numeri diversi ogni volta che avvio il programma. Nessun errore, nessuna Exception. Il risultato è palesemente sbagliato.  

Cosa sta succedendo?  
Alla creazione di anche solo due thread, vivendo nello stesso processo e con la stessa area di memoria condivisa, con accesso ad un riferimento alla cella di memoria della variabile count.  
Quando ho il thread A e il thread B che cercano in parallelo di incrementare, il problema è la sequenza di operazione di inc. Entrambi devono leggere il valore e poi incrementare.  
Nel lasso di tempo in cui A legge il valore e incrementa, nessuno vieta a B di fare quello che vuole. Anche B legge il valore e incrementa. Così viene scritto due volte lo stesso valore. All'aumentare del numero dei thread la situazione peggiora.  

Tutte le volte che i thread modificano i dati, si trova concorrenza. Queste situazioni devono essere gestite (meglio se evitate).  
Se sono inevitabili, ci sono dei meccanismi, tra cui quello più immediato (e anche costoso, sfugge al mondo dei thread) è quello di sincronizzare i thread.

```
    synchronized void inc(){
        count++;
    }
```
Ora funziona giusto, ma abbiamo perso il vantaggio dei thread perchè un solo thread per volta esegue inc. Usare **syncronized** è comunque un buon modo per rilevare conflitti.
