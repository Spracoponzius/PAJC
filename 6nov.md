# 6 novembre

# Applicazioni Client - Server Java

## Architetture e topologie di rete

Protocollo TCP/IP: prende il messaggio e per mandarlo da un soggetto all'altro lo invia ad un indirizzo IP. Per arrivare al destinatario, deve essere instradato nella rete. La rete gestisce questo sottoforma di pacchetti, verso dei nodi.

## Livello applicativo

Sviluppiamo applicativi a livello presentazione, che nella pila protocollare TCP/IP è fuso con il livello applicativo. Le nostri applicazioni saranno su TCP con scambi di messaggi via socket. Per dialogare tra client e server useremo Telnet.  
Le porte vanno specificate per distinguere i diversi servizi erogati da un server.

## Comunicazione in rete in Java

Il nostro obiettivo è creare un'applicazione che esponga dei servizi (prima lato server), inizialmente sarà un echo.  
Per creare questa comunicazione devo stabilire un socket di rete: comunico al SO che se arriva un pacchetto al mio indirizzo IP e ad una determinata porta, quel pacchetto va consegnato all'applicazione (in realtà solo il payload, lo "sbustamento" avviene altrove).  
Il client deve mandare una richiesta di comunicazione (richiesta di apertura canale) e il server deve rispondere che è in grado. Vuol dire creare un sistema biunivoco di trasferimento dati. Vanno usati gli **stream**: una volta stabilita la comunicazione client-server, il server aprirà un canale in scrittura e aprirà un canale in lettura per ricevere le informazioni che il client manda. I canali sono gli stream.  

Java mette a disposizione oggetti di alto livello per costruire la comunicazione.

## ServerSocket
In Java esiste la classe ServerSocket, che rende la parte base molto semplice. I ServerSocket vanno creati in un blocco try.
```
try(
    ServerSocket server = new ServerSocket(1234);
    Socket client = server.accept();
    PrintWriter out = new PrintWriter(client.getOutputStream(),true);
    BufferedReader in = new BufferedReader(
        new InputStreamReader(client.getInputStream()));
){
    out.println("Hello!");
    out.flush();
}catch(IOException e){
...
...
```

Stream di scrittura: mando dei messaggi al client.

Serve fare out.flush perchè altrimenti viene tagliata la connessione prima di mandare il messaggio. Per farlo ad ogni riga, nel costruttore del PrintWriter va specificato true.  

Per avere più client connessi, servono i thread.

## Multiclient

Ci costruiamo un Protocol Processor: ci sposto la logica e libero la parte applicativa, che è sempre in ascolto di un client. Ogni client ha un thread che lo esegue.  
Va spostata la logica applicativa in una classe a parte che va eseguita in un thread separato.  
Ogni ProcessorProtocol gestisce la comunicazione con un client. Mi aspetto che possa essere costruito a partire da un socket, che è il client.  
Ogni volta che si connette un client, gli va dedicato un thread.

## Il Client applicativo

La ClientApp si occuperà di creare un SocketClient e di creare connessioni lettura-scrittura. Queste vanno scritte su thread separati.
```
String host = "localhost";
int port = 1234;

```

N.B. Il client è scritto in Java, ma può essere scritto in qualsiasi linguaggio. Se si codifica con stream binari si limita la compatibilità a seconda dei nostri processori.
