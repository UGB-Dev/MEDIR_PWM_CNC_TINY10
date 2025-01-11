/*

       MEDIR_PWM.c

*/

#include "MEDIR_PWM.h"

///////////////////////////////////////////////////////////////////
///////////////////  CONFIGURACION OVERCLOCK  /////////////////////

void Set_FCPU_8MHz(){
	CCP = 0xD8; // habilita la manipulacion del registro CLKPSR
	CLKPSR = 0x00; // prescalador a 1 (8MHz/1 = 8MHz)
	OSCCAL = 0xFF; // calibracion de oscilador RC al maximo (aprox 13 MHz)
	DDRB |= (1<<PORTB0); // PB0 como salida
}

///////////////////////////////////////////////////////////////////
/////////////  CAPTURA DE PULSO_H Y PERIODO(T)  ///////////////////

void Medir_Evento(){
	uint32_t Captura_A, Captura_B, Captura_C; // variables locales
	
	TCCR0A =TCNT0=0; // inicializacion del timer
	TIFR0 = (1<<ICF0); // limpia la bandera input capture
	
	/*    SE CAPTURA EL INICIO DEL PULSO     */
	TCCR0B = (1<<ICES0)|(1<<CS01); // se habilita el timer y se configura el flanco de subida
	while ((TIFR0 & (1<<ICF0)) == 0); // espera una interrupcion por flanco de subida
	Captura_A = ICR0 ; // se captura los ticks transcurridos
	TIFR0 = (1<<ICF0); // se limpia la bandera input capture
	
	/*    SE CAPTURA EL FINAL DEL PULSO ALTO E INICIO DEL PULSO BAJO   */
	TCCR0B &= ~(1<<ICES0); // se configura el flanco de bajada
	while ((TIFR0 & (1<<ICF0)) == 0); // espera una interrupcion por flanco de bajada
	Captura_B = ICR0; // se captura los ticks transcurridos
	TIFR0 = (1<<ICF0); // se limpia la bandera input capture
	
	/*    SE CAPTURA EL FINAL DEL PULSO BAJO    */
	TCCR0B |= (1<<ICES0); // se configura el flanco de subida
	while ((TIFR0 & (1<<ICF0)) == 0); // espera una interrupcion por flanco de bajada
	Captura_C = ICR0; // se captura los ticks transcurridos
	TIFR0 = (1<<ICF0); // se limpia la bandera input capture
	
	TCCR0B = 0; // se deshabilita el timer
	
	PULSO_H = Captura_B - Captura_A; // se calcula los ticks del pulso alto
	T = Captura_C - Captura_A; // se calcula los ticks del periodo
}

///////////////////////////////////////////////////////////////////
////////////////////  RUTINAS DE INTERRUPCION  ////////////////////

/*
ISR(TIM0_OVF_vect){ // interrupcion por sobreflujo en TCNT1
	OVERFLOW++;
}*/
