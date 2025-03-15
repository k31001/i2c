#include "adc.h"
#include "rcc.h"

/**
 * @brief  ADC 주변장치를 초기화합니다.
 * @param  ADCx: 초기화할 ADC 주변장치 (ADC1, ADC2, ADC3)
 * @param  config: ADC 초기화 구조체 포인터
 * @return None
 */
void ADC_Init(ADC_TypeDef *ADCx, ADC_Config *config)
{
    // ADC 클럭 활성화
    if (ADCx == ADC1) {
        RCC->APB2ENR |= (1 << 8); // ADC1 클럭 활성화
    } else if (ADCx == ADC2) {
        RCC->APB2ENR |= (1 << 9); // ADC2 클럭 활성화
    } else if (ADCx == ADC3) {
        RCC->APB2ENR |= (1 << 10); // ADC3 클럭 활성화
    }
    
    // ADC 비활성화
    ADCx->CR2 &= ~(1 << 0); // ADON 비트 클리어
    
    // ADC 해상도 설정
    ADCx->CR1 &= ~(3 << 24); // RES 비트 클리어
    ADCx->CR1 |= (config->Resolution << 24); // 해상도 설정
    
    // 스캔 모드 설정
    if (config->ScanMode) {
        ADCx->CR1 |= (1 << 8); // SCAN 비트 설정
    } else {
        ADCx->CR1 &= ~(1 << 8); // SCAN 비트 클리어
    }
    
    // 데이터 정렬 설정
    if (config->DataAlign == ADC_ALIGN_LEFT) {
        ADCx->CR2 |= (1 << 11); // ALIGN 비트 설정 (왼쪽 정렬)
    } else {
        ADCx->CR2 &= ~(1 << 11); // ALIGN 비트 클리어 (오른쪽 정렬)
    }
    
    // 변환 모드 설정 (단일 또는 연속)
    if (config->Mode == ADC_MODE_CONTINUOUS) {
        ADCx->CR2 |= (1 << 1); // CONT 비트 설정 (연속 변환)
    } else {
        ADCx->CR2 &= ~(1 << 1); // CONT 비트 클리어 (단일 변환)
    }
    
    // 외부 트리거 설정
    if (config->Trigger != ADC_TRIGGER_DISABLE) {
        // 트리거 검출 활성화, 상승 에지
        ADCx->CR2 |= (1 << 28) | (1 << 29); // EXTEN[1:0] = 01 (상승 에지 검출)
        
        // 트리거 소스 설정
        ADCx->CR2 &= ~(0xF << 24); // EXTSEL[3:0] 클리어
        ADCx->CR2 |= ((config->Trigger & 0xF) << 24); // 트리거 소스 설정
    } else {
        // 외부 트리거 비활성화
        ADCx->CR2 &= ~(3 << 28); // EXTEN[1:0] = 00 (트리거 비활성화)
    }
    
    // ADC 활성화
    ADCx->CR2 |= (1 << 0); // ADON 비트 설정
    
    // 안정화 대기 시간 (최소 10µs)
    for (volatile uint32_t i = 0; i < 10000; i++);
}

/**
 * @brief  ADC 주변장치를 비활성화합니다.
 * @param  ADCx: 비활성화할 ADC 주변장치 (ADC1, ADC2, ADC3)
 * @return None
 */
void ADC_DeInit(ADC_TypeDef *ADCx)
{
    // ADC 비활성화
    ADCx->CR2 &= ~(1 << 0); // ADON 비트 클리어
    
    // ADC 클럭 비활성화
    if (ADCx == ADC1) {
        RCC->APB2ENR &= ~(1 << 8); // ADC1 클럭 비활성화
    } else if (ADCx == ADC2) {
        RCC->APB2ENR &= ~(1 << 9); // ADC2 클럭 비활성화
    } else if (ADCx == ADC3) {
        RCC->APB2ENR &= ~(1 << 10); // ADC3 클럭 비활성화
    }
}

/**
 * @brief  ADC 일반 채널을 구성합니다.
 * @param  ADCx: 구성할 ADC 주변장치 (ADC1, ADC2, ADC3)
 * @param  channel_config: 채널 구성 구조체 포인터
 * @return None
 */
