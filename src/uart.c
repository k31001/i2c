#include "uart.h"
#include "stm32f411xe.h"
#include "sfr/usart.h"
#include <assert.h>

/**
 * @brief  UART 주변장치를 초기화합니다.
 * @param  huart: UART 핸들 포인터
 * @retval UART_Status
 */
UART_Status UART_Init(UART_Handle *huart)
{
    assert(huart != NULL);
    assert(huart->Instance != NULL);

    USART_TypeDef *USARTx = huart->Instance;
    uint32_t pclk = SYSTEM_CLOCK_DEFAULT;

    /* UART 비활성화 */
    USARTx->CR1.b.UE = 0;

    /* 보레이트 설정 */
    USARTx->BRR = (pclk + huart->Config.BaudRate/2) / huart->Config.BaudRate;

    /* 워드 길이 설정 */
    USARTx->CR1.b.M = (huart->Config.WordLength == 9) ? 1 : 0;

    /* 패리티 설정 */
    if (huart->Config.Parity != 0) {
        USARTx->CR1.b.PCE = 1;
        USARTx->CR1.b.PS = (huart->Config.Parity == 2) ? 1 : 0;
    }

    /* 정지 비트 설정 */
    USARTx->CR2.b.STOP = huart->Config.StopBits;

    /* 흐름 제어 설정 */
    USARTx->CR3.b.RTSE = (huart->Config.HwFlowCtl & 0x01) ? 1 : 0;
    USARTx->CR3.b.CTSE = (huart->Config.HwFlowCtl & 0x02) ? 1 : 0;

    /* 송수신 활성화 */
    USARTx->CR1.b.TE = (huart->Config.Mode & 0x01) ? 1 : 0;
    USARTx->CR1.b.RE = (huart->Config.Mode & 0x02) ? 1 : 0;

    /* UART 활성화 */
    USARTx->CR1.b.UE = 1;
    
    return UART_OK;
}

/**
 * @brief  UART 주변장치를 비활성화합니다.
 * @param  huart: UART 핸들 포인터
 * @retval UART_Status
 */
UART_Status UART_DeInit(UART_Handle *huart)
{
    assert(huart != NULL);
    assert(huart->Instance != NULL);

    USART_TypeDef *USARTx = huart->Instance;
    
    /* UART 비활성화 */
    USARTx->CR1.b.UE = 0;
    
    /* 모든 레지스터 초기화 */
    USARTx->CR1.w = 0x00;
    USARTx->CR2.w = 0x00;
    USARTx->CR3.w = 0x00;
    USARTx->BRR = 0x00;
    
    return UART_OK;
}

/**
 * @brief  UART를 통해 데이터를 송신합니다.
 * @param  huart: UART 핸들 포인터
 * @param  pData: 송신할 데이터 버퍼의 포인터
 * @param  Size: 송신할 데이터의 크기
 * @param  Timeout: 타임아웃 값 (단위: 틱)
 * @retval UART_Status
 */
UART_Status UART_Transmit(UART_Handle *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    assert(huart != NULL);
    assert(huart->Instance != NULL);
    assert(pData != NULL);

    USART_TypeDef *USARTx = huart->Instance;
    uint32_t tickstart = 0;

    for (uint16_t i = 0; i < Size; i++)
    {
        tickstart = 0;
        /* TXE 플래그가 설정될 때까지 대기 */
        while (!USARTx->SR.b.TXE)
        {
            if (tickstart++ > Timeout)
            {
                return UART_TIMEOUT;
            }
        }
        
        /* 데이터 전송 */
        USARTx->DR = (*pData++ & 0xFF);
    }
    
    /* 전송 완료 대기 */
    tickstart = 0;
    while (!USARTx->SR.b.TC)
    {
        if (tickstart++ > Timeout)
        {
            return UART_TIMEOUT;
        }
    }
    
    return UART_OK;
}

