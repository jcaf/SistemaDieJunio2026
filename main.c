/* SISTEMA DIE 2026
 * RTM 3/09/2026 para entregar a Wilfredo
 * main.c
 *
 *  Created on: 20 dic. 2024
 *      Author: JCaf
 *      Atmega328P @20MHz
 *
 *Comprobar USART online
 *https://wormfood.net/avrbaudcalc.php?bitrate=19.2k&clock=20&databits=8
 *
 *serial port Terminal: realTerm
 *Terminal serial port at 230400 bauds:
 *https://sourceforge.net/projects/realterm/
 *
 *Encoder
 *http://makeatronics.blogspot.com/2013/02/efficiently-reading-quadrature-with.html

CH340 DATASHEET
https://www.mpja.com/download/35227cpdata.pdf

 Github new password> generar nuevo token
 https://stackoverflow.com/questions/46153433/eclipse-git-egit-not-accepting-remote-username-and-password

token dic2025 GitHub
https://github.com/jcaf/SistemaDieJunio2026.git



 * ----------------------------------------
 1)  http://www.engbedded.com/fusecalc/
 	 lock bits:
 	 http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p

 2) verificar que responda el atmega (ONLY A RESET)
 [jcaf@jcafpc ~]$ avrdude -c usbasp -B5 -v -p m328P

 3) programar fuse (sin preservar EEPROM)

 [jcaf@jcafpc ~]$ avrdude -c usbasp -B5 -p m328P -U lfuse:w:0xff:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

 4) GRABAR EL CODIGO FUENTE CON EL COMANDO ACOSTUMBRADO
 [root@JCAFPC Release]# avrdude -c usbasp -B5 -p m328P -U flash:w:SistemaDieJunio2026.X.production.hex

 NUEVO
 [root@JCAFPC Release]# avrdude -c usbasp -B0.3 -p m328P -V -U flash:w:SistemaDieJunio2026.X.production.hex (MAS RAPIDO!)
 Tambien puede ser sin -BX.. cuando ya esta bien configurado los fuses:
 [root@JCAFPC Release]# avrdude -c usbasp -p m328P -U flash:w:SistemaDieJunio2026.X.production.hex

 5) GRABAR LA EEPROM
 [jcaf@jcafpc Release]$ avrdude -c usbasp -B4 -p m328P -V -U eeprom:w:SistemaDieJunio2026.X.production.eep

 6) programar fuse (PRESERVANDO EEPROM)

		avrdude -c usbasp -B5 -p m328P -U lfuse:w:0xff:m -U hfuse:w:0xd1:m -U efuse:w:0xff:m

 7) Verificar los fuses
 [jcaf@jcafpc Release]$ avrdude -c usbasp -B4 -p m328P -U lfuse:r:-:i -v

 +++++++++++++++++++++++
 proteger flash (modo 3): lectura y escritura
 avrdude -c usbasp -B10 -p m328P -U lock:w:0xFC:m

BitRate UART
19.2k @ 20MHz

https://wormfood.net/avrbaudcalc.php?postbitrate=9600&postclock=1


 https://floating-point-gui.de/errors/comparison/
 */

#include "system.h"
#include "main.h"
#include "rx_trama/rx_trama.h"
#include "pinGetLevel/pinGetLevel.h"
#include "indicator/indicator.h"
#include "usart/usart.h"
#include "serial/serial.h"
#include <avr/eeprom.h>
#include <math.h>
#include <util/atomic.h>

// Variable global nueva para congelar el pulso exacto del último reporte
volatile int32_t enc_count_last = 0; 

volatile int32_t intervalo_actual_copy_from_ISR;
volatile int32_t enc_count_copy_from_ISR;
volatile int32_t intervalo_actual_last;
volatile int32_t pulsos_por_intervalo;
volatile uint8_t intervalo_rearmado = 1;
// Almacena el número de bloque entero (ej: 0, 1, 2, 3) que ya fue procesado
//volatile int32_t ultimo_bloque_procesado = 0; 

volatile uint8_t transmitiendo_intervalo = 0;
//volatile  int64_t enc_count = 0;
volatile  int32_t enc_count = 0;
//

float recorrido_total;// = 100;
volatile float recorrido_actual;
//volatile float recorrido_1vuelta;
volatile float intervalo;
int selector;
int execution;
int motor;
int led_enlace;
uint16_t encoder_PPR;
float longitudArcoPorResolucion;
//volatile int num_vuelta=0;

