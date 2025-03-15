#include "../adc.h"
#include "../gpio.h"
#include <stdio.h>

/**
 * @brief ADC 모든 기능 테스트
 * 
 * 이 테스트는 다음을 검증합니다:
 * 1. ADC 초기화
 * 2. 아날로그 채널 구성
 * 3. 단일 변환 수행
 * 4. 연속 변환 수행
 * 5. 내부 온도 센서 및 참조 전압 측정
 * 6. 다양한 해상도 테스트
 */
void ADC_Test(void)
{
    printf("===== ADC 드라이버 테스트 시작 =====\n");
    
    // ADC 핀 설정 (PA0 = ADC1_IN0)
    GPIO_Config adc_pin_config = {
        .Pin = (1 << 0),          // PA0
        .Mode = GPIO_MODE_ANALOG, // 아날로그 모드
        .Otype = GPIO_OTYPE_PUSHPULL,
        .Speed = GPIO_SPEED_HIGH,
        .PuPd = GPIO_PUPD_NONE
    };
    
    // GPIOA 클럭 활성화
    RCC->AHB1ENR |= (1 << 0); // GPIOA
    
    // GPIO 핀 초기화
    GPIO_Init(GPIOA, &adc_pin_config);
    
    printf("ADC1 핀 설정 완료 (PA0 = ADC1_IN0)\n");
    
    // 1. 기본 ADC 초기화 (단일 변환 모드, 12비트 해상도)
    ADC_Config adc_config = {
        .Resolution = ADC_RESOLUTION_12BIT,
        .Mode = ADC_MODE_SINGLE,
        .Trigger = ADC_TRIGGER_DISABLE,
        .ScanMode = 0,
        .DataAlign = ADC_ALIGN_RIGHT
    };
    
    printf("ADC1 초기화 (단일 변환 모드, 12비트 해상도)...\n");
    ADC_Init(ADC1, &adc_config);
    
    // 채널 설정
    ADC_ChannelConfig channel_config = {
        .Channel = ADC_CHANNEL_0,
        .SampleTime = ADC_SAMPLETIME_56CYCLES,
        .Rank = 1
    };
    
    ADC_ConfigChannel(ADC1, &channel_config);
    printf("ADC1 채널 0 설정 완료 (PA0)\n");
    
    // 단일 변환 테스트
    printf("\n단일 변환 테스트:\n");
    
    // 변환 시작
    printf("변환 시작...\n");
    ADC_StartConversion(ADC1);
    
    // 변환 완료 대기
    while (!ADC_IsConversionComplete(ADC1));
    
    // 변환 결과 읽기
    uint16_t adc_value = ADC_GetValue(ADC1);
    printf("변환 결과: %u (0x%04X)\n", adc_value, adc_value);
    printf("전압 값: %.3f V (3.3V 기준)\n", (float)adc_value * 3.3f / 4095.0f);
    
    // 2. 연속 변환 모드 테스트
    printf("\n연속 변환 모드 테스트:\n");
    adc_config.Mode = ADC_MODE_CONTINUOUS;
    ADC_Init(ADC1, &adc_config);
    
    // 변환 시작
    printf("연속 변환 시작...\n");
    ADC_StartConversion(ADC1);
    
    // 10회 데이터 샘플링
    printf("10회 샘플링:\n");
    for (int i = 0; i < 10; i++) {
        // 변환 완료 대기
        while (!ADC_IsConversionComplete(ADC1));
        
        // 변환 결과 읽기
        adc_value = ADC_GetValue(ADC1);
        printf("  샘플 %d: %u (%.3f V)\n", i+1, adc_value, (float)adc_value * 3.3f / 4095.0f);
        
        // 약간의 지연
        for (volatile uint32_t j = 0; j < 100000; j++);
    }
    
    // 연속 변환 중지
    ADC_StopConversion(ADC1);
    printf("연속 변환 중지됨\n");
    
    // 3. 다양한 해상도 테스트
    printf("\n다양한 해상도 테스트:\n");
    
    // 12비트 해상도
    adc_config.Resolution = ADC_RESOLUTION_12BIT;
    adc_config.Mode = ADC_MODE_SINGLE;
    ADC_Init(ADC1, &adc_config);
    
    printf("12비트 해상도 테스트:\n");
    ADC_StartConversion(ADC1);
    while (!ADC_IsConversionComplete(ADC1));
    adc_value = ADC_GetValue(ADC1);
    printf("  12비트 결과: %u (0x%04X) - 범위: 0-4095\n", adc_value, adc_value);
    
    // 10비트 해상도
    adc_config.Resolution = ADC_RESOLUTION_10BIT;
    ADC_Init(ADC1, &adc_config);
    
    printf("10비트 해상도 테스트:\n");
    ADC_StartConversion(ADC1);
    while (!ADC_IsConversionComplete(ADC1));
    adc_value = ADC_GetValue(ADC1);
    printf("  10비트 결과: %u (0x%04X) - 범위: 0-1023\n", adc_value, adc_value);
    
    // 8비트 해상도
    adc_config.Resolution = ADC_RESOLUTION_8BIT;
    ADC_Init(ADC1, &adc_config);
    
    printf("8비트 해상도 테스트:\n");
    ADC_StartConversion(ADC1);
    while (!ADC_IsConversionComplete(ADC1));
    adc_value = ADC_GetValue(ADC1);
    printf("  8비트 결과: %u (0x%04X) - 범위: 0-255\n", adc_value, adc_value);
    
    // 6비트 해상도
    adc_config.Resolution = ADC_RESOLUTION_6BIT;
    ADC_Init(ADC1, &adc_config);
    
    printf("6비트 해상도 테스트:\n");
    ADC_StartConversion(ADC1);
    while (!ADC_IsConversionComplete(ADC1));
    adc_value = ADC_GetValue(ADC1);
    printf("  6비트 결과: %u (0x%04X) - 범위: 0-63\n", adc_value, adc_value);
    
    // 4. 내부 온도 센서 테스트
    printf("\n내부 온도 센서 테스트:\n");
    
    // 12비트 해상도로 복원
    adc_config.Resolution = ADC_RESOLUTION_12BIT;
    ADC_Init(ADC1, &adc_config);
    
    // 내부 온도 센서 활성화
    ADC_EnableTempSensor(ADC1);
    printf("내부 온도 센서 활성화됨\n");
    
    // 온도 센서 채널 설정 (채널 16)
    channel_config.Channel = ADC_CHANNEL_16;
    channel_config.SampleTime = ADC_SAMPLETIME_480CYCLES; // 온도 센서에는 긴 샘플링 시간 필요
    ADC_ConfigChannel(ADC1, &channel_config);
    
    // 변환 시작
    ADC_StartConversion(ADC1);
    
    // 변환 완료 대기
    while (!ADC_IsConversionComplete(ADC1));
    
    // 온도 센서 판독 값
    adc_value = ADC_GetValue(ADC1);
    printf("온도 센서 ADC 값: %u\n", adc_value);
    
    // 온도 계산 (VDDA = 3300mV 가정)
    float temperature = ADC_TempSensorValue(adc_value, 3300);
    printf("계산된 온도: %.1f°C\n", temperature);
    
    // 내부 온도 센서 비활성화
    ADC_DisableTempSensor(ADC1);
    
    // 5. 내부 기준 전압 테스트
    printf("\n내부 기준 전압(VREFINT) 테스트:\n");
    
    // 내부 기준 전압 활성화
    ADC_EnableVREFINT(ADC1);
    printf("내부 기준 전압 활성화됨\n");
    
    // VREFINT 채널 설정 (채널 17)
    channel_config.Channel = ADC_CHANNEL_17;
    channel_config.SampleTime = ADC_SAMPLETIME_480CYCLES;
    ADC_ConfigChannel(ADC1, &channel_config);
    
    // 변환 시작
    ADC_StartConversion(ADC1);
    
    // 변환 완료 대기
    while (!ADC_IsConversionComplete(ADC1));
    
    // VREFINT 판독 값
    adc_value = ADC_GetValue(ADC1);
    printf("VREFINT ADC 값: %u\n", adc_value);
    
    // VDDA 계산
    uint16_t vdda = ADC_GetVDDA(adc_value);
    printf("계산된 VDDA: %u mV\n", vdda);
    
    // 내부 기준 전압 비활성화
    ADC_DisableVREFINT(ADC1);
    
    // 6. VBAT 모니터링 테스트
    printf("\nVBAT 모니터링 테스트:\n");
    
    // VBAT 모니터링 활성화
    ADC_EnableVBAT(ADC1);
    printf("VBAT 모니터링 활성화됨\n");
    
    // VBAT 채널 설정 (채널 18)
    channel_config.Channel = ADC_CHANNEL_18;
    channel_config.SampleTime = ADC_SAMPLETIME_480CYCLES;
    ADC_ConfigChannel(ADC1, &channel_config);
    
    // 변환 시작
    ADC_StartConversion(ADC1);
    
    // 변환 완료 대기
    while (!ADC_IsConversionComplete(ADC1));
    
    // VBAT 판독 값
    adc_value = ADC_GetValue(ADC1);
    printf("VBAT ADC 값: %u\n", adc_value);
    
    // 배터리 전압 계산 (위에서 계산한 VDDA 사용)
    uint16_t vbat = ADC_GetVBAT(adc_value, vdda);
    printf("계산된 VBAT: %u mV\n", vbat);
    
    // VBAT 모니터링 비활성화
    ADC_DisableVBAT(ADC1);
    
    // ADC 초기화 해제
    printf("\nADC1 초기화 해제\n");
    ADC_DeInit(ADC1);
    
    printf("===== ADC 드라이버 테스트 완료 =====\n\n");
}