/**
 * @brief  UART를 통해 데이터를 수신합니다.
 * @param  huart: UART 핸들 포인터
 * @param  pData: 수신할 데이터 버퍼의 포인터
 * @param  Size: 수신할 데이터의 크기
 * @param  Timeout: 타임아웃 값 (단위: 틱)
 * @retval UART_Status
 */
UART_Status UART_Receive(UART_Handle *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    assert(huart != NULL);
    assert(huart->Instance != NULL);
    assert(pData != NULL);

    USART_TypeDef *USARTx = huart->Instance;
    uint32_t tickstart = 0;

    for (uint16_t i = 0; i < Size; i++)
    {
        tickstart = 0;
        /* RXNE 플래그가 설정될 때까지 대기 */
        while (!USARTx->SR.b.RXNE)
        {
            if (tickstart++ > Timeout)
            {
                return UART_TIMEOUT;
            }
        }
        
        /* 데이터 수신 */
        *pData++ = (uint8_t)(USARTx->DR & 0xFF);
    }
    
    return UART_OK;
}

/**
 * @brief  인터럽트 모드로 UART 송신을 시작합니다.
 * @param  huart: UART 핸들 포인터
 * @param  pData: 송신할 데이터 버퍼의 포인터
 * @param  Size: 송신할 데이터의 크기
 * @retval UART_Status
 */
UART_Status UART_Transmit_IT(UART_Handle *huart, uint8_t *pData, uint16_t Size)
{
    assert(huart != NULL);
    assert(huart->Instance != NULL);
    assert(pData != NULL);

    if (huart->TxCount != 0)
    {
        return UART_BUSY;
    }

    USART_TypeDef *USARTx = huart->Instance;

    huart->pTxBuffer = pData;
    huart->TxSize = Size;
    huart->TxCount = 0;

    /* 송신 인터럽트 활성화 */
    USARTx->CR1.b.TXEIE = 1;

    return UART_OK;
}

/**
 * @brief  인터럽트 모드로 UART 수신을 시작합니다.
 * @param  huart: UART 핸들 포인터
 * @param  pData: 수신할 데이터 버퍼의 포인터
 * @param  Size: 수신할 데이터의 크기
 * @retval UART_Status
 */
UART_Status UART_Receive_IT(UART_Handle *huart, uint8_t *pData, uint16_t Size)
{
    assert(huart != NULL);
    assert(huart->Instance != NULL);
    assert(pData != NULL);

    if (huart->RxCount != 0)
    {
        return UART_BUSY;
    }

    USART_TypeDef *USARTx = huart->Instance;

    huart->pRxBuffer = pData;
    huart->RxSize = Size;
    huart->RxCount = 0;

    /* 수신 인터럽트 활성화 */
    USARTx->CR1.b.RXNEIE = 1;

    return UART_OK;
}

/**
 * @brief  UART 인터럽트 핸들러입니다.
 * @param  huart: UART 핸들 포인터
 */
void UART_IRQHandler(UART_Handle *huart)
{
    assert(huart != NULL);
    assert(huart->Instance != NULL);

    USART_TypeDef *USARTx = huart->Instance;
    uint32_t isrflags = USARTx->SR.w;
    
    /* 송신 인터럽트 처리 */
    if (USARTx->SR.b.TXE && USARTx->CR1.b.TXEIE)
    {
        if (huart->TxCount < huart->TxSize)
        {
            USARTx->DR = (uint8_t)(*huart->pTxBuffer & 0xFF);
            huart->pTxBuffer++;
            huart->TxCount++;
        }
        if (huart->TxCount >= huart->TxSize)
        {
            USARTx->CR1.b.TXEIE = 0;
        }
    }
    
    /* 수신 인터럽트 처리 */
    if (USARTx->SR.b.RXNE && USARTx->CR1.b.RXNEIE)
    {
        if (huart->RxCount < huart->RxSize)
        {
            *huart->pRxBuffer = (uint8_t)(USARTx->DR & 0xFF);
            huart->pRxBuffer++;
            huart->RxCount++;
        }
        if (huart->RxCount >= huart->RxSize)
        {
            USARTx->CR1.b.RXNEIE = 0;
        }
    }
}