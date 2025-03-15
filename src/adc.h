#ifndef __ADC_H
#define __ADC_H

#include "stm32f411xe.h"

/**
 * @brief ADC 채널 정의
 */
typedef enum
{
    ADC_CHANNEL_0 = 0,   /*!< ADC 채널 0 */
    ADC_CHANNEL_1,       /*!< ADC 채널 1 */
    ADC_CHANNEL_2,       /*!< ADC 채널 2 */
    ADC_CHANNEL_3,       /*!< ADC 채널 3 */
    ADC_CHANNEL_4,       /*!< ADC 채널 4 */
    ADC_CHANNEL_5,       /*!< ADC 채널 5 */
    ADC_CHANNEL_6,       /*!< ADC 채널 6 */
    ADC_CHANNEL_7,       /*!< ADC 채널 7 */
    ADC_CHANNEL_8,       /*!< ADC 채널 8 */
    ADC_CHANNEL_9,       /*!< ADC 채널 9 */
    ADC_CHANNEL_10,      /*!< ADC 채널 10 */
    ADC_CHANNEL_11,      /*!< ADC 채널 11 */
    ADC_CHANNEL_12,      /*!< ADC 채널 12 */
    ADC_CHANNEL_13,      /*!< ADC 채널 13 */
    ADC_CHANNEL_14,      /*!< ADC 채널 14 */
    ADC_CHANNEL_15,      /*!< ADC 채널 15 */
    ADC_CHANNEL_16,      /*!< ADC 채널 16 - 내부 온도 센서 */
    ADC_CHANNEL_17,      /*!< ADC 채널 17 - VREFINT */
    ADC_CHANNEL_18       /*!< ADC 채널 18 - VBAT/4 */
} ADC_Channel;

/**
 * @brief ADC 해상도 정의
 */
typedef enum
{
    ADC_RESOLUTION_12BIT = 0, /*!< 12비트 해상도 (0-4095) */
    ADC_RESOLUTION_10BIT,     /*!< 10비트 해상도 (0-1023) */
    ADC_RESOLUTION_8BIT,      /*!< 8비트 해상도 (0-255) */
    ADC_RESOLUTION_6BIT       /*!< 6비트 해상도 (0-63) */
} ADC_Resolution;

/**
 * @brief ADC 변환 모드 정의
 */
typedef enum
{
    ADC_MODE_SINGLE = 0,  /*!< 단일 변환 모드 */
    ADC_MODE_CONTINUOUS   /*!< 연속 변환 모드 */
} ADC_Mode;

/**
 * @brief ADC 외부 트리거 소스 정의 (채널 그룹에 따라 달라질 수 있음)
 */
typedef enum
{
    ADC_TRIGGER_DISABLE = 0, /*!< 외부 트리거 비활성화 */
    ADC_TRIGGER_TIM1_CC1,    /*!< 타이머 1 캡처/비교 1 */
    ADC_TRIGGER_TIM1_CC2,    /*!< 타이머 1 캡처/비교 2 */
    ADC_TRIGGER_TIM1_CC3,    /*!< 타이머 1 캡처/비교 3 */
    ADC_TRIGGER_TIM2_CC2,    /*!< 타이머 2 캡처/비교 2 */
    ADC_TRIGGER_TIM2_CC3,    /*!< 타이머 2 캡처/비교 3 */
    ADC_TRIGGER_TIM2_CC4,    /*!< 타이머 2 캡처/비교 4 */
    ADC_TRIGGER_TIM2_TRGO,   /*!< 타이머 2 TRGO 이벤트 */
    ADC_TRIGGER_TIM3_CC1,    /*!< 타이머 3 캡처/비교 1 */
    ADC_TRIGGER_TIM3_TRGO,   /*!< 타이머 3 TRGO 이벤트 */
    ADC_TRIGGER_TIM4_CC4,    /*!< 타이머 4 캡처/비교 4 */
    ADC_TRIGGER_TIM5_CC1,    /*!< 타이머 5 캡처/비교 1 */
    ADC_TRIGGER_TIM5_CC2,    /*!< 타이머 5 캡처/비교 2 */
    ADC_TRIGGER_TIM5_CC3,    /*!< 타이머 5 캡처/비교 3 */
    ADC_TRIGGER_EXTI11       /*!< 외부 인터럽트 라인 11 */
} ADC_Trigger;

/**
 * @brief ADC 데이터 정렬 정의
 */
typedef enum
{
    ADC_ALIGN_RIGHT = 0, /*!< 오른쪽 정렬 (LSB 정렬) */
    ADC_ALIGN_LEFT       /*!< 왼쪽 정렬 (MSB 정렬) */
} ADC_Align;

/**
 * @brief ADC 샘플링 시간 정의
 */
typedef enum
{
    ADC_SAMPLETIME_3CYCLES = 0,   /*!< 3 사이클 */
    ADC_SAMPLETIME_15CYCLES,      /*!< 15 사이클 */
    ADC_SAMPLETIME_28CYCLES,      /*!< 28 사이클 */
    ADC_SAMPLETIME_56CYCLES,      /*!< 56 사이클 */
    ADC_SAMPLETIME_84CYCLES,      /*!< 84 사이클 */
    ADC_SAMPLETIME_112CYCLES,     /*!< 112 사이클 */
    ADC_SAMPLETIME_144CYCLES,     /*!< 144 사이클 */
    ADC_SAMPLETIME_480CYCLES      /*!< 480 사이클 */
} ADC_SampleTime;

/**
 * @brief ADC 초기화 구조체
 */
