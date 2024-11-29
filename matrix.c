#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef MATRIX_ELEMENT_TYPE
#define MATRIX_ELEMENT_TYPE int
#endif

typedef MATRIX_ELEMENT_TYPE matrix_type;

typedef struct matrix{
    int rows;
    int cols;
    matrix_type *data;
} matrix;

matrix* mtx_create(int rows, int cols){
    matrix *m = (matrix*)malloc(sizeof(matrix));
    m->rows = rows;
    m->cols = cols;
    m->data = (matrix_type*)malloc(rows*cols*sizeof(matrix_type));
    return m;
}


//i tre puntini vogliono dire: numero variabile di parametri, va incluso stdarg.h
matrix* mtx_set_values(matrix* m, ...){
    va_list args;
    //va_start inizializza il puntatore all'inizio della lista 
    va_start(args,m);
    matrix_type *p = m->data;
    for(int i=0;i<m->rows*m->cols;i++){
        *p++ = va_arg(args, matrix_type);
    }   
    va_end(args);
    return m;
}

/**
 * va_end chiude correttamente l'accesso allo stack (altrimenti pointer exception)
 * 
 * va_arg ...
 */

void mtx_print(matrix *m){
    matrix_type *p = m->data;
    for(int i=0; i<m->rows ;i++){
        for(int j=0; j<m->cols; j++){
            printf("%d ",*p++);
        }
        printf("\n");
    }
}

matrix* mtx_init(matrix* m){
    matrix_type *p = m->data;
    for(int k=0; k<m->rows*m->cols; k++){
        *p++ = 1;
    }
    return m;
}



//C = A + B
matrix* mtx_add(matrix* a, matrix* b, matrix* c){
    if(a->rows != b->rows || a->rows != c->rows || a->cols != b->cols || a->cols != c->cols){
        printf("Errore: somma non possibile tra matrici di taglia diversa\n");
        return NULL;
    }
    matrix_type *pa = a->data;
    matrix_type *pb = b->data;
    matrix_type *pc = c->data;

    for(int k=0; k<c->rows*c->cols; k++){
        *pc++ = (*pa++) + (*pb++);
    }

    return c;
}

//C = A - B
matrix* mtx_diff(matrix* a, matrix* b, matrix* c){
    if(a->rows != b->rows || a->rows != c->rows || a->cols != b->cols || a->cols != c->cols){
        printf("Errore: differenza non possibile tra matrici di taglia diversa\n");
        return NULL;
    }
    matrix_type *pa = a->data;
    matrix_type *pb = b->data;
    matrix_type *pc = c->data;

    for(int k=0; k<c->rows*c->cols; k++){
        *pc++ = (*pa++) - (*pb++);
    }

    return c;
}

//Prodotto riga x colonna mtx_molt
matrix* mtx_molt(matrix* a, matrix* b, matrix* c){
    if(a->cols != b->rows || a->rows != c->rows || b->cols != c->cols){
        printf("Errore: prodotto tra matrici impossibile con queste dimensioni\n");
        return NULL;
    }
    matrix_type *pa = a->data;
    matrix_type *pb = b->data;
    matrix_type *pc = c->data;

    for(int i=0; i< a->rows; i++){
        for(int j=0; j< b->cols; j++){
            matrix_type somma = 0;
            pa = a->data + i*a->cols;
            pb = b->data + j;
            for(int k=0; k< a->cols; k++){
                somma = somma + *pa * *pb;
                pa++;
                pb += b->cols;
            }
            *pc++ = somma;
        }
    }

    return c;
}

int main(){
    matrix *m = mtx_create(3,3);
    matrix *n = mtx_create(3,3);
    matrix *o = mtx_create(3,3);
    mtx_init(m);
    mtx_set_values(n,1,2,3,4,5,6,7,8,9);
    //mtx_print(mtx_add(m,m,m));
    //mtx_print(mtx_diff(m,m,m));
    mtx_print(mtx_molt(m,n,o));
    return 0;
}
