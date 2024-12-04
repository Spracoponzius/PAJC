#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int somma(int a, int b){
	return a+b;
}

int sottrazione(int a, int b){
	return a-b;
}

int moltiplicazione(int a, int b){
	return a*b;
}

void main(){
	/*
	 * Quando eseguo il programma, si specifica al processore l'indirizzo della
	 * prossima istruzione (registro Instruction Pointer).
	 * Cosa rappresenta la parola chiave somma? UN PUNTATORE
	 * E' un puntatore ad una locazione di memoria dove risiedono le istruzioni.
	 * Fa saltare l'IP alla locazione delle istruzioni di somma.
	 * Allora, la differenza tra chiamare somma e sottrazione è il puntatore
	 * all'area di memoria. Se metto il valore di somma 
	 * in qualche puntatore, sono apposto.
	 * Uso un puntatore ad un tipo di dato void. Ma bisogna trattare con il compiler.
	 */
	int a = 3;
	int b = 5;
	void* operazione = somma;
	
	int res = somma(a,b);
}




