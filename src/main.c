#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "inttypes.h"

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
/* Infinite loop /
/ Use GDB to find out why we're here */
while (1);
}
#endif


 
void send_char(char c)
{
 while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
 USART_SendData(USART2, c);
}
 
int __io_putchar(int c)
{
 //if (c=='\n')
 //send_char('\r');
 send_char(c);
 return c;
}
 
void send_string(const char* s)
{
 while (*s)
 send_char(*s++);
}
 
int main(void)
{
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef uart;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio);
	
	gpio.GPIO_Pin = GPIO_Pin_3;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio);
	
	USART_StructInit(&uart);
	uart.USART_BaudRate = 115200;
	USART_Init(USART2, &uart);
	
	USART_Cmd(USART2, ENABLE);
	int cnt = 0;
	while (1) {
		
		if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE)) {
			char c = USART_ReceiveData(USART2);
			switch (c)
			{
					case 'a':
						send_string("Odebrano komunikat A!\n");
						break;
					case 'b':
						send_string("Odebrano komunikat B!\n");
						break;
					default:
						cnt++;
						printf("Nieznany komunikat numer: %i (int size=%i)\n", cnt, sizeof(cnt));
						break;
			}
		}
	
	}
}