//uint16_t EEMEM EEMEM_encoder_PPR=0;
//float EEMEM EEMEM_longitudArcoPorResolucion=0;
//bug fixed: 16/09/2025
uint16_t EEMEM EEMEM_encoder_PPR=500;
float EEMEM EEMEM_longitudArcoPorResolucion=1000.0f;//inicialmente un valor para que al probar nos de un valor y no un "NAN"


volatile struct _isr_flag
{
	unsigned sysTickMs :1;
	//unsigned send_recorrido_actual :1;
	unsigned __a :7;
} isr_flag = { 0 };

volatile struct _mainflag mainflag;

volatile uint8_t control_recorrido;
volatile uint8_t usb_send_selector;
volatile uint8_t  usb_send_execution;
volatile uint8_t usb_send_motor;
volatile uint8_t usb_send_led_enlace;
volatile uint8_t usb_send_intervalo_completo;
volatile uint8_t send_recorrido_actual;

volatile static uint8_t enc_val = 0;
volatile uint8_t old_PORTRxENC_CHB;//track last change in quadrature
//
volatile float ENCODER_KRESOL; //es la kte. de div. Longiotud de Arco entre PPR*4

void set_execution(int execution)
{
	if (execution == INICIO)
	{
		motor = MOTOR_ON;
		set_motor(motor);
		set_led_motor(motor);
		
                control_recorrido = 1;

		usb_send_execution = 1;
	}
	else if (execution == PAUSA)
	{
		motor = MOTOR_OFF;
		set_motor(motor);//espera a la orden de inicio
		set_led_motor(motor);

		usb_send_execution = 1;
	}
	else if (execution == PARAR)
	{
		motor = MOTOR_OFF;
		set_motor(motor);//espera a la orden de inicio
		set_led_motor(motor);
		control_recorrido = 0;
		//usb_send_motor = 1;

		usb_send_execution = 1;
	}
	else if (execution == RESET)
	{
		//
		cli(); 

		//num_vuelta = 0;
		enc_count = 0;
                enc_count_copy_from_ISR = 0;
                intervalo_actual_copy_from_ISR = 0;    //added
                
		recorrido_actual = enc_count*ENCODER_KRESOL;

                enc_count_last = 0;
                
                transmitiendo_intervalo = 0; // Liberamos el escudo de la UART
                intervalo_actual_last = 0;
                intervalo_rearmado = 1;
                
                usb_send_intervalo_completo = 0;
                send_recorrido_actual = 0;
                 // Forzamos un valor absurdo para obligar al ISR a calcular la posición real 
                // en su primerísima interrupción, sin importar si va a positivos o negativos.
                //ultimo_bloque_procesado = -999; 
                
		USB_send_data_float(USB_DATACODE_SET_RECORRIDO_ACTUAL, recorrido_actual);

		usb_send_execution = 1;
		sei();
	}
}

void set_led_enlace(int led_enlace)
{
	if (led_enlace == 1)
	{
		PinTo1(PORTWxLED_ENLACE,PINxLED_ENLACE);
	}
	else
	{
		PinTo0(PORTWxLED_ENLACE,PINxLED_ENLACE);
	}
}

void set_led_motor(int led_motor)
{
	if (led_motor == 1)
	{
		PinTo1(PORTWxLED_MOTOR_ACTIVADO,PINxLED_MOTOR_ACTIVADO);
	}
	else
	{
		PinTo0(PORTWxLED_MOTOR_ACTIVADO,PINxLED_MOTOR_ACTIVADO);
	}
}
void set_motor(int motor)
{
	if (motor == 1)
	{
		PinTo1(PORTWxRELAY_MOTOR_GIRO,PINxRELAY_MOTOR_GIRO);
	}
	else
	{
		PinTo0(PORTWxRELAY_MOTOR_GIRO,PINxRELAY_MOTOR_GIRO);
	}

}
void set_ledselector(int selector)
{
	if (selector == SELECTOR_SP)
	{
		PinTo1(PORTWxLED1,PINxLED1);
		PinTo0(PORTWxLED2,PINxLED2);
		PinTo0(PORTWxLED3,PINxLED3);
		PinTo0(PORTWxLED4,PINxLED4);
	}
	else if (selector == SELECTOR_NC)
	{
		PinTo0(PORTWxLED1,PINxLED1);
		PinTo1(PORTWxLED2,PINxLED2);
		PinTo0(PORTWxLED3,PINxLED3);
		PinTo0(PORTWxLED4,PINxLED4);
	}
	else if (selector == SELECTOR_NL)
	{
		PinTo0(PORTWxLED1,PINxLED1);
		PinTo0(PORTWxLED2,PINxLED2);
		PinTo1(PORTWxLED3,PINxLED3);
		PinTo0(PORTWxLED4,PINxLED4);
	}
	else if (selector == SELECTOR_L)
	{
		PinTo0(PORTWxLED1,PINxLED1);
		PinTo0(PORTWxLED2,PINxLED2);
		PinTo0(PORTWxLED3,PINxLED3);
		PinTo1(PORTWxLED4,PINxLED4);
	}

}
/*
 * modificado el 01/06/2026 para la nueva tarjeta del 2026
 */
