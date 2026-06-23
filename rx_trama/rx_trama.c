

/*
 * la busqueda en el buffer circular es cada "x" ms
 * no puede ser directa porque perderia mucho tiempo hasta que se complete la trama completa
 *
 * octave:7>  dec2hex(sum(int8('@N512F1023R256')))
	ans = 321 -> el resultado esta en HEX, solo me quedo con el byte menor = 0x21
 *
 * 	@N512F1023R256C21
	@N512F1023R257C22
 */

#include "rx_trama.h"
#include "../serial/serial.h"
#include "../system.h"
#include "../main.h"
#include "../indicator/indicator.h"
#include <avr/eeprom.h>
#include "../usart/usart.h"

#define RX_CSTR_SIZE 32

struct _job_rx
{
	int8_t sm0;
}rx;


//char str[50];
//char buff[50];

void USB_commands(char USB_DATACODE, char *USB_payload_char)
{
	char* endPtr;

	MCUSR = 0;
//			if (USB_payload_idx > 0)	//Si existe payload, tendra payload util
//				{payload_f = atof(USB_payload_char);}


	//
//			indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
//			indicatorTimed_run();
	//
	switch (USB_DATACODE)
	{
		//setters
		case USB_DATACODE_SET_RECORRIDO_TOTAL:

			//recorrido_total = atof(USB_payload_char);
			recorrido_total = strtod(USB_payload_char, &endPtr);
			if (*endPtr == '\0')
			{
				USB_send_data_float(USB_DATACODE_SET_RECORRIDO_TOTAL, recorrido_total);
				//
				indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
				indicatorTimed_run();
				//
				//PinTo1(PORTWxLED1,PINxLED1);
			}

		break;

		case USB_DATACODE_SET_INTERVALO:
			//intervalo = atof(USB_payload_char);
			cli();
			intervalo = strtod(USB_payload_char, &endPtr);
                        
                        pulsos_por_intervalo = intervalo / ENCODER_KRESOL;
			sei();
			if (*endPtr == '\0')
			{
				USB_send_data_float(USB_DATACODE_SET_INTERVALO, intervalo);
				//
				indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
				indicatorTimed_run();
				//
				//PinTo1(PORTWxLED2,PINxLED2);
			}
		 break;

		case USB_DATACODE_SET_SELECTOR:
			selector = atoi(USB_payload_char);
			set_selector(selector);
			set_ledselector(selector);

			USB_send_data_integer(USB_DATACODE_SET_SELECTOR, selector);
			//
			indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
			indicatorTimed_run();
			//
		 break;

		case USB_DATACODE_SET_EXECUTION:
			execution = atoi(USB_payload_char);
			set_execution(execution);
			//
			indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
			indicatorTimed_run();
			//

		 break;

		case USB_DATACODE_SET_MOTOR:
			motor = atoi(USB_payload_char);
			set_motor(motor);
			//
			indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
			indicatorTimed_run();
			//

		 break;

		case USB_DATACODE_SET_LED_ENLACE:
			led_enlace = atoi(USB_payload_char);
			set_led_enlace(led_enlace);
			//
			indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
			indicatorTimed_run();
			//

		break;

		case USB_DATACODE_SET_ENCODER_PPR:
			encoder_PPR = atoi(USB_payload_char);
			eeprom_update_word(&EEMEM_encoder_PPR, encoder_PPR );

			USB_send_data_integer(USB_DATACODE_SET_ENCODER_PPR, eeprom_read_word(&EEMEM_encoder_PPR));
			//
			indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
			indicatorTimed_run();
			//
			//bug fixed 16/9/2025
			ENCODER_KRESOL = (longitudArcoPorResolucion/1000.0f)/(encoder_PPR * ENCODE_QUADRATURE);//una kte en metros/(PPR*4)
			set_execution(RESET);
		break;

		case USB_DATACODE_SET_LONGITUD_ARCO:
			//longitudArcoPorResolucion =  atof(USB_payload_char);
			longitudArcoPorResolucion =  strtod(USB_payload_char,&endPtr);
			if (*endPtr == '\0')
			{
				eeprom_update_float(&EEMEM_longitudArcoPorResolucion, longitudArcoPorResolucion);

				USB_send_data_float(USB_DATACODE_SET_LONGITUD_ARCO, eeprom_read_float(&EEMEM_longitudArcoPorResolucion));
				//
				indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
				indicatorTimed_run();
				//
				//bug fixed 16/9/2025
				ENCODER_KRESOL = (longitudArcoPorResolucion/1000.0f)/(encoder_PPR * ENCODE_QUADRATURE);//una kte en metros/(PPR*4)
				set_execution(RESET);
			}

		break;

		case USB_DATACODE_CONTROL_ACTIVATED:
			mainflag.control_recorrido = 1;

			USB_send_data_integer(USB_DATACODE_CONTROL_ACTIVATED, 0);

			//
			indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
			indicatorTimed_run();

			//

			//PinTo1(PORTWxLED3,PINxLED3);

		break;

		//getters, prepare
		case USB_DATACODE_GET_SELECTOR:
			mainflag.usb_send_selector =1;
			//
			indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
			indicatorTimed_run();
			//

		break;
		case USB_DATACODE_GET_EXECUTION:
			mainflag.usb_send_execution = 1;
			//
			indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
			indicatorTimed_run();
			//

		break;
		case USB_DATACODE_GET_MOTOR:
			mainflag.usb_send_motor = 1;
			//
			indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
			indicatorTimed_run();
			//

		break;
		case USB_DATACODE_GET_LED_ENLACE:
			mainflag.usb_send_led_enlace = 1;
			//
			indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
			indicatorTimed_run();
			//

		break;

		default:
			break;
	}

}

