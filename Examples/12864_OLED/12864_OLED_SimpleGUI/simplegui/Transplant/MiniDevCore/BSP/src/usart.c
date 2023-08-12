#include "usart.h"

#define USART_REV_INT_CBPF_DEFINE(PORT, INIT)   static USART_INT_FP PORT##_INIT_CALLBACK = (INIT)
#define USART_REV_INT_CFG_DEFINE(PORT)          void PORT##_ConfigReceiveInterrupt(USART_INT_FP pfCallBack, int iPreemptionPriority, int SubPriority)\
                                                {\
                                                    NVIC_InitTypeDef NVIC_InitStructure;\
                                                    NVIC_InitStructure.NVIC_IRQChannel = PORT##_IRQn;\
                                                    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=iPreemptionPriority;\
                                                    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;\
                                                    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;\
                                                    NVIC_Init(&NVIC_InitStructure);\
                                                    PORT##_INIT_CALLBACK = pfCallBack;\
                                                }

#define USART_REV_INT_SRV_DEFINE(PORT)          void PORT##_IRQHandler(void)\
                                                {\
                                                    uint8_t uiReg;\
                                                    if(USART_GetITStatus(PORT, USART_IT_IDLE) == SET)\
                                                    {\
                                                        if(NULL != PORT##_INIT_CALLBACK)\
                                                        {\
                                                            PORT##_INIT_CALLBACK(USART_INT_REASON_IDLE, 0x00);\
                                                        }\
                                                        uiReg = PORT->SR;\
                                                        uiReg = PORT->DR;\
                                                        (void)uiReg;\
                                                    }\
                                                    if(USART_GetITStatus(PORT, USART_IT_RXNE) == SET)\
                                                    {\
                                                        if(NULL != PORT##_INIT_CALLBACK)\
                                                        {\
                                                            PORT##_INIT_CALLBACK(USART_INT_REASON_REV, USART_ReceiveData(PORT));\
                                                        }\
                                                    }\
                                                }\


USART_REV_INT_CBPF_DEFINE(USART1, NULL);
USART_REV_INT_CFG_DEFINE(USART1);
USART_REV_INT_SRV_DEFINE(USART1);
USART_REV_INT_CBPF_DEFINE(USART2, NULL);
USART_REV_INT_CFG_DEFINE(USART2);
USART_REV_INT_SRV_DEFINE(USART2)

void USART_Initialize(USART_TypeDef* pstUSARTx, uint32_t Baudrate)
{
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = Baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(pstUSARTx, &USART_InitStructure);

    USART_Cmd(pstUSARTx, ENABLE);
    USART_ClearFlag(pstUSARTx, USART_FLAG_TXE);
}

void USART_SendByte(USART_TypeDef* pstUSARTx, uint16_t uiByte)
{
    USART_ClearFlag(pstUSARTx,USART_FLAG_TC);
    /* Send data. */
    USART_SendData(pstUSARTx, uiByte);
    /* Wait for send complete. */
    while(USART_GetFlagStatus(pstUSARTx, USART_FLAG_TC) != SET);
}

void USART_ActiveInterrupt(USART_TypeDef* pstUSARTx, bool bEnable)
{
    USART_Cmd(pstUSARTx, DISABLE);
    if(true == bEnable)
    {
        USART_ITConfig(pstUSARTx, USART_IT_RXNE, ENABLE);
        USART_ITConfig(pstUSARTx, USART_IT_IDLE, ENABLE);
    }
    else
    {
        USART_ITConfig(pstUSARTx, USART_IT_RXNE, DISABLE);
        USART_ITConfig(pstUSARTx, USART_IT_IDLE, DISABLE);
    }
    USART_Cmd(pstUSARTx, ENABLE);
}