void set_selector(int selector)
{
	if (selector == SELECTOR_SP)
	{
		//RLY2
		PinTo0(PORTWxRELAY1,PINxRELAY1);//1
		PinTo1(PORTWxRELAY2,PINxRELAY2);//2
		PinTo0(PORTWxRELAY3,PINxRELAY3);//4
		PinTo0(PORTWxRELAY4,PINxRELAY4);//3
	}
	else if (selector == SELECTOR_NC)
	{
		//RLY1 - RLY2
		PinTo1(PORTWxRELAY1,PINxRELAY1);//1
		PinTo1(PORTWxRELAY2,PINxRELAY2);//2
		PinTo0(PORTWxRELAY3,PINxRELAY3);//4
		PinTo0(PORTWxRELAY4,PINxRELAY4);//3
	}
	else if (selector == SELECTOR_NL)
	{
		//RLY1 - RLY3
		PinTo1(PORTWxRELAY1,PINxRELAY1);//1
		PinTo1(PORTWxRELAY2,PINxRELAY2);//2
		PinTo0(PORTWxRELAY3,PINxRELAY3);//4
		PinTo1(PORTWxRELAY4,PINxRELAY4);//3
	}
	else if (selector == SELECTOR_L)
	{
		//RLY1 - RLY4
		PinTo1(PORTWxRELAY1,PINxRELAY1);//1
		PinTo0(PORTWxRELAY2,PINxRELAY2);//2
		PinTo1(PORTWxRELAY3,PINxRELAY3);//4
		PinTo0(PORTWxRELAY4,PINxRELAY4);//3
	}
}


void USB_send_data_float(char datacode, float payload0)
{
	char str[30];
	char buff[30];

	str[0] = USB_DATACODE_TOKEN_BEGIN;
	str[1] = datacode;
	str[2] = '\0';
	dtostrf(payload0, 0, 2, buff);
	strcat(str,buff);
	strcat(str,"\r");
	//
	usart_print_string(str);
}

void USB_send_data_integer(char datacode, int payload0)
{
	char str[30];
	char buff[30];

	str[0] = USB_DATACODE_TOKEN_BEGIN;
	str[1] = datacode;
	str[2] = '\0';
	itoa(payload0, buff, 10);
	strcat(str,buff);

	strcat(str,"\r");
	//strcat(str,"\n");

	//usart_println_string(str);
	usart_print_string(str);
}
//int8_t AreSame(float a, float b)
//{
//	if ( (fabs(a-b)) < 0.005)
//		return 1;
//  return 0;
//}

