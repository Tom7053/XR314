/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief
 *      Transmition d'un code ASCII à l'aide d'une liaison série
 *
 * \par Note: A partir de la Datasheet ATmega328P
 *
 * \par Documentation: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
 *
 * \author
 *      Github: https://github.com/Tom7053
 *      Email: tom.maine@etudiant.univ-reims.fr
 *
 * $Name: Liaison série $
 * $Revision: 1.0 $
 * $RCSfile: main.c,v $
 * $Date: 2023/11/21 10:43:51 $  \n
 ******************************************************************************/
//#include <avr/io.h>
#define FOSC 16000000  // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1
int flag = 0;


/*! \brief Fonction Main 
 */
int main(void) {
  USART_Init(MYUBRR);
  sei();
  //USART_Transmit(USART_Receive()+1);
  while (1) {
    _delay_ms(1);
    USART_putsln("IUT");

  }
}

/*! \brief Fonction Initialisation de la communication
 */
void USART_Init(unsigned int ubrr) {
  /*Set baud rate */
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  //Enable receiver and transmitter * /
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  /* Set frame format: 8data, 2stop bit */
  UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

/*! \brief Fonction Transmission de la communication
 */
void USART_Transmit(unsigned char data) {
  /* Wait for empty transmit buffer */
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  /* Put data into buffer, sends the data */
  UDR0 = data;
}

/*! \brief Fonction Reception de la communication
 */
/*unsigned char USART_Receive(void)
{
/* Wait for data to be received */
//while (!(UCSR0A & (1<<RXC0)))
//;
/* Get and return received data from buffer */
//return UDR0;
//}

/*! \brief https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
 */
ISR(USART_RX_vect) {
  flag = 1;
}

void USART_puts(unsigned char *str) {
  do {
    USART_Transmit(*str);
  } while (*++str != 0);
}

void USART_putsln(unsigned char *str) {
    USART_puts(str);
    USART_Transmit(10);
    USART_Transmit(13);
}