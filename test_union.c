#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union byte_decoder{
	unsigned int value;
	unsigned char byte[4];
	struct{
		unsigned char header;
		unsigned char type;
		unsigned char length;
		unsigned char checksum;
	}msg;
};


void main(){

	union byte_decoder decoder;
	decoder.value = 123456;
	//byte all'indice 1 di value
	printf("\n%x", decoder.byte[1]);
	//posso accedere direttamente tramite la proprietà length
	printf("\n%x", decoder.msg.length);

	decoder.byte[1] = 0;
	//Cosa accadrà mai? Cambierà il valore di value!
	//Quel secondo byte sarà settato a 0
	printf("\n%x", decoder.value); 		//avrò un valore diverso di value

	/*
	int value = 123456; //0x123456
	
	while(value>0){
		int cifra = value & 0xFF;
		printf("\n%x", cifra);
		value = value >> 8;
	}
	*/
}