int main(void)
{
	int counter0=0;
	char buff[10];

	//Activar pullups en las entradas
	pinGetLevel_init(); //with Changed=flag activated at initialization

	ConfigOutputPin(CONFIGIOxRELAY1, PINxRELAY1);
	ConfigOutputPin(CONFIGIOxRELAY2, PINxRELAY2);
	ConfigOutputPin(CONFIGIOxRELAY3, PINxRELAY3);
	ConfigOutputPin(CONFIGIOxRELAY4, PINxRELAY4);

	ConfigOutputPin(CONFIGIOxLED_MOTOR_ACTIVADO, PINxLED_MOTOR_ACTIVADO);
	ConfigOutputPin(CONFIGIOxLED_ENLACE, PINxLED_ENLACE);
	ConfigOutputPin(CONFIGIOxLED1, PINxLED1);
	ConfigOutputPin(CONFIGIOxLED2, PINxLED2);
	ConfigOutputPin(CONFIGIOxLED3, PINxLED3);
	ConfigOutputPin(CONFIGIOxLED4, PINxLED4);

	ConfigOutputPin(CONFIGIOxRELAY_MOTOR_GIRO, PINxRELAY_MOTOR_GIRO);

	ConfigOutputPin(CONFIGIOxBUZZER, PINxBUZZER);
	indicator_setPortPin(&PORTWxBUZZER, PINxBUZZER);
//	indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
	indicatorTimed_setKSysTickTime_ms(1500/SYSTICK_MS);
	indicatorTimed_run();

	ConfigInputPin(CONFIGIOxENC_CHA, PINxENC_CHA);
	ConfigInputPin(CONFIGIOxENC_CHB, PINxENC_CHB);

	longitudArcoPorResolucion = eeprom_read_float( &EEMEM_longitudArcoPorResolucion);//longitud de arco se almacena en milimetros
	encoder_PPR = eeprom_read_word(&EEMEM_encoder_PPR);//
	ENCODER_KRESOL = (longitudArcoPorResolucion/1000.0f)/(encoder_PPR * ENCODE_QUADRATURE);//una kte en metros/(PPR*4)

        

	//With prescaler 64, gets 1 ms exact (OCR0=249) @16mhz
	//With prescaler 256, gets 1 ms no-exact (OCR0=77) @20mhz
	//Config to 1ms
	TCNT0 = 0x00;
	TCCR0A = (1 << WGM01);
	TCCR0B =  (1 << CS02) | (0 << CS01) | (0 << CS00); //CTC, PRES=256
	OCR0A = CTC_SET_OCR_BYTIME(1e-3, 256);// 77.125
	//
	TIMSK0 |= (1 << OCIE0A);
	//sei();

	//USART_Init ( 64);//38400 baudios
//	USART_Init ( (int)MYUBRR);
//	USART_Init ( 10); //@230400

	USART_Init ( 4); //@250000 @20mhz + DoubleSpeed U2X0 = 0
//	USART_Init (129); //@250000 @20mhz + DoubleSpeed U2X0 = 0

//	while (1)
//	{	usart_print_string("38400");
//		__delay_ms(1000);
//	}
	//Encoder setup Atmega328P, external Pull-ups 1K
	//channel A = PD2 INT0 / PCINT18
	//channel B = PD3 INT1 / PCINT19
	PCICR 	= 0x04;//PCIE2 PCINT[23:16] Any change on any enabled PCINT[23:16] pin will cause an interrupt.
	PCMSK2 	= 0x0C;//PCINT18 PCINT19
	//old_PORTRxENC_CHB = PORTRxENC_CHB;

	enc_val = ((PIND & 0b00001100) >> 2);
	sei();

	USB_send_data_integer(USB_DATACODE_RESET_BOARD, MCUSR);


	while (1)
	{
		if (isr_flag.sysTickMs)
		{
			isr_flag.sysTickMs = 0;
			mainflag.sysTickMs = 1;
		}

		if (mainflag.sysTickMs)
		{
			if (++counter0 == (20/SYSTICK_MS))    //20ms
			{
				counter0 = 0;

				pinGetLevel_job();
				//UP
				if (pinGetLevel_hasChanged(PGLEVEL_LYOUT_KEY_P1UP))
				{
					pinGetLevel_clearChange(PGLEVEL_LYOUT_KEY_P1UP);
					//
					if (control_recorrido == 0)
					{
						if (pinGetLevel_level(PGLEVEL_LYOUT_KEY_P1UP)== 0)	//active in low
						{
							if (++selector > SELECTOR_L)
							{
								selector = SELECTOR_SP;
							}
							set_selector(selector);
							set_ledselector(selector);

							//Se ha deshabilitado en el software el radio button "L"
							// por eso por seguridad deja sin efecto sÃ³lo en el envio, pero la sÃ¡lida de rele y leds sÃ­ se activan
							//USB_send_data_integer(USB_DATACODE_SET_SELECTOR, selector);
							if (selector != SELECTOR_L)
							{
								USB_send_data_integer(USB_DATACODE_SET_SELECTOR, selector);
							}


							//
							indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
							indicatorTimed_run();

						}
					}
				}
				//DOWN
				if (pinGetLevel_hasChanged(PGLEVEL_LYOUT_KEY_P2DOWN))
				{
					pinGetLevel_clearChange(PGLEVEL_LYOUT_KEY_P2DOWN);
					//
					if (control_recorrido == 0)
					{

						if (pinGetLevel_level(PGLEVEL_LYOUT_KEY_P2DOWN)== 0)	//active in low
						{
							if (--selector < SELECTOR_SP)
							{
								selector = SELECTOR_L;
							}
							set_selector(selector);
							set_ledselector(selector);
							//
							//USB_send_data_integer(USB_DATACODE_SET_SELECTOR, selector);
							if (selector != SELECTOR_L)
							{
								USB_send_data_integer(USB_DATACODE_SET_SELECTOR, selector);
							}


							//
							indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
							indicatorTimed_run();
						}
					}
				}

			}
		}


		//Send data to Host PC
		if (usb_send_selector)
		{
			USB_send_data_integer(USB_DATACODE_SET_SELECTOR, selector);
			usb_send_selector =0;
		}
		if (usb_send_execution)
		{
			USB_send_data_integer(USB_DATACODE_SET_EXECUTION, execution);
			usb_send_execution = 0;
		}
		if (usb_send_motor)
		{
			USB_send_data_integer(USB_DATACODE_SET_MOTOR, motor);
			usb_send_motor = 0;
		}
		if (usb_send_led_enlace)
		{
			USB_send_data_integer(USB_DATACODE_SET_LED_ENLACE, led_enlace);
			usb_send_led_enlace = 0;
		}

                int32_t enc_count_copia = 0;    
                int32_t intervalo_actual_copia = 0;
                int8_t es_intervalo_completo=0;
		
                
                /* Arquitectura
                 * ISR
                    ?
                    ? escribe variables
                    ?
                   variables compartidas
                    ?
                    ?
                   MAIN
                    ?
                    ??? IRQ OFF
                    ?     copiar datos
                    ?     copiar flags
                    ?     limpiar flags
                    ?
                    ??? IRQ ON
                    ?
                    ??? transmitir UART
                 */
                if (send_recorrido_actual)
                {
                    ATOMIC_BLOCK(ATOMIC_FORCEON)
                    {
                        enc_count_copia = enc_count_copy_from_ISR;
                        intervalo_actual_copia = intervalo_actual_copy_from_ISR;
                        es_intervalo_completo = usb_send_intervalo_completo; 

                        if (es_intervalo_completo) 
                        {
                            usb_send_intervalo_completo = 0;
                        }

                        // OPTIMIZACIÓN: Limpiamos la bandera aquí dentro para que sea una operación atómica
                        send_recorrido_actual = 0; 
                    }

                    if (es_intervalo_completo == 0)
                    {
                        // Telemetría normal continua en paz
                        recorrido_actual = (float) enc_count_copia * ENCODER_KRESOL;
                        USB_send_data_float(USB_DATACODE_SET_RECORRIDO_ACTUAL, recorrido_actual);
                    }
                    else 
                    {
                        // ¡Llegamos al intervalo! Activamos el escudo protector
                        transmitiendo_intervalo = 1;

                        // CORRECCIÓN MATEMÁTICA: Forzamos el valor flotante teórico exacto
                        // Ej: Si intervalo_actual es 9 e intervalo es 1.00f, dará 9.00f exacto.
                        
                        recorrido_actual = (float) intervalo_actual_copia * intervalo;

                        
                        // Transmitimos ambos comandos consecutivamente. 
                        // El ISR interrumpirá el bucle 'while' de la UART para contar pulsos (no pierdes pasos), 
                        // pero no ejecutará multiplicaciones float ni pisará variables. 
                        // Los bytes saldrán limpios uno detrás del otro por UDR0.

                        USB_send_data_float(USB_DATACODE_SET_RECORRIDO_ACTUAL, recorrido_actual);
                        
                        // El intervalo 0 representa el origen.
                        // Se actualiza la posición en PC, pero NO se informa como un punto de medición alcanzado.
                        //la PC queda protegida de interpretar el origen como una fila/punto nuevo.
                        if (intervalo_actual_copia > 0)// (recorrido_actual>0) "No reportar el intervalo cero."
                        {
                            USB_send_data_integer(USB_DATACODE_INTERVALO_COMPLETO, 0);
                            
                            indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
                            indicatorTimed_run();
                        }

                        // Liberamos el escudo
                        transmitiendo_intervalo = 0;

                        
                    }
                    // Línea eliminada de aquí afuera para evitar la carrera de firmware
                }

		//++
		rx_trama();
		indicatorTimed_job();
		mainflag.sysTickMs = 0;
	}
	return 0;
}

