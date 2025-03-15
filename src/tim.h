#ifndef __TIM_H
#define __TIM_H

#include "stm32f411xe.h"

/**
 * @brief 타이머 채널 정의
 */
typedef enum
{
    TIM_CHANNEL_1 = 0,
    TIM_CHANNEL_2,
    TIM_CHANNEL_3,
    TIM_CHANNEL_4
} TIM_Channel;

/**
 * @brief 타이머 카운터 모드 정의
 */
typedef enum
{
    TIM_COUNTER_UP = 0,         /*!< 업 카운터 모드 */
    TIM_COUNTER_DOWN,           /*!< 다운 카운터 모드 */
    TIM_COUNTER_CENTER_ALIGNED1, /*!< 중앙 정렬 모드 1 */
    TIM_COUNTER_CENTER_ALIGNED2, /*!< 중앙 정렬 모드 2 */
    TIM_COUNTER_CENTER_ALIGNED3  /*!< 중앙 정렬 모드 3 */
} TIM_CounterMode;

/**
 * @brief 타이머 클럭 분주기 정의
 */
typedef enum
{
    TIM_CKD_DIV1 = 0,
    TIM_CKD_DIV2,
    TIM_CKD_DIV4
} TIM_ClockDivision;

/**
 * @brief 타이머 자동 리로드 프리로드 정의
 */
typedef enum
{
    TIM_AUTORELOAD_PRELOAD_DISABLE = 0,
    TIM_AUTORELOAD_PRELOAD_ENABLE
} TIM_AutoReloadPreload;

/**
 * @brief 타이머 출력 비교 모드 정의
 */
typedef enum
{
    TIM_OCMODE_FROZEN = 0,
    TIM_OCMODE_ACTIVE,
    TIM_OCMODE_INACTIVE,
    TIM_OCMODE_TOGGLE,
    TIM_OCMODE_FORCED_ACTIVE,
    TIM_OCMODE_FORCED_INACTIVE,
    TIM_OCMODE_PWM1,
    TIM_OCMODE_PWM2
} TIM_OCMode;

/**
 * @brief 타이머 출력 상태 정의
 */
typedef enum
{
    TIM_OUTPUT_STATE_DISABLE = 0,
    TIM_OUTPUT_STATE_ENABLE
} TIM_OutputState;

/**
 * @brief 타이머 출력 극성 정의
 */
typedef enum
{
    TIM_OCPOLARITY_HIGH = 0,
    TIM_OCPOLARITY_LOW
} TIM_OCPolarity;

/**
 * @brief 타이머 입력 캡처 극성 정의
 */
typedef enum
{
    TIM_ICPOLARITY_RISING = 0,
    TIM_ICPOLARITY_FALLING,
    TIM_ICPOLARITY_BOTH_EDGE
} TIM_ICPolarity;

/**
 * @brief 타이머 입력 캡처 선택 정의
 */
typedef enum
{
    TIM_ICSELECTION_DIRECT = 0,
    TIM_ICSELECTION_INDIRECT,
    TIM_ICSELECTION_TRC
} TIM_ICSelection;

/**
 * @brief 타이머 입력 캡처 프리스케일러 정의
 */
typedef enum
{
    TIM_ICPSC_DIV1 = 0,
    TIM_ICPSC_DIV2,
    TIM_ICPSC_DIV4,
    TIM_ICPSC_DIV8
} TIM_ICPSC;

/**
 * @brief 타이머 원-펄스 모드 정의
 */
typedef enum
{
    TIM_OPMODE_SINGLE = 0,
    TIM_OPMODE_REPETITIVE
} TIM_OPMode;

/**
 * @brief 타이머 기본 초기화 구조체
 */
typedef struct
{
    uint16_t Prescaler;         /*!< 타이머 프리스케일러 값 */
    TIM_CounterMode CounterMode; /*!< 타이머 카운터 모드 */
    uint32_t Period;            /*!< 타이머 주기 (자동 리로드 값) */
    TIM_ClockDivision ClockDivision; /*!< 타이머 클럭 분주 */
    TIM_AutoReloadPreload AutoReloadPreload; /*!< 자동 리로드 프리로드 활성화 여부 */
    uint8_t RepetitionCounter;   /*!< 반복 카운터 값 (고급 타이머만 해당) */
} TIM_Base_Config;

/**
 * @brief 타이머 출력 비교 초기화 구조체
 */
typedef struct
{
    TIM_OCMode OCMode;          /*!< 출력 비교 모드 */
    TIM_OutputState OutputState; /*!< 출력 상태 */
    uint32_t Pulse;             /*!< 출력 비교 펄스 값 */
    TIM_OCPolarity OCPolarity;   /*!< 출력 비교 극성 */
} TIM_OC_Config;

/**
 * @brief 타이머 입력 캡처 초기화 구조체
 */