void ADC_ConfigChannel(ADC_TypeDef *ADCx, ADC_ChannelConfig *channel_config)
{
    uint8_t channel = channel_config->Channel;
    uint8_t rank = channel_config->Rank;
    uint8_t sample_time = channel_config->SampleTime;
    
    // 레귤러 시퀀스 길이 설정 (1 이상의 변환 수)
    if (rank > 0) {
        uint8_t L = (rank > 16) ? 16 : rank;
        ADCx->SQR1 &= ~(0xF << 20); // L[3:0] 클리어
        ADCx->SQR1 |= ((L - 1) << 20); // L[3:0] 설정
    }
    
    // 채널 순서 설정
    if (rank > 0 && rank <= 6) {
        uint8_t offset = (rank - 1) * 5;
        ADCx->SQR3 &= ~(0x1F << offset); // SQx 클리어
        ADCx->SQR3 |= (channel << offset); // SQx 설정
    } else if (rank > 6 && rank <= 12) {
        uint8_t offset = (rank - 7) * 5;
        ADCx->SQR2 &= ~(0x1F << offset); // SQx 클리어
        ADCx->SQR2 |= (channel << offset); // SQx 설정
    } else if (rank > 12 && rank <= 16) {
        uint8_t offset = (rank - 13) * 5;
        ADCx->SQR1 &= ~(0x1F << offset); // SQx 클리어
        ADCx->SQR1 |= (channel << offset); // SQx 설정
    }
    
    // 채널 샘플링 시간 설정
    if (channel <= 9) {
        uint8_t offset = channel * 3;
        ADCx->SMPR2 &= ~(7 << offset); // SMPx 클리어
        ADCx->SMPR2 |= (sample_time << offset); // SMPx 설정
    } else if (channel <= 18) {
        uint8_t offset = (channel - 10) * 3;
        ADCx->SMPR1 &= ~(7 << offset); // SMPx 클리어
        ADCx->SMPR1 |= (sample_time << offset); // SMPx 설정
    }
}

/**
 * @brief  ADC 변환을 시작합니다.
 * @param  ADCx: 시작할 ADC 주변장치 (ADC1, ADC2, ADC3)
 * @return None
 */
void ADC_StartConversion(ADC_TypeDef *ADCx)
{
    // 외부 트리거가 구성되어 있지 않은 경우 소프트웨어 시작
    if ((ADCx->CR2 & (3 << 28)) == 0) {
        ADCx->CR2 |= (1 << 30); // SWSTART 비트 설정
    }
}

/**
 * @brief  ADC 변환을 중지합니다.
 * @param  ADCx: 중지할 ADC 주변장치 (ADC1, ADC2, ADC3)
 * @return None
 */
void ADC_StopConversion(ADC_TypeDef *ADCx)
{
    // ADC 비활성화
    ADCx->CR2 &= ~(1 << 0); // ADON 비트 클리어
    
    // 잠시 대기
    for (volatile uint32_t i = 0; i < 1000; i++);
    
    // ADC 다시 활성화
    ADCx->CR2 |= (1 << 0); // ADON 비트 설정
}

/**
 * @brief  ADC 변환 결과를 획득합니다.
 * @param  ADCx: 결과를 획득할 ADC 주변장치 (ADC1, ADC2, ADC3)
 * @return 변환 결과 값
 */
uint16_t ADC_GetValue(ADC_TypeDef *ADCx)
{
    // ADC 데이터 레지스터에서 변환 결과 읽기
    return (uint16_t)ADCx->DR;
}

/**
 * @brief  ADC 변환 완료 상태를 확인합니다.
 * @param  ADCx: 상태를 확인할 ADC 주변장치 (ADC1, ADC2, ADC3)
 * @return 0: 변환 진행 중, 1: 변환 완료
 */
uint8_t ADC_IsConversionComplete(ADC_TypeDef *ADCx)
{
    // EOC(End Of Conversion) 플래그 확인
    return (ADCx->SR & (1 << 1)) ? 1 : 0;
}

/**
 * @brief  ADC 내부 온도 센서를 활성화합니다.
 * @param  ADCx: 온도 센서를 활성화할 ADC 주변장치 (ADC1)
 * @return None
 */
void ADC_EnableTempSensor(ADC_TypeDef *ADCx)
{
    // 내부 온도 센서 활성화
    ADC123_COMMON->CCR |= (1 << 23); // TSVREFE 비트 설정
    
    // 안정화 대기 시간
    for (volatile uint32_t i = 0; i < 10000; i++);
}

/**
 * @brief  ADC 내부 온도 센서를 비활성화합니다.
 * @param  ADCx: 온도 센서를 비활성화할 ADC 주변장치 (ADC1)
 * @return None
 */
