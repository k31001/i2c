#ifndef __SPI_H
#define __SPI_H

#include "stm32f411xe.h"

/**
 * @brief SPI 통신 상태를 나타내는 열거형
 */
typedef enum
{
    SPI_OK = 0, /*!< 정상 동작 완료 */
    SPI_ERROR,  /*!< 일반적인 오류 발생 */
    SPI_BUSY,   /*!< SPI 버스가 사용 중 */
    SPI_TIMEOUT /*!< 타임아웃 발생 */
} SPI_Status;

/**
 * @brief SPI 통신 모드를 나타내는 열거형
 */
typedef enum
{
    SPI_MODE_0 = 0, /*!< CPOL = 0, CPHA = 0 */
    SPI_MODE_1,     /*!< CPOL = 0, CPHA = 1 */
    SPI_MODE_2,     /*!< CPOL = 1, CPHA = 0 */
    SPI_MODE_3      /*!< CPOL = 1, CPHA = 1 */
} SPI_Mode;

/**
 * @brief SPI 초기화를 위한 설정 구조체
 */
typedef struct
{
    uint32_t ClockSpeed; /*!< SPI 통신 속도 (Hz). 범위: 최대 PCLK/2 */
    SPI_Mode Mode;       /*!< SPI 모드 (0-3) */
    uint8_t DataSize;    /*!< 데이터 크기. 0: 8비트, 1: 16비트 */
    uint8_t FirstBit;    /*!< 첫 비트 전송 순서. 0: MSB first, 1: LSB first */
    uint8_t NSS;         /*!< NSS 핀 관리 방식. 0: 하드웨어, 1: 소프트웨어 */
} SPI_Config;

/**
 * @brief  SPI 주변장치를 초기화합니다.
 * @param  SPIx: 초기화할 SPI 주변장치 (SPI1, SPI2 또는 SPI3)
 * @param  config: SPI 초기화 설정 구조체 포인터
 * @return None
 * @note   이 함수는 SPI 주변장치의 클럭을 활성화하고, 지정된 설정으로 초기화합니다.
 * @warning
 *         - 이 함수 호출 전에 해당 SPI 핀들이 올바르게 설정되어 있어야 합니다.
 *         - PCLK1(APB1) 또는 PCLK2(APB2) 클럭은 기본값 16MHz로 가정합니다.
 *           다른 클럭 주파수를 사용하는 경우 코드를 수정해야 합니다.
 */
void SPI_Init(SPI_TypeDef *SPIx, SPI_Config *config);

/**
 * @brief  SPI 주변장치를 비활성화합니다.
 * @param  SPIx: 비활성화할 SPI 주변장치 (SPI1, SPI2 또는 SPI3)
 * @return None
 * @note   이 함수는 SPI 주변장치를 비활성화하고 클럭을 비활성화합니다.
 */
void SPI_DeInit(SPI_TypeDef *SPIx);

/**
 * @brief  SPI를 통해 1바이트 데이터를 전송합니다.
 * @param  SPIx: 사용할 SPI 주변장치 (SPI1, SPI2 또는 SPI3)
 * @param  data: 전송할 1바이트 데이터
 * @return SPI_Status: 데이터 전송 결과
 * @note   이 함수는 데이터 전송이 완료될 때까지 대기합니다.
 * @remark 이 함수는 내부적으로 타임아웃을 체크합니다.
 */
SPI_Status SPI_WriteByte(SPI_TypeDef *SPIx, uint8_t data);

/**
 * @brief  SPI를 통해 1바이트 데이터를 수신합니다.
 * @param  SPIx: 사용할 SPI 주변장치 (SPI1, SPI2 또는 SPI3)
 * @param  data: 수신한 데이터를 저장할 포인터
 * @return SPI_Status: 데이터 수신 결과
 * @note   이 함수는 데이터 수신이 완료될 때까지 대기합니다.
 * @warning data 포인터는 반드시 유효한 메모리 주소를 가리켜야 합니다.
 */
SPI_Status SPI_ReadByte(SPI_TypeDef *SPIx, uint8_t *data);

/**
 * @brief  SPI를 통해 여러 바이트의 데이터를 전송합니다.
 * @param  SPIx: 사용할 SPI 주변장치 (SPI1, SPI2 또는 SPI3)
 * @param  data: 전송할 데이터 버퍼의 포인터
 * @param  len: 전송할 데이터의 길이 (바이트)
 * @return SPI_Status: 데이터 전송 결과
 * @note   이 함수는 모든 데이터가 전송될 때까지 대기합니다.
 * @warning
 *         - data 버퍼는 최소 len 바이트의 크기를 가져야 합니다.
 *         - 전송 도중 슬레이브가 응답하지 않으면 SPI_ERROR를 반환합니다.
 */
SPI_Status SPI_WriteData(SPI_TypeDef *SPIx, uint8_t *data, uint16_t len);

/**
 * @brief  SPI를 통해 여러 바이트의 데이터를 수신합니다.
 * @param  SPIx: 사용할 SPI 주변장치 (SPI1, SPI2 또는 SPI3)
 * @param  data: 수신한 데이터를 저장할 버퍼의 포인터
 * @param  len: 수신할 데이터의 길이 (바이트)
 * @return SPI_Status: 데이터 수신 결과
 * @note   이 함수는 모든 데이터가 수신될 때까지 대기합니다.
 * @warning
 *         - data 버퍼는 최소 len 바이트의 크기를 가져야 합니다.
 *         - 수신 도중 슬레이브가 응답하지 않으면 SPI_ERROR를 반환합니다.
 */
SPI_Status SPI_ReadData(SPI_TypeDef *SPIx, uint8_t *data, uint16_t len);

/**
 * @brief  SPI를 통해 데이터를 동시에 송수신합니다.
 * @param  SPIx: 사용할 SPI 주변장치 (SPI1, SPI2 또는 SPI3)
 * @param  txData: 전송할 데이터 버퍼의 포인터
 * @param  rxData: 수신한 데이터를 저장할 버퍼의 포인터
 * @param  len: 송수신할 데이터의 길이 (바이트)
 * @return SPI_Status: 데이터 송수신 결과
 * @note   이 함수는 모든 데이터가 송수신될 때까지 대기합니다.
 * @warning
 *         - txData와 rxData 버퍼는 각각 최소 len 바이트의 크기를 가져야 합니다.
 *         - 송수신 도중 슬레이브가 응답하지 않으면 SPI_ERROR를 반환합니다.
 */
SPI_Status SPI_TransferData(SPI_TypeDef *SPIx, uint8_t *txData, uint8_t *rxData, uint16_t len);

/**
 * @brief  NSS(Slave Select) 핀을 제어합니다.
 * @param  SPIx: 사용할 SPI 주변장치 (SPI1, SPI2 또는 SPI3)
 * @param  state: NSS 핀의 상태 (0: Low, 1: High)
 * @return None
 * @note   이 함수는 소프트웨어 NSS 모드에서만 동작합니다.
 * @warning 하드웨어 NSS 모드에서는 이 함수가 동작하지 않습니다.
 */
void SPI_SetNSS(SPI_TypeDef *SPIx, uint8_t state);

#endif /* __SPI_H */