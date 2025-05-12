#include "matrixstate.h"

typedef enum{
    ESTADO_MOSTRAR = 1,
    ESTADO_DESPLAZAR,
} Estado_matriz;

int mEstado = ESTADO_MOSTRAR;

// uint8_t arr_sketch[MAX_FIL]={
//   0b01110,
//   0b10001,
//   0b10101,
//   0b01110,
//   0b10101,
// };

uint8_t arr_sketch[MAX_FIL]={
  0b01110,\
  0b00011,\
  0b00101,\
  0b11000,\
  0b00000,\
};

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
        matrix->output[i]=0;
    }
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
    for (int fil = 0; fil < MAX_FIL; fil++)
    {
        matrix->columns_port->ODR&=~(arr_sketch[fil]<<__builtin_ctz(matrix->col_pin));
        HAL_GPIO_WritePin(matrix->rows_port,(matrix->row_pin<<fil),GPIO_PIN_SET);
        Delay_us(5000);
        matrix->columns_port->ODR|=(arr_sketch[fil]<<__builtin_ctz(matrix->col_pin));
        HAL_GPIO_WritePin(matrix->rows_port,(matrix->row_pin<<fil),GPIO_PIN_RESET);
    }   
}

void shift_matrix(uint8_t y)
{
    int old_value=arr_sketch[0];
    int new_value=arr_sketch[0];
    for (size_t fil = 0; fil < MAX_FIL; fil++)
    {
        if(y){
            if(fil==0){
                arr_sketch[fil]=arr_sketch[MAX_FIL-1];
            }
            else
            {
                old_value=arr_sketch[fil];
                arr_sketch[fil]=new_value;
                new_value=old_value;
            }
            
        }
        else
        {
            arr_sketch[fil]=(arr_sketch[fil]<<1);
            if((1<<MAX_FIL)&arr_sketch[fil])
                arr_sketch[fil]++;
        }
    }
}

void crop_input (Matrix_t *matrix){

    for (int row = 0; row < matrix->rows; row++)
    {
        arr_sketch[row]&= matrix->x_mask;
    }
    
}