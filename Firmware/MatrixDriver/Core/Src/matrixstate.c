#include "matrixstate.h"

typedef enum{
    ESTADO_MOSTRAR = 1,
    ESTADO_DESPLAZAR,
} Estado_matriz;

int mEstado = ESTADO_MOSTRAR;

void Matrix_Init(
    Matrix_t *new_matrix,
    uint8_t rows,\
    uint8_t columns,\
    GPIO_TypeDef *row_port,\
    GPIO_TypeDef *col_port,\
    uint16_t first_row_pin,\
    uint16_t first_col_pin)
{
    new_matrix->initialized=0;

    if(rows<=MAX_FILAS && columns<=MAX_FILAS){
        //Setteamos la cantidad de filas y columnas
        new_matrix->rows=rows;
        new_matrix->columns=columns;
        
        //Setteamos los puertos y pines de salida
        new_matrix->rows_port=row_port;
        new_matrix->columns_port=col_port;
        new_matrix->row_pin=first_row_pin;
        new_matrix->col_pin=first_col_pin;

        //Guardamos los valores de limitacion de la matriz
        new_matrix->y_mask=(1<<rows)-1;
        new_matrix->x_mask=(1<<columns)-1; 
 
        Matrix_Clear(new_matrix);

        new_matrix->initialized=1;
    }


}

void Matrix_Clear(Matrix_t *matrix){
    for (int i = 0; i < MAX_FILAS; i++)
    {
        matrix->output[i]=-1;
    }
    multiplexado(matrix);
}

void Maquina_Estado(void){
    Delay_us(600);
    // switch (mEstado)
    // {
    // case ESTADO_MOSTRAR:
    //     /* funcion multiplexado */
    //     break;
    
    // default:
    //     break;
    // }
}

void multiplexado(Matrix_t *matrix){
    crop_input(matrix);
    for (int fil = 0; fil < matrix->rows; fil++)
    {
        matrix->columns_port->ODR&=~(matrix->output[fil]<<__builtin_ctz(matrix->col_pin));
        HAL_GPIO_WritePin(matrix->rows_port,(matrix->row_pin<<fil),GPIO_PIN_SET);
        Delay_us(5000);
        matrix->columns_port->ODR|=(matrix->output[fil]<<__builtin_ctz(matrix->col_pin));
        HAL_GPIO_WritePin(matrix->rows_port,(matrix->row_pin<<fil),GPIO_PIN_RESET);
    }   
}

void shift_matrix(Matrix_t *matrix,uint8_t y)
{
    int old_value=matrix->output[0];
    int new_value=matrix->output[0];
    for (int fil = 0; fil < matrix->rows; fil++)
    {
        if(y){
            if(fil==0){
                matrix->output[fil]=matrix->output[(matrix->rows)-1];
            }
            else
            {
                old_value=matrix->output[fil];
                matrix->output[fil]=new_value;
                new_value=old_value;
            }
            
        }
        else
        {
            matrix->output[fil]=(matrix->output[fil]<<1);
            if((1<<matrix->rows)&matrix->output[fil])
                matrix->output[fil]++;
        }
    }
}

void crop_input (Matrix_t *matrix){

    for (int row = 0; row < matrix->rows; row++)
    {
        matrix->output[row]&= matrix->x_mask;
    }
    
}

void load_output(Matrix_t *matrix, uint8_t data[]){
    for (int i = 0; i < matrix->rows; i++)
    {
        matrix->output[i]=data[i];
    }
    
}

void flip_x(Matrix_t *matrix){
    for (int fila = 0; fila < matrix->rows; fila++)
    {
        uint8_t valor_flip = 0;
        uint8_t valor_fila=matrix->output[fila];
        for (int col = 0; col < matrix->columns; col++) {
            valor_flip <<= 1;           // Desplaza el resultado a la izquierda
            valor_flip |= (valor_fila & 1);      // Copia el bit menos significativo de n
            valor_fila >>= 1;                // Desplaza n a la derecha
        }
        matrix->output[fila]=valor_flip;
    }
}

void flip_y(Matrix_t *matrix){
    uint8_t aux[matrix->rows];
    for (int fila = 0; fila < matrix->rows; fila++)
    {
        aux[fila]=matrix->output[fila];
    }
    for (int fila = 0; fila < matrix->rows; fila++)
    {
        matrix->output[fila]=aux[matrix->rows-fila-1];
    }
    
}

void negate_output(Matrix_t *matrix){
    for (int i = 0; i < matrix->rows; i++)
    {
        matrix->output[i]=~(matrix->output[i]);
    }
}