#include <stdio.h>
#include "usart_stdio.h"
#include "usart.h"

#define STDIO_USARTx            USART1
#define STDIO_USARTx_CLK_FUN    RCC_APB2PeriphClockCmd
#define STDIO_USARTx_RCC        RCC_APB2Periph_USART1
#define STDIO_USARTx_PORT       GPIOA
#define STDIO_USARTx_TXPin      GPIO_Pin_9
#define STDIO_USARTx_RXPin      GPIO_Pin_10

#pragma import(__use_no_semihosting)

FILE __stdout;
FILE __stdin;

static void Stdio_ConfigGPIO(void)
{
    // Initialize data structure.
    GPIO_InitTypeDef GPIO_InitStructure;
    // Enable RCC.
    STDIO_USARTx_CLK_FUN(RCC_APB2Periph_GPIOA, ENABLE);
    // Config TX GPIO.
    GPIO_InitStructure.GPIO_Pin =   STDIO_USARTx_TXPin;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(STDIO_USARTx_PORT, &GPIO_InitStructure);
    // Config RX GPIO.
    GPIO_InitStructure.GPIO_Pin =   STDIO_USARTx_RXPin;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
    GPIO_Init(STDIO_USARTx_PORT, &GPIO_InitStructure);
}

void _sys_exit(int return_code)
{
    while(1)
    {
        /* Blank loop */
    }
}

int ferror(FILE *f)
{
    /* Your implementation of ferror */
    return EOF;
}

void _ttywrch(int c)
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(STDIO_USARTx, (uint8_t) c);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(STDIO_USARTx, USART_FLAG_TC) == RESET)
    {
        /* Blank loop */
    }
}

/*******************************************************************************
* Function Name  : int fputc(int ch, FILE *f)
* Description    : Retargets the C library printf function to the USART.printf
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fputc(int ch, FILE *f)
{
    while(USART_GetFlagStatus(STDIO_USARTx,USART_FLAG_TC)==RESET)
    {
        /* Blank loop */
    }
    USART_SendData(STDIO_USARTx,(uint8_t)ch);
    return ch;
}

/*******************************************************************************
* Function Name  : int fgetc(FILE *f)
* Description    : Retargets the C library printf function to the USART.fgetc
* Input          : None
* Output         : None
* Return         : Read char
*******************************************************************************/
int fgetc(FILE *f)
{
    uint16_t uiRtnValue;
    /* Loop until received a char */
    while(!(USART_GetFlagStatus(STDIO_USARTx, USART_FLAG_RXNE) == SET))
    {
        /* Blank loop */
    }
    uiRtnValue = USART_ReceiveData(STDIO_USARTx);

    /* Read a character from the USART and RETURN */
    return (uiRtnValue);
}

/*******************************************************************************
* Function Name  : USART_Stdio_Initialize
* Description    : Initialize usart port for stdio functions.
* Input          : Baudrate
* Output         : None
* Return         : None
*******************************************************************************/
void USART_Stdio_Initialize(uint32_t Baudrate)
{
    /* Configure USART1 */
    STDIO_USARTx_CLK_FUN(STDIO_USARTx_RCC, ENABLE);
    USART_Initialize(STDIO_USARTx, Baudrate);
    /* Configure USART1 GPIO */
    Stdio_ConfigGPIO();
}