typedef struct
{
    TIM_ICPolarity ICPolarity;   /*!< 입력 캡처 극성 */
    TIM_ICSelection ICSelection; /*!< 입력 캡처 선택 */
    TIM_ICPSC ICPrescaler;       /*!< 입력 캡처 프리스케일러 */
    uint8_t ICFilter;           /*!< 입력 캡처 필터 */
} TIM_IC_Config;

/**
 * @brief 타이머 원-펄스 모드 초기화 구조체
 */
typedef struct
{
    TIM_OPMode OPMode;          /*!< 원-펄스 모드 */
    TIM_OutputState OutputState; /*!< 출력 상태 */
    uint32_t Pulse;             /*!< 출력 비교 펄스 값 */
    TIM_OCPolarity OCPolarity;   /*!< 출력 비교 극성 */
} TIM_OnePulse_Config;

/**
 * @brief  타이머 기본 기능을 초기화합니다.
 * @param  TIMx: 초기화할 타이머 인스턴스 (TIM1-TIM11)
 * @param  config: 타이머 기본 초기화 구조체 포인터
 * @return None
 */
void TIM_Base_Init(TIM_TypeDef *TIMx, TIM_Base_Config *config);

/**
 * @brief  타이머 초기화를 해제합니다.
 * @param  TIMx: 초기화를 해제할 타이머 인스턴스 (TIM1-TIM11)
 * @return None
 */
void TIM_DeInit(TIM_TypeDef *TIMx);

/**
 * @brief  타이머를 시작합니다.
 * @param  TIMx: 시작할 타이머 인스턴스 (TIM1-TIM11)
 * @return None
 */
void TIM_Start(TIM_TypeDef *TIMx);

/**
 * @brief  타이머를 중지합니다.
 * @param  TIMx: 중지할 타이머 인스턴스 (TIM1-TIM11)
 * @return None
 */
void TIM_Stop(TIM_TypeDef *TIMx);

/**
 * @brief  타이머 카운터 값을 설정합니다.
 * @param  TIMx: 설정할 타이머 인스턴스 (TIM1-TIM11)
 * @param  counter: 새 카운터 값
 * @return None
 */
void TIM_SetCounter(TIM_TypeDef *TIMx, uint32_t counter);

/**
 * @brief  타이머 카운터 값을 얻습니다.
 * @param  TIMx: 값을 얻을 타이머 인스턴스 (TIM1-TIM11)
 * @return 현재 카운터 값
 */
uint32_t TIM_GetCounter(TIM_TypeDef *TIMx);

/**
 * @brief  타이머 주기를 설정합니다.
 * @param  TIMx: 설정할 타이머 인스턴스 (TIM1-TIM11)
 * @param  period: 새 주기 값 (자동 리로드 값)
 * @return None
 */
void TIM_SetPeriod(TIM_TypeDef *TIMx, uint32_t period);

/**
 * @brief  타이머 주기를 얻습니다.
 * @param  TIMx: 값을 얻을 타이머 인스턴스 (TIM1-TIM11)
 * @return 현재 주기 값
 */
uint32_t TIM_GetPeriod(TIM_TypeDef *TIMx);

/**
 * @brief  타이머 프리스케일러를 설정합니다.
 * @param  TIMx: 설정할 타이머 인스턴스 (TIM1-TIM11)
 * @param  prescaler: 새 프리스케일러 값
 * @return None
 */
void TIM_SetPrescaler(TIM_TypeDef *TIMx, uint16_t prescaler);

/**
 * @brief  타이머 프리스케일러를 얻습니다.
 * @param  TIMx: 값을 얻을 타이머 인스턴스 (TIM1-TIM11)
 * @return 현재 프리스케일러 값
 */
uint16_t TIM_GetPrescaler(TIM_TypeDef *TIMx);

/**
 * @brief  타이머 출력 비교 채널을 초기화합니다.
 * @param  TIMx: 초기화할 타이머 인스턴스 (TIM1-TIM11)
 * @param  channel: 초기화할 타이머 채널
 * @param  config: 출력 비교 초기화 구조체 포인터
 * @return None
 */
void TIM_OC_Init(TIM_TypeDef *TIMx, TIM_Channel channel, TIM_OC_Config *config);

/**
 * @brief  타이머 출력 비교 채널을 활성화합니다.
 * @param  TIMx: 활성화할 타이머 인스턴스 (TIM1-TIM11)
 * @param  channel: 활성화할 타이머 채널
 * @return None
 */
void TIM_OC_EnableChannel(TIM_TypeDef *TIMx, TIM_Channel channel);

/**
 * @brief  타이머 출력 비교 채널을 비활성화합니다.
 * @param  TIMx: 비활성화할 타이머 인스턴스 (TIM1-TIM11)
 * @param  channel: 비활성화할 타이머 채널
 * @return None
 */
void TIM_OC_DisableChannel(TIM_TypeDef *TIMx, TIM_Channel channel);

/**
 * @brief  타이머 출력 비교 펄스 값을 설정합니다.
 * @param  TIMx: 설정할 타이머 인스턴스 (TIM1-TIM11)
 * @param  channel: 설정할 타이머 채널
 * @param  pulse: 새 펄스 값
 * @return None
 */
