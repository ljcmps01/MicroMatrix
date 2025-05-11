#ifndef __MATRIXSTATE_H
#define __MATRIXSTATE_H

#include "includes.h"

#define MAX_FILAS 8

#define MAX_FIL 5 //Legacy, remover cuando se migre la funcion de multiplexado

extern int mEstado;
extern uint8_t arr_sketch[MAX_FIL];

typedef struct
{
    int rows;
    int columns;
    int output[MAX_FILAS];
    GPIO_TypeDef * rows_port;
    GPIO_TypeDef * columns_port;
    uint16_t row_pin;
    uint16_t col_pin;
    uint8_t initialized;

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

#endif