typedef struct
{
    ADC_Resolution Resolution;   /*!< ADC 해상도 */
    ADC_Mode Mode;               /*!< 단일 또는 연속 변환 모드 */
    ADC_Trigger Trigger;         /*!< 외부 트리거 소스 */
    uint8_t ScanMode;            /*!< 스캔 모드 활성화 (0: 비활성화, 1: 활성화) */
    ADC_Align DataAlign;         /*!< 데이터 정렬 (오른쪽 또는 왼쪽) */
} ADC_Config;

/**
 * @brief ADC 채널 구성 구조체
 */
typedef struct
{
    ADC_Channel Channel;        /*!< ADC 채널 번호 */
    ADC_SampleTime SampleTime;  /*!< 채널의 샘플링 시간 */
    uint8_t Rank;               /*!< 변환 순서 (1-16) */
} ADC_ChannelConfig;

/**
 * @brief  ADC 주변장치를 초기화합니다.
 * @param  ADCx: 초기화할 ADC 주변장치 (ADC1, ADC2, ADC3)
 * @param  config: ADC 초기화 구조체 포인터
 * @return None
 */
void ADC_Init(ADC_TypeDef *ADCx, ADC_Config *config);

/**
 * @brief  ADC 주변장치를 비활성화합니다.
 * @param  ADCx: 비활성화할 ADC 주변장치 (ADC1, ADC2, ADC3)
 * @return None
 */
void ADC_DeInit(ADC_TypeDef *ADCx);

/**
 * @brief  ADC 일반 채널을 구성합니다.
 * @param  ADCx: 구성할 ADC 주변장치 (ADC1, ADC2, ADC3)
 * @param  channel_config: 채널 구성 구조체 포인터
 * @return None
 */
void ADC_ConfigChannel(ADC_TypeDef *ADCx, ADC_ChannelConfig *channel_config);

/**
 * @brief  ADC 변환을 시작합니다.
 * @param  ADCx: 시작할 ADC 주변장치 (ADC1, ADC2, ADC3)
 * @return None
 */
void ADC_StartConversion(ADC_TypeDef *ADCx);

/**
 * @brief  ADC 변환을 중지합니다.
 * @param  ADCx: 중지할 ADC 주변장치 (ADC1, ADC2, ADC3)
 * @return None
 */
void ADC_StopConversion(ADC_TypeDef *ADCx);

/**
 * @brief  ADC 변환 결과를 획득합니다.
 * @param  ADCx: 결과를 획득할 ADC 주변장치 (ADC1, ADC2, ADC3)
 * @return 변환 결과 값
 */
uint16_t ADC_GetValue(ADC_TypeDef *ADCx);

/**
 * @brief  ADC 변환 완료 상태를 확인합니다.
 * @param  ADCx: 상태를 확인할 ADC 주변장치 (ADC1, ADC2, ADC3)
 * @return 0: 변환 진행 중, 1: 변환 완료
 */
uint8_t ADC_IsConversionComplete(ADC_TypeDef *ADCx);

/**
 * @brief  ADC 내부 온도 센서를 활성화합니다.
 * @param  ADCx: 온도 센서를 활성화할 ADC 주변장치 (ADC1)
 * @return None
 */
void ADC_EnableTempSensor(ADC_TypeDef *ADCx);

/**
 * @brief  ADC 내부 온도 센서를 비활성화합니다.
 * @param  ADCx: 온도 센서를 비활성화할 ADC 주변장치 (ADC1)
 * @return None
 */
void ADC_DisableTempSensor(ADC_TypeDef *ADCx);

/**
 * @brief  내부 온도 센서 판독 값을 섭씨 온도로 변환합니다.
 * @param  ADC_value: ADC 변환 결과 값
 * @param  vdda_mV: VDDA 전압 (mV)
 * @return 계산된 온도 (섭씨)
 */
float ADC_TempSensorValue(uint16_t ADC_value, uint16_t vdda_mV);

/**
 * @brief  내부 기준 전압(VREFINT)을 활성화합니다.
 * @param  ADCx: 기준 전압을 활성화할 ADC 주변장치 (ADC1)
 * @return None
 */
void ADC_EnableVREFINT(ADC_TypeDef *ADCx);

/**
 * @brief  내부 기준 전압(VREFINT)을 비활성화합니다.
 * @param  ADCx: 기준 전압을 비활성화할 ADC 주변장치 (ADC1)
 * @return None
 */
void ADC_DisableVREFINT(ADC_TypeDef *ADCx);

/**
 * @brief  VREFINT 판독 값을 사용하여 VDDA 전압을 계산합니다.
 * @param  VREFINT_VALUE: VREFINT 채널의 ADC 변환 결과 값
 * @return 계산된 VDDA 전압 (mV)
 */
uint16_t ADC_GetVDDA(uint16_t VREFINT_VALUE);

/**
 * @brief  배터리 전압 모니터링을 활성화합니다.
 * @param  ADCx: 배터리 전압 모니터링을 활성화할 ADC 주변장치 (ADC1)
 * @return None
 */
void ADC_EnableVBAT(ADC_TypeDef *ADCx);

/**
 * @brief  배터리 전압 모니터링을 비활성화합니다.
 * @param  ADCx: 배터리 전압 모니터링을 비활성화할 ADC 주변장치 (ADC1)
 * @return None
 */
void ADC_DisableVBAT(ADC_TypeDef *ADCx);

/**
 * @brief  VBAT 판독 값을 실제 배터리 전압으로 변환합니다.
 * @param  VBAT_VALUE: VBAT 채널의 ADC 변환 결과 값
 * @param  vdda_mV: VDDA 전압 (mV)
 * @return 계산된 배터리 전압 (mV)
 */
uint16_t ADC_GetVBAT(uint16_t VBAT_VALUE, uint16_t vdda_mV);

#endif /* __ADC_H */