void ADC_DisableTempSensor(ADC_TypeDef *ADCx)
{
    // 내부 온도 센서 비활성화
    ADC123_COMMON->CCR &= ~(1 << 23); // TSVREFE 비트 클리어
}

/**
 * @brief  내부 온도 센서 판독 값을 섭씨 온도로 변환합니다.
 * @param  ADC_value: ADC 변환 결과 값
 * @param  vdda_mV: VDDA 전압 (mV)
 * @return 계산된 온도 (섭씨)
 */
float ADC_TempSensorValue(uint16_t ADC_value, uint16_t vdda_mV)
{
    float temperature;
    float vdda_volts = vdda_mV / 1000.0f;
    float adc_voltage = (ADC_value * vdda_volts) / 4095.0f; // 12비트 ADC 기준
    
    // STM32F411 데이터시트 기준 변환 공식
    // 공장 보정 값 (구현은 약간 다를 수 있음)
    #define TEMP_V25         0.76f    // 25°C에서 전압 값 (V)
    #define TEMP_AVG_SLOPE   2.5f     // mV/°C
    
    // 온도 계산
    temperature = ((adc_voltage - TEMP_V25) * 1000.0f) / TEMP_AVG_SLOPE + 25.0f;
    
    return temperature;
}

/**
 * @brief  내부 기준 전압(VREFINT)을 활성화합니다.
 * @param  ADCx: 기준 전압을 활성화할 ADC 주변장치 (ADC1)
 * @return None
 */
void ADC_EnableVREFINT(ADC_TypeDef *ADCx)
{
    // 내부 참조 전압 활성화
    ADC123_COMMON->CCR |= (1 << 22); // VBATE 비트 설정
    
    // 안정화 대기 시간
    for (volatile uint32_t i = 0; i < 10000; i++);
}

/**
 * @brief  내부 기준 전압(VREFINT)을 비활성화합니다.
 * @param  ADCx: 기준 전압을 비활성화할 ADC 주변장치 (ADC1)
 * @return None
 */
void ADC_DisableVREFINT(ADC_TypeDef *ADCx)
{
    // 내부 참조 전압 비활성화
    ADC123_COMMON->CCR &= ~(1 << 22); // VBATE 비트 클리어
}

/**
 * @brief  VREFINT 판독 값을 사용하여 VDDA 전압을 계산합니다.
 * @param  VREFINT_VALUE: VREFINT 채널의 ADC 변환 결과 값
 * @return 계산된 VDDA 전압 (mV)
 */
uint16_t ADC_GetVDDA(uint16_t VREFINT_VALUE)
{
    // STM32F411 기준 값
    #define VREFINT_CAL      *((uint16_t*)0x1FFF7A2A)  // 보정 값 주소
    #define VREFINT_VOLTAGE  1210                      // 1.21V (1210mV)
    
    // VDDA 계산 (mV)
    uint16_t vdda = (VREFINT_VOLTAGE * VREFINT_CAL) / VREFINT_VALUE;
    
    return vdda;
}

/**
 * @brief  배터리 전압 모니터링을 활성화합니다.
 * @param  ADCx: 배터리 전압 모니터링을 활성화할 ADC 주변장치 (ADC1)
 * @return None
 */
void ADC_EnableVBAT(ADC_TypeDef *ADCx)
{
    // 배터리 전압 모니터링 활성화
    ADC123_COMMON->CCR |= (1 << 22); // VBATE 비트 설정
    
    // 안정화 대기 시간
    for (volatile uint32_t i = 0; i < 10000; i++);
}

/**
 * @brief  배터리 전압 모니터링을 비활성화합니다.
 * @param  ADCx: 배터리 전압 모니터링을 비활성화할 ADC 주변장치 (ADC1)
 * @return None
 */
void ADC_DisableVBAT(ADC_TypeDef *ADCx)
{
    // 배터리 전압 모니터링 비활성화
    ADC123_COMMON->CCR &= ~(1 << 22); // VBATE 비트 클리어
}

/**
 * @brief  VBAT 판독 값을 실제 배터리 전압으로 변환합니다.
 * @param  VBAT_VALUE: VBAT 채널의 ADC 변환 결과 값
 * @param  vdda_mV: VDDA 전압 (mV)
 * @return 계산된 배터리 전압 (mV)
 */
uint16_t ADC_GetVBAT(uint16_t VBAT_VALUE, uint16_t vdda_mV)
{
    // VBAT는 1/4로 분압됨
    uint16_t vbat = (VBAT_VALUE * vdda_mV * 4) / 4095; // 12비트 ADC 기준
    
    return vbat;
}
