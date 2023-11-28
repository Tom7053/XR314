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

//#include <avr/io.h> // Ce code est actuellement commenté, peut-être une référence à une bibliothèque d'E/S AVR.

#define FOSC 16000000  // Fréquence du Clock
#define BAUD 9600      // Vitesse de transmission
#define MYUBRR FOSC / 16 / BAUD - 1 // Calcul de l'UBRR

#define FRAMING_ERROR (1 << FE0) // Erreur de trame
#define PARITY_ERROR (1 << UPE0) // Erreur de parité
#define DATA_OVERRUN (1 << DOR0) // Débordement de données

/*! \brief Fonction Main */
int main(void) {
  USART_Init(MYUBRR); // Initialise la communication série
  sei(); // Active les interruptions globales
  //USART_Transmit(USART_Receive()+1); // Exemple de transmission et réception (actuellement commenté)
  while (1) {
    _delay_ms(1); // Attente
  }
}

/*! \brief Fonction Initialisation de la communication */
void USART_Init(unsigned int ubrr) {
  /* Configuration de la vitesse de transmission */
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  // Activation du récepteur et du transmetteur
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  /* Configuration du format de trame: 8 bits de données, 2 bits d'arrêt */
  UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

/*! \brief Fonction Transmission de la communication */
void USART_Transmit(unsigned char data) {
  /* Attente du buffer de transmission vide */
  while (!(UCSR0A & (1 << UDRE0)));
  /* Mettre les données dans le buffer, envoie les données */
  UDR0 = data;
}

/*! \brief Fonction Reception de la communication */
unsigned char USART_Receive(void) {
  /* Attente de la réception des données */
  while (!(UCSR0A & (1 << RXC0)));
  /* Récupérer et retourner les données reçues du buffer */
  return UDR0;
}

/*! \brief Gestionnaire d'interruption pour la réception de données USART */
ISR(USART_RX_vect) {
  char status = UCSR0A;
  char byteReceive = UDR0;
  /* Vérification des erreurs possibles lors de la réception */
  if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN)) == 0) {
    USART_Transmit(byteReceive); // Transmettre les données reçues
  } else {
    USART_putsln("error"); // Transmettre "error" en cas d'erreur
  }
}

/*! \brief Fonction Transmission d'une chaîne de caractères */
void USART_puts(unsigned char *str) {
  do {
    USART_Transmit(*str); // Transmission caractère par caractère
  } while (*++str != 0);
}

/*! \brief Fonction Transmission d'une chaîne de caractères avec un retour à la ligne */
void USART_putsln(unsigned char *str) {
  USART_puts(str); // Transmission de la chaîne
  USART_Transmit(10); // Retour à la ligne
  USART_Transmit(13); // Saut de ligne
}
