# SWING

L'esecuzione e' molto diversa da quella del programma a linea di comando.
Il programma e' trattato come un processo (visibile da taskmgr).
Un thread è un unità computazionale che sta facendo qualcosa.
L'esecuzione del codice va disaccoppiata dall'interfaccia stessa.
Il thread della grafica è separato dal thread che esegue il codice.
Mettendo questo in un unico thread è indice di cattiva programmazione.


### WINDOW BUILDER

WindowBuilder mostra l'anteprima dell'interfaccia. La sezione components dà una sezione di componenti che sono più o meno noti.

Elenco di componenti:
- TextField
- ComboBox
- Button
- CheckBox
- ...

E' bidirezionale: posso modificare l'interfaccia grafica e avere modifiche nel codice e cambiare righe di codice per avere modifiche nella GUI.  
CTRL+C,CTRL+V funziona anche sugli oggetti grafici.


### CONTENTPANEL
Posso trascinare il componente nell'anteprima.
Il ContentPanel è una sorta di pannello di tipo container (può contenere componenti). Ha una certa disposizione automatica dell'interfaccia grafica.
In Java il ContentPanel ha un layout manager che organizza da solo la disposizione.

Selezionando il ContentPanel o la Label cambiano le proprietà che posso modificare.

* Variable: nome dell'oggetto che viene usato nel codice sorgente.
* Contenuto

#### CONVENZIONI SUI PREFISSI

Le label sono definite con il prefisso lbl

I bottoni sono definiti con btn

...

## EVENTI
Clicco su un bottone e appare un testo. Come faccio?  
Prima metto JButton nel WindowBuilder, chiamo il bottone btnUno. Metto come etichetta "Uno"  
Il bottone è un oggetto complesso, gestisce da solo tutti gli eventi di base.  
Supponiamo che non so come gestire un evento.  
L'interazione con l'interfaccia deve essere **istantanea**: non posso usare cicli while per definire interazioni.  
Definisco quindi funzioni che non mi interessa dove sono implementate: devono intercettare l'evento per esempio di click di un bottone.

Facendo doppio click sul bottone nel WindowBuilder, il WindowBuilder aggiunge un blocco di codice.

`   
     btnUno.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e){
                System.out.println("Bottone uno premuto");
                //Si, posso mandare messaggi alla console!
        }
`

addActionListener: nella gestione degli eventi io posso dichiarare più azioni specifiche.

Quando il bottone viene premuto è come se lanciassi un messaggio. Gli elementi che lo intercettano sono elementi in ascolto. La sequenza di chiamata degli action listener non è garantita.

Non mi interessa **quanti** listener si agganciano. Ho un framework che in grado di mettere chiunque in ascolto.  
Voglio aggiornare l'etichetta in un nuovo action listener.  
`
    lblInfo.setText("Bottone premuto");    
`   
lblInfo viene trattata come una variabile effective final. Possiamo farlo perchè stiamo variando non **la locazione di lblInfo**, ma le sue **proprietà interne**.  

#### Eventi di debug

Possono essere abilitati o disabilitati all'occorrenza dall'event handler. Creo un oggetto debugListener della classe ActionListener che stampa in console i log. (quindi usa i metodi getSource()).

#### Title

Tutti gli eventi derivano dalla classe AVTEvent. Tutti questi eventi hanno la proprietà
source. Il source dell'evento è l'oggetto che ha generato l'evento stesso.  
Ora creo un secondo bottone, voglio intercettare anche l'evento del secondo bottone.  
Posso benissimo usare un ActionListener (come debugListener) già usato da btnUno.  

# Prova a casa

- Prova a imitare l'esempio del prof
- Fai in modo che anche il secondo bottone cambi la label
- Usa le lambda expressions quando usi le classi anonime.
