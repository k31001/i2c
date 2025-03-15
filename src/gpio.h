#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f411xe.h"

/**
 * @brief GPIO 핀 설정을 위한 모드 정의
 */
typedef enum
{
    GPIO_MODE_INPUT = 0,  /*!< 입력 모드 */
    GPIO_MODE_OUTPUT,     /*!< 출력 모드 */
    GPIO_MODE_ALT,        /*!< 대체 기능 모드 */
    GPIO_MODE_ANALOG      /*!< 아날로그 모드 */
} GPIO_Mode;

/**
 * @brief GPIO 출력 타입 정의
 */
typedef enum
{
    GPIO_OTYPE_PUSHPULL = 0,  /*!< 푸시풀 출력 */
    GPIO_OTYPE_OPENDRAIN      /*!< 오픈 드레인 출력 */
} GPIO_OutputType;

/**
 * @brief GPIO 출력 속도 정의
 */
typedef enum
{
    GPIO_SPEED_LOW = 0,    /*!< 저속 */
    GPIO_SPEED_MEDIUM,     /*!< 중속 */
    GPIO_SPEED_HIGH,       /*!< 고속 */
    GPIO_SPEED_VERYHIGH    /*!< 초고속 */
} GPIO_Speed;

/**
 * @brief GPIO 풀업/풀다운 설정 정의
 */
typedef enum
{
    GPIO_PUPD_NONE = 0,    /*!< 풀업/풀다운 없음 */
    GPIO_PUPD_PULLUP,      /*!< 풀업 */
    GPIO_PUPD_PULLDOWN     /*!< 풀다운 */
} GPIO_PuPd;

/**
 * @brief GPIO 대체 기능 정의
 */
typedef enum
{
    GPIO_AF0 = 0,  /*!< 대체 기능 0 */
    GPIO_AF1,      /*!< 대체 기능 1 */
    GPIO_AF2,      /*!< 대체 기능 2 */
    GPIO_AF3,      /*!< 대체 기능 3 */
    GPIO_AF4,      /*!< 대체 기능 4 */
    GPIO_AF5,      /*!< 대체 기능 5 */
    GPIO_AF6,      /*!< 대체 기능 6 */
    GPIO_AF7,      /*!< 대체 기능 7 */
    GPIO_AF8,      /*!< 대체 기능 8 */
    GPIO_AF9,      /*!< 대체 기능 9 */
    GPIO_AF10,     /*!< 대체 기능 10 */
    GPIO_AF11,     /*!< 대체 기능 11 */
    GPIO_AF12,     /*!< 대체 기능 12 */
    GPIO_AF13,     /*!< 대체 기능 13 */
    GPIO_AF14,     /*!< 대체 기능 14 */
    GPIO_AF15      /*!< 대체 기능 15 */
} GPIO_AlternateFunction;

/**
 * @brief GPIO 핀 설정 구조체
 */
typedef struct
{
    uint16_t Pin;            /*!< GPIO 핀 번호 (0-15) */
    GPIO_Mode Mode;          /*!< GPIO 모드 */
    GPIO_OutputType Otype;   /*!< GPIO 출력 타입 (출력 모드에서만 사용) */
    GPIO_Speed Speed;        /*!< GPIO 속도 (출력 모드에서만 사용) */
    GPIO_PuPd PuPd;          /*!< GPIO 풀업/풀다운 설정 */
    GPIO_AlternateFunction AF; /*!< GPIO 대체 기능 (대체 기능 모드에서만 사용) */
} GPIO_Config;

/**
 * @brief  GPIO 핀을 초기화합니다.
 * @param  GPIOx: 초기화할 GPIO 포트 (GPIOA-GPIOH)
 * @param  config: GPIO 초기화 설정 구조체 포인터
 * @return None
 * @note   이 함수는 GPIO 포트의 클럭을 활성화하고 지정된 설정으로 초기화합니다.
 */
void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_Config *config);

/**
 * @brief  GPIO 핀 설정을 초기 상태로 재설정합니다.
 * @param  GPIOx: 초기화할 GPIO 포트 (GPIOA-GPIOH)
 * @param  pin: 초기화할 핀 번호 (0-15)
 * @return None
 */
void GPIO_DeInit(GPIO_TypeDef *GPIOx, uint16_t pin);

/**
 * @brief  GPIO 핀의 출력 값을 설정합니다.
 * @param  GPIOx: GPIO 포트 (GPIOA-GPIOH)
 * @param  pin: 핀 번호 (0-15)
 * @param  state: 설정할 상태 (0: Low, 1: High)
 * @return None
 */
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t pin, uint8_t state);

/**
 * @brief  GPIO 핀의 입력 값을 읽습니다.
 * @param  GPIOx: GPIO 포트 (GPIOA-GPIOH)
 * @param  pin: 핀 번호 (0-15)
 * @return 핀의 입력 상태 (0: Low, 1: High)
 */
uint8_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t pin);

/**
 * @brief  GPIO 핀의 상태를 반전시킵니다.
 * @param  GPIOx: GPIO 포트 (GPIOA-GPIOH)
 * @param  pin: 핀 번호 (0-15)
 * @return None
 */
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t pin);

/**
 * @brief  GPIO 포트의 출력 값을 설정합니다.
 * @param  GPIOx: GPIO 포트 (GPIOA-GPIOH)
 * @param  value: 설정할 16비트 값
 * @return None
 */
void GPIO_WritePort(GPIO_TypeDef *GPIOx, uint16_t value);

/**
 * @brief  GPIO 포트의 입력 값을 읽습니다.
 * @param  GPIOx: GPIO 포트 (GPIOA-GPIOH)
 * @return 포트의 16비트 입력 값
 */
uint16_t GPIO_ReadPort(GPIO_TypeDef *GPIOx);

#endif /* __GPIO_H */
