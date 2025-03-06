#ifndef __I2C_H
#define __I2C_H

#include "stm32f411xe.h"

/**
 * @brief I2C 통신 상태를 나타내는 열거형
 */
typedef enum
{
    I2C_OK = 0, /*!< 정상 동작 완료 */
    I2C_ERROR,  /*!< 일반적인 오류 발생 */
    I2C_BUSY,   /*!< I2C 버스가 사용 중 */
    I2C_TIMEOUT /*!< 타임아웃 발생 */
} I2C_Status;

/**
 * @brief I2C 초기화를 위한 설정 구조체
 */
typedef struct
{
    uint32_t ClockSpeed;   /*!< I2C 통신 속도 (Hz). 범위: 100KHz(표준모드) 또는 400KHz(고속모드) */
    uint8_t OwnAddress;    /*!< 자신의 슬레이브 주소 (7비트). 마스터 모드에서는 사용되지 않음 */
    uint8_t DutyCycle;     /*!< 고속 모드에서의 듀티 사이클. 0: 2:1, 1: 16:9 */
    uint8_t GeneralCall;   /*!< General Call 활성화 여부. 0: 비활성화, 1: 활성화 */
    uint8_t NoStretchMode; /*!< Clock Stretching 비활성화 여부. 0: 활성화(stretching 허용), 1: 비활성화 */
} I2C_Config;

/**
 * @brief  I2C 주변장치를 초기화합니다.
 * @param  I2Cx: 초기화할 I2C 주변장치 (I2C1 또는 I2C2)
 * @param  config: I2C 초기화 설정 구조체 포인터
 * @return None
 * @note   이 함수는 I2C 주변장치의 클럭을 활성화하고, 지정된 설정으로 초기화합니다.
 * @warning
 *         - 이 함수 호출 전에 해당 I2C 핀들이 올바르게 설정되어 있어야 합니다.
 *         - PCLK1(APB1) 클럭은 기본값 16MHz로 가정합니다. 다른 클럭 주파수를 사용하는 경우
 *           코드를 수정해야 합니다.
 */
void I2C_Init(I2C_TypeDef *I2Cx, I2C_Config *config);

/**
 * @brief  I2C 주변장치를 비활성화합니다.
 * @param  I2Cx: 비활성화할 I2C 주변장치 (I2C1 또는 I2C2)
 * @return None
 * @note   이 함수는 I2C 주변장치를 비활성화하고 클럭을 비활성화합니다.
 */
void I2C_DeInit(I2C_TypeDef *I2Cx);

/**
 * @brief  I2C 통신의 시작 조건을 생성합니다.
 * @param  I2Cx: 사용할 I2C 주변장치 (I2C1 또는 I2C2)
 * @return I2C_Status: 시작 조건 생성 결과
 * @note   이 함수는 버스가 유휴 상태일 때만 호출해야 합니다.
 * @warning 시작 조건 생성 후에는 반드시 주소 또는 데이터를 전송해야 합니다.
 */
I2C_Status I2C_Start(I2C_TypeDef *I2Cx);

/**
 * @brief  I2C 통신의 정지 조건을 생성합니다.
 * @param  I2Cx: 사용할 I2C 주변장치 (I2C1 또는 I2C2)
 * @return None
 * @note   이 함수는 현재 진행 중인 전송을 완료한 후 정지 조건을 생성합니다.
 */
void I2C_Stop(I2C_TypeDef *I2Cx);

/**
 * @brief  I2C를 통해 1바이트 데이터를 전송합니다.
 * @param  I2Cx: 사용할 I2C 주변장치 (I2C1 또는 I2C2)
 * @param  data: 전송할 1바이트 데이터
 * @return I2C_Status: 데이터 전송 결과
 * @note   이 함수는 데이터 전송이 완료될 때까지 대기합니다.
 * @remark 이 함수는 내부적으로 타임아웃을 체크합니다.
 */
I2C_Status I2C_WriteByte(I2C_TypeDef *I2Cx, uint8_t data);

/**
 * @brief  I2C를 통해 1바이트 데이터를 수신합니다.
 * @param  I2Cx: 사용할 I2C 주변장치 (I2C1 또는 I2C2)
 * @param  data: 수신한 데이터를 저장할 포인터
 * @param  ack: 수신 후 ACK 전송 여부 (0: NACK 전송, 1: ACK 전송)
 * @return I2C_Status: 데이터 수신 결과
 * @note   마지막 바이트 수신 시에는 ack를 0으로 설정해야 합니다.
 * @warning data 포인터는 반드시 유효한 메모리 주소를 가리켜야 합니다.
 */
I2C_Status I2C_ReadByte(I2C_TypeDef *I2Cx, uint8_t *data, uint8_t ack);

/**
 * @brief  I2C를 통해 여러 바이트의 데이터를 전송합니다.
 * @param  I2Cx: 사용할 I2C 주변장치 (I2C1 또는 I2C2)
 * @param  slaveAddr: 대상 슬레이브의 7비트 주소
 * @param  data: 전송할 데이터 버퍼의 포인터
 * @param  len: 전송할 데이터의 길이 (바이트)
 * @return I2C_Status: 데이터 전송 결과
 * @note   이 함수는 시작 조건 생성부터 정지 조건 생성까지 전체 전송 과정을 처리합니다.
 * @warning
 *         - data 버퍼는 최소 len 바이트의 크기를 가져야 합니다.
 *         - 전송 도중 슬레이브가 응답하지 않으면 I2C_ERROR를 반환합니다.
 */
I2C_Status I2C_WriteData(I2C_TypeDef *I2Cx, uint8_t slaveAddr, uint8_t *data, uint16_t len);

/**
 * @brief  I2C를 통해 여러 바이트의 데이터를 수신합니다.
 * @param  I2Cx: 사용할 I2C 주변장치 (I2C1 또는 I2C2)
 * @param  slaveAddr: 대상 슬레이브의 7비트 주소
 * @param  data: 수신한 데이터를 저장할 버퍼의 포인터
 * @param  len: 수신할 데이터의 길이 (바이트)
 * @return I2C_Status: 데이터 수신 결과
 * @note   이 함수는 시작 조건 생성부터 정지 조건 생성까지 전체 수신 과정을 처리합니다.
 * @warning
 *         - data 버퍼는 최소 len 바이트의 크기를 가져야 합니다.
 *         - 수신 도중 슬레이브가 응답하지 않으면 I2C_ERROR를 반환합니다.
 *         - 마지막 바이트 수신 시 자동으로 NACK를 전송합니다.
 */
I2C_Status I2C_ReadData(I2C_TypeDef *I2Cx, uint8_t slaveAddr, uint8_t *data, uint16_t len);

#endif /* __I2C_H */