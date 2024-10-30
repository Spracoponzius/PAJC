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

# Sistemi di riferimento per il Viewport

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

## Ruotare gli oggetti

Può essere utile se vogliamo disegnare qualcosa e fare in modo che ruota assieme al SdR. Disegno le entità in un SdR fisso e poi ruoto tutto.  
Così mi risparmio calcoli trigonometrici.
