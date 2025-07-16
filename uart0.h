void InitUART0(void);
void UART_Tx(char ch);  /* Write character to Serial Port    */  
char UART_Rx(void);    /* Read character from Serial Port   */
void UART_str(char *s);
void UART0_Int(unsigned int n);
void UART0_Float(float f);
