#include <led.h>

#define LED_RCC             (RCC_APB2Periph_GPIOC)
#define LED_PORT            (GPIOC)
#define LED_PIN             (GPIO_Pin_13)

void LED_Initialize(void)
{
    // Initialize data structure.
    GPIO_InitTypeDef GPIO_InitStructure;
    // Loop for initialize each GPIO in array list.
    // Enable RCC.
    RCC_APB2PeriphClockCmd(LED_RCC, ENABLE);
    // Enable GPIO.
    GPIO_InitStructure.GPIO_Pin =   LED_PIN;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
}

void LED_On(void)
{
    // Set pin low.
    GPIO_ResetBits(LED_PORT, LED_PIN);
}

void LED_Off(void)
{
    // Set pin high.
    GPIO_SetBits(LED_PORT, LED_PIN);
}

void LED_Reverse(void)
{
    // Reverse pin state.
    if(1 == LED_GetState())
    {
        LED_On();
    }
    else
    {
        LED_Off();
    }
}

uint8_t LED_GetState(void)
{
    // Read pin state.
    return GPIO_ReadOutputDataBit(LED_PORT, LED_PIN);
}
