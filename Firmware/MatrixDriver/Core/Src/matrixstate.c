#include "matrixstate.h"

typedef enum{
    ESTADO_MOSTRAR = 1,
    ESTADO_DESPLAZAR,
} Estado_matriz;

int mEstado = ESTADO_MOSTRAR;

void Maquina_Estado(void){
    switch (mEstado)
    {
    case ESTADO_MOSTRAR:
        /* funcion multiplexado */
        break;
    
    default:
        break;
    }
}

void multiplexado(void){
    for (int fil = 0; fil < MAX_FIL; fil++)
    {
      COL1_GPIO_Port->ODR&=~(arr_sketch[fil]<<__builtin_ctz(COL1_Pin)); 
      HAL_GPIO_WritePin(FIL1_GPIO_Port,(FIL1_Pin<<fil),GPIO_PIN_SET);
      // Delay_us(5000);
      HAL_Delay(2);
      COL1_GPIO_Port->ODR|=(arr_sketch[fil]<<__builtin_ctz(COL1_Pin));
      HAL_GPIO_WritePin(FIL1_GPIO_Port,(FIL1_Pin<<fil),GPIO_PIN_RESET);
    }   
}