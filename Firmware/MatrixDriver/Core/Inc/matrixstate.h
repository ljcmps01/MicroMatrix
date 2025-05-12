#ifndef __MATRIXSTATE_H
#define __MATRIXSTATE_H

#include "includes.h"

#define MAX_FILAS 8

#define MAX_FIL 5 //Legacy, remover cuando se migre la funcion de multiplexado

extern int mEstado;
extern uint8_t arr_sketch[MAX_FIL]; //Se mantiene extern para simplificar el
                                    // desarrollo no deberia manternse accesible

typedef struct
{
    int rows;       //Cantidad de filas (x)
    int columns;    //Cantidad de columnas (y)

    uint16_t output[MAX_FILAS]; //Matriz de visualizacion del display
    uint16_t buffer[MAX_FILAS];

    //Puertos de filas y columnas
    GPIO_TypeDef * rows_port;   
    GPIO_TypeDef * columns_port;

    //Primeros pines de fila y columnas (deben ser consecutivos)
    uint16_t row_pin;           
    uint16_t col_pin;

    //Flag de inicializacion correcta
    uint8_t initialized;

    //valores para limitar la matriz de salida a los valores marcados antes
    int x_mask;
    int y_mask;

}Matrix_t;

void Matrix_Init(
    Matrix_t *new_matrix,
    uint8_t rows,\
    uint8_t columns,\
    GPIO_TypeDef *row_port,\
    GPIO_TypeDef *col_port,\
    uint16_t first_row_pin,\
    uint16_t first_col_pin
    );
void Matrix_Clear(Matrix_t *matrix);
void Maquina_Estado(void);
void multiplexado(Matrix_t *matrix);
void shift_matrix(uint8_t y);
void crop_input ();

#endif