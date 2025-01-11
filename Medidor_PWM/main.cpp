/*
 * Medidor_PWM.cpp
 *
 * Created: 09/01/2025 08:12:31 p. m.
 * Author : UGB
 */ 

#define F_CPU 13485000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "MEDIR_PWM.c"

/*
	EL PWM POR DEFECTO DE LA PLACA " MKS DLC32 " ES DE 1 KHz SE PUEDE CONFIGURAR DE 
	1 KHz A 10 KHz ( 1 ms a 100 us) YA QUE EL REGISTRO ES DE 16 BITS Y PUEDE CONTAR 
	0 A 65535 (74.15 ns a 4.85 ms )
*/

int main(void){
	Set_FCPU_8MHz(); // ajusta la frecuencia a 13 MHz aprox
	
    uint8_t PORCENTAJE; // variable local
	
    while (1){
/*
			medir evento():
             _____________
            |             |             |
            |             |             |
            |             |             |
            |             |             |
            |             |_____________|
            ^             ^             ^
        Captura_A     Captura_B     Captura_C
		
		Tiempo_H = Captura_B - Captura_A
		T = Captura_C - Captura_A
		
		EL ANCHO DE PULSO SE DETERMINA MEDIANTE EL TIEMPO EN ALTO SOBRE EL PERIODO
		MULTIPLICADO POR 100% ( (PULSO_H *100)/ T )
*/
		Medir_Evento();
		
		PORCENTAJE = (PULSO_H *100)/ T; // se calcula el porcentaje del pulso
		
		if ((PORCENTAJE >= 90) && (PORCENTAJE<100)){ // si " 90 < PORCENTAJE < 100 ", PB0 en 1 logico
			PORTB |= (1<<PORTB0); 
		}
		else if((PORCENTAJE<10) && (PORCENTAJE>=0)){ // si " 0 < PORCENTAJE < 10 ", PB0 en 0 logico
			PORTB &= ~(1<<PORTB0);
		}
    }
}





