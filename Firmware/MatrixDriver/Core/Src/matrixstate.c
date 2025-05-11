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
  5,\
  31,\
  5,\
  5,\
  5,\
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
        new_matrix->rows=rows;
        new_matrix->columns=columns;
        new_matrix->rows_port=row_port;
        new_matrix->columns_port=col_port;
        new_matrix->row_pin=first_row_pin;
        new_matrix->col_pin=first_col_pin;

        Matrix_Clear(new_matrix);
    }

    new_matrix->initialized=1;

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
    for (int fil = 0; fil < MAX_FIL; fil++)
    {
        matrix->columns_port->ODR&=~(arr_sketch[fil]<<__builtin_ctz(matrix->col_pin));
    //   COL1_GPIO_Port->ODR&=~(arr_sketch[fil]<<__builtin_ctz(COL1_Pin)); 
        HAL_GPIO_WritePin(matrix->rows_port,(matrix->row_pin<<fil),GPIO_PIN_SET);
        Delay_us(5000);
        // HAL_Delay(2);
        matrix->columns_port->ODR|=(arr_sketch[fil]<<__builtin_ctz(matrix->col_pin));
        HAL_GPIO_WritePin(matrix->rows_port,(matrix->row_pin<<fil),GPIO_PIN_RESET);
    }   
}

// void shift_matrix(){
//     for (size_t fil = 0; fil < MAX_FIL; fil++)
//   {
//     arr_sketch[fil]=(arr_sketch[fil]<<0);
//     arr_sketch[fil]&=((1<<(MAX_FIL+1))-1);
//   }
// }