void rx_trama(void)
{
	static char USB_DATACODE = ' ';
	static char USB_payload_char[30];
	static int8_t USB_payload_idx = 0;

	//bug 23-01-2005 ahora Cstr NO ES STATIC, no concatena datos
	//static
	char Cstr[RX_CSTR_SIZE+1];//todos los bytes se inicializan a 0

	//char sbuff_out_temp[SCIRBUF_BUFF_SIZE];
	//char sbuff_out_temp[SCIRBUF_BUFF_SIZE +1]; //bug 23-01-2005

	//Cstr es siempre static porque recolecta y junta todos los caracteres disponibles del buffer serial

	uint8_t bytes_available;
	char c;
	int8_t newData = 0;
	int length;

	//float payload_f=0;



	//busqueda en buffer circular
	bytes_available = scirbuf_bytes_available();
	if (bytes_available > 0)
	{
		//OPCION 1
		/*
		scirbuf_read_nbytes((uint8_t*)sbuff_out_temp, bytes_available); //hago la copia desde el buffer circular hacia el de salida temporal
		sbuff_out_temp[bytes_available] = '\0';//convertir en c_str
		//strcat(Cstr,sbuff_out_temp);
		strcpy(Cstr,sbuff_out_temp);//bug 23-01-2005
		*/

		//OPCION 2
		//bug 23-01-2005 MAS DIRECTO, para no tener que usar  sbuff_out_temp como intermediario ya que ya no se guarda ni se concatena datos en Cstr
		scirbuf_read_nbytes((uint8_t*)Cstr, bytes_available); //hago la copia desde el buffer circular hacia el de salida temporal
		Cstr[bytes_available] = '\0';//convertir en c_str
//
//usart_print_string(" TrIniRx:");
//usart_print_string(Cstr);
//usart_print_string("Tr.final ");
		//
		length = strlen(Cstr);

		//usart_print_string(Cstr);

		//rx.sm0 = 0;//bug 23-01-2005
		for (int i=0; i< length; i++)
		{
			c =  Cstr[i];

			//

			if (rx.sm0 == 0)
			{
				if ( c == USB_DATACODE_TOKEN_BEGIN)
				{
					USB_payload_idx = 0;

					//+-
//					itoa(rx.sm0, buff, 10);
//					strcpy(str," ***rx.sm0=");
//					strcat(str,buff);
//					strcat(str," tkni=");
//					usart_print_string(str);
//					USART_Transmit(c);
//					usart_print_string(" +++");
					//-+

					rx.sm0++;
				}
			}
			else if (rx.sm0 == 1)
			{
				USB_DATACODE = c;

				//+-
//				itoa(rx.sm0, buff, 10);
//				strcpy(str," ***rx.sm0=");
//				strcat(str,buff);
//				strcat(str," USB_DATACODE=");
//				usart_print_string(str);
//				USART_Transmit(USB_DATACODE);
//				usart_print_string(" +++");
				//-+


				rx.sm0++;
			}
			else if (rx.sm0 == 2)//storage payload
			{

				if (c == USB_DATACODE_TOKEN_END)
				{
					USB_payload_char[USB_payload_idx] = '\0';

					//+-
//					itoa(rx.sm0, buff, 10);
//					strcpy(str," ***rx.sm0=");
//					strcat(str,buff);
//					strcat(str," TokenEnd USB_payload_char[");
//					itoa(USB_payload_idx, buff, 10);
//					strcat(str,buff);
//					strcat(str,"]=");
//					itoa(USB_payload_char[USB_payload_idx], buff, 10);
//					strcat(str,buff);
//					usart_print_string(str);
//					usart_print_string(" +++");
					//-+



					//
					//strcpy(Cstr,"");//bug 23-01-2005 Cstr ahora no acumula

					rx.sm0 = 0;
					//newData = 1;
					USB_commands(USB_DATACODE, USB_payload_char);
					//break;//bug 23-01-2005
				}
				else
				{
					USB_payload_char[USB_payload_idx] = c;

					//+-
//					itoa(rx.sm0, buff, 10);
//					strcpy(str," ***rx.sm0=");
//					strcat(str,buff);
//					strcat(str," USB_payload_char[");
//					itoa(USB_payload_idx, buff, 10);
//					strcat(str,buff);
//					strcat(str,"]=");
//					usart_print_string(str);
//					USART_Transmit(c);
//					usart_print_string(" +++");
					//-+

					USB_payload_idx++;

				}
			}
		}

	}
}