void TIM_OC_SetPulse(TIM_TypeDef *TIMx, TIM_Channel channel, uint32_t pulse);

/**
 * @brief  타이머 입력 캡처 채널을 초기화합니다.
 * @param  TIMx: 초기화할 타이머 인스턴스 (TIM1-TIM11)
 * @param  channel: 초기화할 타이머 채널
 * @param  config: 입력 캡처 초기화 구조체 포인터
 * @return None
 */
void TIM_IC_Init(TIM_TypeDef *TIMx, TIM_Channel channel, TIM_IC_Config *config);

/**
 * @brief  타이머 입력 캡처 채널을 활성화합니다.
 * @param  TIMx: 활성화할 타이머 인스턴스 (TIM1-TIM11)
 * @param  channel: 활성화할 타이머 채널
 * @return None
 */
void TIM_IC_EnableChannel(TIM_TypeDef *TIMx, TIM_Channel channel);

/**
 * @brief  타이머 입력 캡처 채널을 비활성화합니다.
 * @param  TIMx: 비활성화할 타이머 인스턴스 (TIM1-TIM11)
 * @param  channel: 비활성화할 타이머 채널
 * @return None
 */
void TIM_IC_DisableChannel(TIM_TypeDef *TIMx, TIM_Channel channel);

/**
 * @brief  타이머 입력 캡처 값을 얻습니다.
 * @param  TIMx: 값을 얻을 타이머 인스턴스 (TIM1-TIM11)
 * @param  channel: 값을 얻을 타이머 채널
 * @return 캡처된 값
 */
uint32_t TIM_IC_GetCapture(TIM_TypeDef *TIMx, TIM_Channel channel);

/**
 * @brief  타이머 원-펄스 모드를 초기화합니다.
 * @param  TIMx: 초기화할 타이머 인스턴스 (TIM1-TIM11)
 * @param  channel: 초기화할 타이머 채널
 * @param  config: 원-펄스 모드 초기화 구조체 포인터
 * @return None
 */
void TIM_OnePulse_Init(TIM_TypeDef *TIMx, TIM_Channel channel, TIM_OnePulse_Config *config);

/**
 * @brief  타이머 원-펄스 모드를 활성화합니다.
 * @param  TIMx: 활성화할 타이머 인스턴스 (TIM1-TIM11)
 * @return None
 */
void TIM_OnePulse_Enable(TIM_TypeDef *TIMx);

/**
 * @brief  타이머 원-펄스 모드를 비활성화합니다.
 * @param  TIMx: 비활성화할 타이머 인스턴스 (TIM1-TIM11)
 * @return None
 */
void TIM_OnePulse_Disable(TIM_TypeDef *TIMx);

/**
 * @brief  타이머 인터럽트를 활성화합니다.
 * @param  TIMx: 인터럽트를 활성화할 타이머 인스턴스 (TIM1-TIM11)
 * @param  interrupt: 활성화할 인터럽트 (비트마스크)
 *                    TIM_IT_UPDATE, TIM_IT_CC1, TIM_IT_CC2, TIM_IT_CC3, TIM_IT_CC4, TIM_IT_COM, TIM_IT_TRIGGER, TIM_IT_BREAK
 * @return None
 */
void TIM_EnableInterrupt(TIM_TypeDef *TIMx, uint32_t interrupt);

/**
 * @brief  타이머 인터럽트를 비활성화합니다.
 * @param  TIMx: 인터럽트를 비활성화할 타이머 인스턴스 (TIM1-TIM11)
 * @param  interrupt: 비활성화할 인터럽트 (비트마스크)
 * @return None
 */
void TIM_DisableInterrupt(TIM_TypeDef *TIMx, uint32_t interrupt);

/**
 * @brief  타이머 인터럽트 플래그 상태를 얻습니다.
 * @param  TIMx: 상태를 얻을 타이머 인스턴스 (TIM1-TIM11)
 * @param  flag: 확인할 인터럽트 플래그
 * @return 플래그 상태 (0: 리셋, 1: 설정)
 */
uint8_t TIM_GetFlagStatus(TIM_TypeDef *TIMx, uint32_t flag);

/**
 * @brief  타이머 인터럽트 플래그를 클리어합니다.
 * @param  TIMx: 플래그를 클리어할 타이머 인스턴스 (TIM1-TIM11)
 * @param  flag: 클리어할 인터럽트 플래그
 * @return None
 */
void TIM_ClearFlag(TIM_TypeDef *TIMx, uint32_t flag);

/**
 * @brief  타이머 딜레이를 시작합니다(폴링 방식).
 * @param  TIMx: 사용할 타이머 인스턴스 (TIM1-TIM11)
 * @param  milliseconds: 지연 시간(밀리초)
 * @return None
 */
void TIM_Delay_ms(TIM_TypeDef *TIMx, uint32_t milliseconds);

#endif /* __TIM_H */
