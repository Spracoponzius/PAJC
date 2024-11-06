# 30 ottobre

# Graphics: immagini e trasparenza

Le immagini per g2(Graphics2D) sono matrici di interi che assumono valori per RGBA: A è il canale ALPHA che indica la trasparenza. Le immagini .png hanno già la trasparenza regolata.  
Posso passare un event handler per le immagini, perchè il caricamento delle immagini può impiegare del tempo (non sincrono). L'immagine nei nostri esempi verrà precaricata.  

## BufferedImage

Una BufferedImage è la copia in memoria di un'immagine caricata da disco.  
```
BufferedImage img = ImageIO.read(new File(".../piece.png");
```
Serve racchiudere questo in un blocco try-catch.  

L'immagine può essere anche costruita: posso creare un context sull'immagine stessa.

## Sistemi di riferimento per il Viewport

Voglio disegnare un cerchio al centro del pannello, ma anche le funzioni seno e coseno. Voglio applicare dei concetti in modo relativo: voglio disegnare una luna rispetto al sistema di riferimento del pianeta.  
Voglio lavorare in un SdR definito dall'utente. 

Qualsiasi trasformazione dello spazio eseguiamo, si traduce in una matrice trasformata affine, ovvero una 3x3. (Operazioni grafiche anche complesse sono così computazionalmente semplici).  
Ho primitive per regolare il SdR. 
```
g2.translate(w/2,h/2);
```
Così sposto l'origine del SdR in una certa posizione.  

Di base il sistema di riferimento della Viewport ha l'origine in alto a sinistra. Per spostare in basso a sinistra, eseguo translate (0,400) e scale(1,-1) (Ho una finestra 800x400).  
scale(1,-1) moltiplica ogni coordinata x per 1 e ogni coordinata y per -1. (Effettivamente, ribalto l'asse delle y).  
Così finisco in un sistema di riferimento cartesiano.  
Le trasformazioni dipendono a questo punto dal **SdR corrente in quel momento**.  

Ora voglio lavorare in uno spazio 1000x1000. Con scale(800/1000,400/1000) applico un fattore di scala (come se stessi allargando un'immagine), ma non sto rispettando l'aspect ratio: dovrei applicare lo stesso rapporto di scala. Non rispettando l'aspect ratio (rapporti di scala) l'immagine viene distorta.

> **Attenzione!** In Java la divisione tra interi in casi come questi va fatta con w.1000., aggiungendo un punto dopo il divisore. Diversamente, otterrei o 0 o 1 a seconda della risoluzione della finestra.

Applicando prima il fattore di scala e poi altre trasformazioni, ho delle coordinate fisse.  

## Preservare l'aspect ratio

Per far sì che le coordinate non si stretchino, si usano fattori di scala omogenei su x e y.  
Diversamente, avrei un'area utile (dove si preserva l'aspect ratio) minore rispetto alla finestra che effettivamente vedo.  
Come trovo il fattore di scala massimo mantenendo l'aspect ratio?

- Mettto w/200. da entrambe le parti: una parte del mondo esce dal pannello. Stessa cosa mettendo h.
- Prendo il minimo tra w e h: così risolvo il problema.  

```
int d = Math.min(w,h);
g2.scale(d/200., d/200.);
```

## Sequenza delle chiamate

Eseguendo:
```
int d = Math.min(w,h);
translate(w/2,h/2);
scale(d/100.,-d/100.);
drawRect(-25,-25, 20,20);
```
Aggancio il mio oggetto grafico al centro.
  
Al contrario:
```
int d = Math.min(w,h);
scale(d/100.,-d/100.);
translate(50,-50);
drawRect(-25,-25, 20,20);
```
Vincola il mio oggetto grafico nel quadrante in alto a sinistra.

## Ruotare gli oggetti

Può essere utile se vogliamo disegnare qualcosa e fare in modo che ruota assieme al SdR. Disegno le entità in un SdR fisso e poi ruoto tutto.  
Così mi risparmio calcoli trigonometrici.  
Attraverso un timer con x millisecondi e la funzione rotate applicata a g2, posso ruotare i miei componenti grafici.  

## Oggetti composti da tanti elementi grafici

Il tipo dati Path2D può essere utilizzato per disegnare polilinee, ma anche oggetti più complessi. Voglio disegnare una freccia.

```
public void fillArrow(Graphics2D g2, int x, int y, int size, double t){
    Path2D p = new Path2D.Double();
    p.moveTo(0,0);
    p.lineTo(-size,size/3);
    p.lineTo(-size,-size/3);
    p.closePath();
    
    AffineTransform at = new AffineTrasform();
    at.translate(x,y);
    at.rotate(t);    

    g2.fill(at.createTransformedShape(p));
}
```
Mi disinteresso del SdR fisso e costruisco il path. Il path può essere considerato come un pennarello che può tracciare una linea o saltare dei punti.  
Mi posiziono al centro, traccio due linee, chiudo il path e lo riempio.  
Per applicare trasformazioni a piacere, creo l'oggetto della trasformata affine e uso le sue primitive.

# Reflection

Una reflection consiste nella possibilità che venga interrogato l'oggetto a runtime. Negli altri linguaggi si chiama metaprogrammazione.  
Un serializzatore JSON è un'applicazione che preso un oggetto descrive le sue proprietà in un file di testo. La reflection serve proprio a esaminare a runtime i metodi e gli attributi dell'oggetto.  
Ogni oggetto è istanza della classe Object. Il metodo .getClass() restituisce un'istanza di una classe. Rappresenta l'insieme di metodi proprietà, derivazioni, metodi che rappresentano quella classe.  
Ho accesso runtime ad una classe: posso chiederle le sue proprietà.

```
Class(?) objectClass = o.getClass();
System.out.println("Classe di o: %s",objectClass.getName());

for(Method m : objectClass.getMethods()){
    System.out.println("=======================");
    System.out.printf("Method name: %s\n", m.getName());
    System.out.printf("------->: %s\n", m.getReturnType()); 
    for(Parameter p : m.getParameters()){
         System.out.printf("\t> %s [%s]\n", p.getName(),p.getType());
    }

    for(Annotation a : m.getAnnotations()){
        System.out.printf("-- annotation: %s\n", a);
    }
}
```

Tanti metodi di quelli visualizzati sono derivati da Object.  
Devo importare una libreria esterna, uso la Reflection per cercare particolari metodi con decodator (es. @Override) e di conseguenza comportarmi in un determinato modo.  

Tramite Reflection posso anche invocare metodi, saltando tutte le barriere private, protected, ...  
 Ogni metodo porta con sè anche tutti i parametri che necessita e il tipo di dato ritornato.  
Il codice scritto sopra sta facendo una introspection: non sta modificando niente.  

Esempio: progetto GeoGebra con parsing di una TextBox e compilazione Just In Time.

**Compito**:
- Gioca con la grafica e crea funzioni generiche