ISR(TIMER0_COMPA_vect)
{
	isr_flag.sysTickMs = 1;
}

//void encoder_reset(void)
//{
//	//
//	PCICR 	= 0x00;//disable PCIE2 PCINT[23:16]
//	//encoder.rotaryCount = 0x0000;
//	encoder = encoderReset;//clear struct
//	old_PORTRxENC_CHB = PORTRxENC_CHB;
////	isr_flag.send_posicion = 1;
//
//	PCIFR 	= 0x04;//PCINT18 PCINT19 clear flags
//	PCICR 	= 0x04;//PCIE2 PCINT[23:16] Any change on any enabled PCINT[23:16] pin will cause an interrupt.
//
//Este ISR es por cambio de nivel

/*
 
 * Acabo de reportar una frontera
        ?
intervalo_rearmado = 0
        ?
1 pulso de distancia ? sigue bloqueado
2 pulsos             ? sigue bloqueado
3 pulsos             ? sigue bloqueado
4 pulsos             ? sigue bloqueado
5 pulsos             ? REARMADO
 *  */

// Ajusta este valor según qué tanto vibre tu máquina físicamente



#define MARGEN_HISTERESIS 14 // 4 pulsos de zona muerta (ideal para filtrar ruido eléctrico)

ISR(PCINT2_vect)
{
    volatile static int8_t lookup_table[] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};

    // -----------------------------------------
    // 1. Decodificación cuadratura x4 (operación atómica nativa)
    // -----------------------------------------
    enc_val = enc_val << 2;
    enc_val = enc_val | ((PIND & 0b00001100) >> 2);
    enc_count = enc_count + lookup_table[enc_val & 0b00001111];
    
    // -----------------------------------------
    // 2. Telemetría continua(Se congela si el main está transmitiendo un intervalo)
    // -----------------------------------------
    if (transmitiendo_intervalo == 0)
    {
        enc_count_copy_from_ISR = enc_count;
        send_recorrido_actual = 1;
    }

    // -----------------------------------------
    // 3. Rearme de la histéresis
    // -----------------------------------------
    if (intervalo_rearmado == 0)
    {
        int32_t delta = enc_count - enc_count_last;

        if ((delta > MARGEN_HISTERESIS) || (delta < -MARGEN_HISTERESIS))
        {
            intervalo_rearmado = 1;
        }
    }
    
    // -----------------------------------------
    // 4. Detección de fronteras: Lógica de control de intervalos por bloques enteros
    // -----------------------------------------
    if (1) // (control_recorrido == 1)
    {
        int32_t intervalo_actual = enc_count / pulsos_por_intervalo;//zona donde estoy
        
        int32_t nuevo_intervalo_aceptado;//frontera que acabo de cruzar
        

        // Evaluamos si el encoder cruzó físicamente la frontera matemática de un bloque
        if (intervalo_actual != intervalo_actual_last)
        {
            // Determinamos la dirección del cruce actual y asignamos la marca teórica exacta
            int8_t direccion_actual = (intervalo_actual > intervalo_actual_last) ? 1 : -1;
            
            // Determinar la marca física cruzada
            if (direccion_actual == 1) 
            {
                //frontera que acabo de cruzar
                nuevo_intervalo_aceptado = intervalo_actual;      // Avance (Subida)
            }
            else 
            {
                //frontera que acabo de cruzar
                nuevo_intervalo_aceptado = intervalo_actual + 1;  // Reversa (Bajada) //2
                // es +1, porque si ha cruzado de 2->1, me tiene que decir que estoy en 2, no en 1
            }
           
            // ---------------------------------
            // FILTRO DE HISTÉRESIS
            // ---------------------------------
            if (intervalo_rearmado)
            {
                if (control_recorrido == 1)
                {
                    motor = MOTOR_OFF;

                    set_motor(motor);
                    set_led_motor(motor);
                }


                // Guardar la marca aceptada
                intervalo_actual_copy_from_ISR = nuevo_intervalo_aceptado;
                // Guardar el pulso exacto del disparo.   // A partir de aquí empieza la zona muerta.
                enc_count_last = enc_count;
                // Actualizar bloque matemático
                intervalo_actual_last = intervalo_actual;


                // Bloquear nuevos disparos hasta       alejarnos más de MARGEN_HISTERESIS.
                intervalo_rearmado = 0;


                // Solicitar reporte al main
                usb_send_intervalo_completo = 1;
                enc_count_copy_from_ISR = enc_count;
                send_recorrido_actual = 1;
            }
            else
            {
                /*
                 * Cruzamos nuevamente la frontera,
                 * pero todavía estamos dentro de la
                 * zona de histéresis.
                 *
                 * Es jitter: NO informar al PC.
                 */
                intervalo_actual_last = intervalo_actual;
            }
        }
    }
}
