#ifndef __UART_H
#define __UART_H

#include "stm32f411xe.h"

/**
 * @brief UART 통신 상태를 나타내는 열거형
 */
typedef enum
{
    UART_OK = 0,      /*!< 정상 동작 완료 */
    UART_ERROR,       /*!< 일반적인 오류 발생 */
    UART_BUSY,        /*!< UART가 사용 중 */
    UART_TIMEOUT      /*!< 타임아웃 발생 */
} UART_Status;

/**
 * @brief UART 초기화를 위한 설정 구조체
 */
typedef struct
{
    uint32_t BaudRate;     /*!< 통신 속도 (bps) */
    uint8_t WordLength;    /*!< 데이터 비트 길이 (8 또는 9) */
    uint8_t StopBits;      /*!< 정지 비트 (1 또는 2) */
    uint8_t Parity;        /*!< 패리티 (0: None, 1: Even, 2: Odd) */
    uint8_t Mode;          /*!< 송수신 모드 (1: Tx, 2: Rx, 3: Tx+Rx) */
    uint8_t HwFlowCtl;     /*!< 하드웨어 흐름 제어 (0: 없음, 1: RTS, 2: CTS, 3: RTS+CTS) */
    uint8_t OverSampling;  /*!< 오버샘플링 (16 또는 8) */
} UART_Config;

/**
 * @brief UART 핸들 구조체
 */
typedef struct
{
    USART_TypeDef *Instance;  /*!< UART 레지스터 베이스 주소 */
    UART_Config    Config;    /*!< UART 설정 */
    uint8_t       *pTxBuffer; /*!< 송신 버퍼 포인터 */
    uint16_t       TxSize;    /*!< 송신 데이터 크기 */
    uint16_t       TxCount;   /*!< 송신된 데이터 수 */
    uint8_t       *pRxBuffer; /*!< 수신 버퍼 포인터 */
    uint16_t       RxSize;    /*!< 수신 데이터 크기 */
    uint16_t       RxCount;   /*!< 수신된 데이터 수 */
    uint32_t       Timeout;   /*!< 타임아웃 값 */
} UART_Handle;

/**
 * @brief  UART 주변장치를 초기화합니다.
 * @param  huart: UART 핸들 구조체 포인터
 * @return UART_Status: 초기화 결과
 * @note   이 함수는 UART 주변장치의 클럭을 활성화하고, 지정된 설정으로 초기화합니다.
 * @warning
 *         - 이 함수 호출 전에 해당 UART 핀들이 올바르게 설정되어 있어야 합니다.
 *         - PCLK2(APB2) 클럭은 기본값 16MHz로 가정합니다.
 */
UART_Status UART_Init(UART_Handle *huart);

/**
 * @brief  UART 주변장치를 비활성화합니다.
 * @param  huart: UART 핸들 구조체 포인터
 * @return UART_Status: 비활성화 결과
 * @note   이 함수는 UART 주변장치를 비활성화하고 클럭을 비활성화합니다.
 */
UART_Status UART_DeInit(UART_Handle *huart);

/**
 * @brief  UART를 통해 데이터를 송신합니다.
 * @param  huart: UART 핸들 구조체 포인터
 * @param  pData: 송신할 데이터 버퍼의 포인터
 * @param  Size: 송신할 데이터의 크기
 * @param  Timeout: 송신 타임아웃 값 (ms)
 * @return UART_Status: 송신 결과
 * @note   이 함수는 모든 데이터가 송신될 때까지 대기합니다.
 * @warning
 *         - pData 버퍼는 최소 Size 바이트의 크기를 가져야 합니다.
 *         - 송신 중 타임아웃이 발생하면 UART_TIMEOUT을 반환합니다.
 */
UART_Status UART_Transmit(UART_Handle *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
 * @brief  UART를 통해 데이터를 수신합니다.
 * @param  huart: UART 핸들 구조체 포인터
 * @param  pData: 수신한 데이터를 저장할 버퍼의 포인터
 * @param  Size: 수신할 데이터의 크기
 * @param  Timeout: 수신 타임아웃 값 (ms)
 * @return UART_Status: 수신 결과
 * @note   이 함수는 지정된 크기의 데이터를 수신할 때까지 대기합니다.
 * @warning
 *         - pData 버퍼는 최소 Size 바이트의 크기를 가져야 합니다.
 *         - 수신 중 타임아웃이 발생하면 UART_TIMEOUT을 반환합니다.
 */
UART_Status UART_Receive(UART_Handle *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
 * @brief  인터럽트 모드로 UART 송신을 시작합니다.
 * @param  huart: UART 핸들 구조체 포인터
 * @param  pData: 송신할 데이터 버퍼의 포인터
 * @param  Size: 송신할 데이터의 크기
 * @return UART_Status: 송신 시작 결과
 * @note   이 함수는 비동기적으로 동작하며, 송신 완료는 인터럽트에서 처리됩니다.
 */
UART_Status UART_Transmit_IT(UART_Handle *huart, uint8_t *pData, uint16_t Size);

/**
 * @brief  인터럽트 모드로 UART 수신을 시작합니다.
 * @param  huart: UART 핸들 구조체 포인터
 * @param  pData: 수신할 데이터 버퍼의 포인터
 * @param  Size: 수신할 데이터의 크기
 * @return UART_Status: 수신 시작 결과
 * @note   이 함수는 비동기적으로 동작하며, 수신 완료는 인터럽트에서 처리됩니다.
 */
UART_Status UART_Receive_IT(UART_Handle *huart, uint8_t *pData, uint16_t Size);

/**
 * @brief  UART 인터럽트 핸들러입니다.
 * @param  huart: UART 핸들 구조체 포인터
 * @return None
 * @note   이 함수는 UART 인터럽트 발생 시 호출되어야 합니다.
 */
void UART_IRQHandler(UART_Handle *huart);

#endif /* __UART_H */