/*

#include "rx_trama.h"
#include "../serial/serial.h"
#include "../system.h"
#include "../main.h"
#include "../indicator/indicator.h"
#include <avr/eeprom.h>


#define RX_CSTR_SIZE 32
struct _job_rx
{
	int8_t sm0;
}rx;

void rx_trama(void)
{
	char sbuff_out_temp[SCIRBUF_BUFF_SIZE];

	//Cstr es siempre static porque recolecta y junta todos los caracteres disponibles del buffer serial

	static char Cstr[RX_CSTR_SIZE];//todos los bytes se inicializan a 0
	uint8_t bytes_available;
	char USB_DATACODE = ' ';
	char USB_payload_char[30];
	int8_t USB_payload_idx = 0;
	char c;
	int8_t newData = 0;
	int length;

	float payload_f=0;

	//busqueda en buffer circular
	bytes_available = scirbuf_bytes_available();
	if (bytes_available > 0)
	{
		scirbuf_read_nbytes((uint8_t*)sbuff_out_temp, bytes_available); //hago la copia desde el buffer circular hacia el de salida temporal
		//
		sbuff_out_temp[bytes_available] = '\0';//convertir en c_str
		strcat(Cstr,sbuff_out_temp);

		//
		length = strlen(Cstr);

//usart_print_string(Cstr);

		rx.sm0 = 0;
		for (int i=0; i< length; i++)
		{
			c =  Cstr[i];
			if (rx.sm0 == 0)
			{
				if ( c == USB_DATACODE_TOKEN_BEGIN)
				{
					USB_payload_idx = 0;
					rx.sm0++;
				}
			}
			else if (rx.sm0 == 1)
			{
				USB_DATACODE = c;
				rx.sm0++;
			}
			else if (rx.sm0 == 2)//storage payload
			{
				if (c == USB_DATACODE_TOKEN_END)
				{
					USB_payload_char[USB_payload_idx] = '\0';
					//
					strcpy(Cstr,"");

					rx.sm0 = 0;
					newData = 1;
					break;
				}
				else
				{
					USB_payload_char[USB_payload_idx++] = c;
				}
			}
		}
		if (newData == 1)
		{
//			if (USB_payload_idx > 0)	//Si existe payload, tendra payload util
//				{payload_f = atof(USB_payload_char);}

			newData = 0;

			//
//			indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
//			indicatorTimed_run();
			//
			switch (USB_DATACODE)
			{
				//setters
				case USB_DATACODE_SET_RECORRIDO_TOTAL:
					recorrido_total = atof(USB_payload_char);
					//USB_send_data_float(USB_DATACODE_SET_RECORRIDO_TOTAL, recorrido_total);
					//
					indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
					indicatorTimed_run();
					//

				break;

				case USB_DATACODE_SET_INTERVALO:
					intervalo = atof(USB_payload_char);
					//USB_send_data_float(USB_DATACODE_SET_INTERVALO, intervalo);
					//
					indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
					indicatorTimed_run();
					//

				 break;

				case USB_DATACODE_SET_SELECTOR:
					selector = atoi(USB_payload_char);
					set_selector(selector);
					set_ledselector(selector);

					USB_send_data_integer(USB_DATACODE_SET_SELECTOR, selector);
					//
					indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
					indicatorTimed_run();
					//


				 break;

				case USB_DATACODE_SET_EXECUTION:
					execution = atoi(USB_payload_char);
					set_execution(execution);
					//
					indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
					indicatorTimed_run();
					//

				 break;

				case USB_DATACODE_SET_MOTOR:
					motor = atoi(USB_payload_char);
					set_motor(motor);
					//
					indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
					indicatorTimed_run();
					//

				 break;

				case USB_DATACODE_SET_LED_ENLACE:
					led_enlace = atoi(USB_payload_char);
					set_led_enlace(led_enlace);
					//
					indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
					indicatorTimed_run();
					//

				break;

				case USB_DATACODE_SET_ENCODER_PPR:
					encoder_PPR = atoi(USB_payload_char);
					eeprom_update_word(&EEMEM_encoder_PPR, encoder_PPR );

					USB_send_data_integer(USB_DATACODE_SET_ENCODER_PPR, eeprom_read_word(&EEMEM_encoder_PPR));
					//
					indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
					indicatorTimed_run();
					//

				break;

				case USB_DATACODE_SET_LONGITUD_ARCO:
					longitudArcoPorResolucion =  atof(USB_payload_char);
					eeprom_update_float(&EEMEM_longitudArcoPorResolucion, longitudArcoPorResolucion);

					USB_send_data_float(USB_DATACODE_SET_LONGITUD_ARCO, eeprom_read_float(&EEMEM_longitudArcoPorResolucion));
					//
					indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
					indicatorTimed_run();
					//

				break;

				case USB_DATACODE_CONTROL_ACTIVATED:
					mainflag.control_recorrido = 1;
					//
					indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
					indicatorTimed_run();
					//


				break;



				//getters, prepare
				case USB_DATACODE_GET_SELECTOR:
					mainflag.usb_send_selector =1;
					//
					indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
					indicatorTimed_run();
					//

				break;
				case USB_DATACODE_GET_EXECUTION:
					mainflag.usb_send_execution = 1;
					//
					indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
					indicatorTimed_run();
					//

				break;
				case USB_DATACODE_GET_MOTOR:
					mainflag.usb_send_motor = 1;
					//
					indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
					indicatorTimed_run();
					//

				break;
				case USB_DATACODE_GET_LED_ENLACE:
					mainflag.usb_send_led_enlace = 1;
					//
					indicatorTimed_setKSysTickTime_ms(75/SYSTICK_MS);
					indicatorTimed_run();
					//

				break;

				default:
					break;
			}
		}

	}


}
 */
