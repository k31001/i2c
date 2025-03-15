#include "../tim.h"
#include "../gpio.h"
#include <stdio.h>

/**
 * @brief 타이머 기본 기능 테스트
 * 
 * 다음 기능을 테스트합니다:
 * 1. 타이머 초기화
 * 2. 타이머 카운터 값 설정 및 읽기
 * 3. 타이머 프리스케일러 및 주기 설정
 */
void TIM_Base_Test(void)
{
    printf("===== 타이머 기본 기능 테스트 시작 =====\n");
    
    // 타이머 초기화 구조체 설정
    TIM_Base_Config config;
    config.Prescaler = 8400 - 1;         // 84MHz / 8400 = 10KHz
    config.CounterMode = TIM_COUNTER_UP; // 업 카운터 모드
    config.Period = 1000 - 1;            // 10KHz / 1000 = 10Hz (100ms 주기)
    config.ClockDivision = TIM_CKD_DIV1;
    config.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    config.RepetitionCounter = 0;
    
    printf("타이머 초기화 중... (프리스케일러: %d, 주기: %d)\n", 
           config.Prescaler, config.Period);
    
    // TIM2 초기화
    TIM_Base_Init(TIM2, &config);
    
    printf("타이머 초기화 완료\n");
    
    // 타이머 시작
    printf("타이머 카운팅 시작...\n");
    TIM_Start(TIM2);
    
    // 카운터 값 확인 (몇 번 샘플링)
    printf("카운터 값 모니터링:\n");
    for (int i = 0; i < 5; i++) {
        uint32_t counter = TIM_GetCounter(TIM2);
        printf("  샘플 %d: 카운터 값 = %lu\n", i+1, counter);
        
        // 약간의 지연
        for (volatile uint32_t j = 0; j < 1000000; j++);
    }
    
    // 카운터 값 설정
    printf("카운터 값을 500으로 설정\n");
    TIM_SetCounter(TIM2, 500);
    
    // 설정된 값 확인
    printf("설정된 카운터 값 = %lu\n", TIM_GetCounter(TIM2));
    
    // 주기 변경 확인
    printf("주기 값을 2000으로 변경\n");
    TIM_SetPeriod(TIM2, 2000);
    
    // 변경된 주기 확인
    printf("변경된 주기 값 = %lu\n", TIM_GetPeriod(TIM2));
    
    // 프리스케일러 변경 확인
    printf("프리스케일러 값을 4200으로 변경\n");
    TIM_SetPrescaler(TIM2, 4200);
    
    // 변경된 프리스케일러 확인
    printf("변경된 프리스케일러 값 = %u\n", TIM_GetPrescaler(TIM2));
    
    // 타이머 중지
    printf("타이머 중지\n");
    TIM_Stop(TIM2);
    
    // 중지 확인
    printf("타이머 중지 후 카운터 값 = %lu\n", TIM_GetCounter(TIM2));
    
    // 잠시 후 다시 확인
    for (volatile uint32_t j = 0; j < 1000000; j++);
    printf("지연 후 카운터 값 = %lu (중지 상태 확인)\n", TIM_GetCounter(TIM2));
    
    // 타이머 초기화 해제
    printf("타이머 초기화 해제\n");
    TIM_DeInit(TIM2);
    
    printf("===== 타이머 기본 기능 테스트 완료 =====\n\n");
}

/**
 * @brief 타이머 출력 비교 모드 테스트
 * 
 * 다음 기능을 테스트합니다:
 * 1. PWM 출력 설정
 * 2. 출력 비교 채널 제어
 * 3. PWM 듀티 사이클 변경
 */
void TIM_PWM_Test(void)
{
    printf("===== 타이머 PWM 출력 테스트 시작 =====\n");
    
    // GPIO 핀 설정 (TIM4_CH1 - PB6)
    GPIO_Config gpio_config;
    gpio_config.Mode = GPIO_MODE_AF;
    gpio_config.Pull = GPIO_NOPULL;
    gpio_config.Speed = GPIO_SPEED_HIGH;
    gpio_config.Alternate = 2;  // TIM4의 대체 기능
    
    printf("PWM 출력을 위한 GPIO 핀 설정 (PB6 - TIM4_CH1)\n");
    GPIO_Init(GPIOB, GPIO_PIN_6, &gpio_config);
    
    // 타이머 기본 설정
    TIM_Base_Config tim_config;
    tim_config.Prescaler = 84 - 1;        // 84MHz / 84 = 1MHz
    tim_config.CounterMode = TIM_COUNTER_UP;
    tim_config.Period = 1000 - 1;         // 1MHz / 1000 = 1KHz (1ms PWM 주기)
    tim_config.ClockDivision = TIM_CKD_DIV1;
    tim_config.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    tim_config.RepetitionCounter = 0;
    
    printf("타이머 기본 설정 (주파수: 1KHz)\n");
    TIM_Base_Init(TIM4, &tim_config);
    
    // 출력 비교 채널 설정
    TIM_OC_Config oc_config;
    oc_config.OCMode = TIM_OCMODE_PWM1;
    oc_config.OutputState = TIM_OUTPUT_STATE_ENABLE;
    oc_config.Pulse = 500;  // 50% 듀티 사이클 (500/1000)
    oc_config.OCPolarity = TIM_OCPOLARITY_HIGH;
    
    printf("출력 비교 채널 설정 (PWM 모드, 50% 듀티 사이클)\n");
    TIM_OC_Init(TIM4, TIM_CHANNEL_1, &oc_config);
    
    // 채널 활성화
    printf("출력 비교 채널 활성화\n");
    TIM_OC_EnableChannel(TIM4, TIM_CHANNEL_1);
    
    // 타이머 시작
    printf("타이머 시작 (PWM 출력 시작)\n");
    TIM_Start(TIM4);
    
    // 여러 듀티 사이클 테스트
    printf("다양한 듀티 사이클 테스트 중...\n");
    
    printf("10% 듀티 사이클로 변경\n");
    TIM_OC_SetPulse(TIM4, TIM_CHANNEL_1, 100);
    for (volatile uint32_t j = 0; j < 10000000; j++);
    
    printf("25% 듀티 사이클로 변경\n");
    TIM_OC_SetPulse(TIM4, TIM_CHANNEL_1, 250);
    for (volatile uint32_t j = 0; j < 10000000; j++);
    
    printf("75% 듀티 사이클로 변경\n");
    TIM_OC_SetPulse(TIM4, TIM_CHANNEL_1, 750);
    for (volatile uint32_t j = 0; j < 10000000; j++);
    
    printf("90% 듀티 사이클로 변경\n");
    TIM_OC_SetPulse(TIM4, TIM_CHANNEL_1, 900);
    for (volatile uint32_t j = 0; j < 10000000; j++);
    
    // 채널 비활성화
    printf("출력 비교 채널 비활성화\n");
    TIM_OC_DisableChannel(TIM4, TIM_CHANNEL_1);
    
    // 타이머 중지
    printf("타이머 중지\n");
    TIM_Stop(TIM4);
    
    // 타이머 초기화 해제
    printf("타이머 초기화 해제\n");
    TIM_DeInit(TIM4);
    
    printf("===== 타이머 PWM 출력 테스트 완료 =====\n\n");
}

/**
 * @brief 타이머 입력 캡처 테스트
 * 
 * 다음 기능을 테스트합니다:
 * 1. 입력 캡처 초기화
 * 2. 입력 신호 캡처 및 값 확인
 */
void TIM_InputCapture_Test(void)
{
    printf("===== 타이머 입력 캡처 테스트 시작 =====\n");
    
    // GPIO 핀 설정 (TIM3_CH1 - PA6)
    GPIO_Config gpio_config;
    gpio_config.Mode = GPIO_MODE_AF;
    gpio_config.Pull = GPIO_PULLUP;
    gpio_config.Speed = GPIO_SPEED_HIGH;
    gpio_config.Alternate = 2;  // TIM3의 대체 기능
    
    printf("입력 캡처를 위한 GPIO 핀 설정 (PA6 - TIM3_CH1)\n");
    GPIO_Init(GPIOA, GPIO_PIN_6, &gpio_config);
    
    // 타이머 기본 설정
    TIM_Base_Config tim_config;
    tim_config.Prescaler = 84 - 1;        // 84MHz / 84 = 1MHz
    tim_config.CounterMode = TIM_COUNTER_UP;
    tim_config.Period = 0xFFFF;           // 최대 주기
    tim_config.ClockDivision = TIM_CKD_DIV1;
    tim_config.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    tim_config.RepetitionCounter = 0;
    
    printf("타이머 기본 설정 (1MHz 타이머 클럭)\n");
    TIM_Base_Init(TIM3, &tim_config);
    
    // 입력 캡처 채널 설정
    TIM_IC_Config ic_config;
    ic_config.ICPolarity = TIM_ICPOLARITY_RISING;
    ic_config.ICSelection = TIM_ICSELECTION_DIRECT;
    ic_config.ICPrescaler = TIM_ICPSC_DIV1;
    ic_config.ICFilter = 0;
    
    printf("입력 캡처 채널 설정 (상승 에지 캡처)\n");
    TIM_IC_Init(TIM3, TIM_CHANNEL_1, &ic_config);
    
    // 입력 캡처 인터럽트 활성화
    printf("입력 캡처 인터럽트 활성화\n");
    TIM_EnableInterrupt(TIM3, TIM_IT_CC1);
    
    // 채널 활성화
    printf("입력 캡처 채널 활성화\n");
    TIM_IC_EnableChannel(TIM3, TIM_CHANNEL_1);
    
    // 타이머 시작
    printf("타이머 시작 (입력 캡처 시작)\n");
    TIM_Start(TIM3);
    
    // 테스트를 위한 시뮬레이션된 입력 캡처 이벤트
    printf("입력 캡처 이벤트 대기 중... (실제 하드웨어에서는 외부 신호 필요)\n");
    printf("이 테스트는 실제 신호가 필요합니다. 시뮬레이션된 결과만 표시합니다.\n");
    
    // 캡처 플래그를 인위적으로 설정 (실제 하드웨어에서는 외부 신호에 의해 설정됨)
    TIM3->SR |= TIM_SR_CC1IF;
    TIM3->CCR1 = 12345;  // 시뮬레이션된 캡처 값
    
    // 캡처 플래그 확인
    if (TIM_GetFlagStatus(TIM3, TIM_SR_CC1IF)) {
        printf("입력 캡처 이벤트 감지!\n");
        uint32_t capture_value = TIM_IC_GetCapture(TIM3, TIM_CHANNEL_1);
        printf("캡처된 값: %lu (1MHz 카운터 클럭 기준)\n", capture_value);
        
        // 실제 주파수 계산 (시뮬레이션)
        printf("시뮬레이션 된 입력 신호 주파수: %.2f Hz\n", 1000000.0 / capture_value);
        
        // 플래그 클리어
        TIM_ClearFlag(TIM3, TIM_SR_CC1IF);
    } else {
        printf("캡처 이벤트가 발생하지 않았습니다.\n");
    }
    
    // 채널 비활성화
    printf("입력 캡처 채널 비활성화\n");
    TIM_IC_DisableChannel(TIM3, TIM_CHANNEL_1);
    
    // 타이머 중지
    printf("타이머 중지\n");
    TIM_Stop(TIM3);
    
    // 타이머 초기화 해제
    printf("타이머 초기화 해제\n");
    TIM_DeInit(TIM3);
    
    printf("===== 타이머 입력 캡처 테스트 완료 =====\n\n");
}

/**
 * @brief 타이머 딜레이 함수 테스트
 */
void TIM_Delay_Test(void)
{
    printf("===== 타이머 딜레이 함수 테스트 시작 =====\n");
    
    // 다양한 딜레이 테스트
    printf("100ms 딜레이 시작...\n");
    TIM_Delay_ms(TIM5, 100);
    printf("100ms 딜레이 완료\n");
    
    printf("500ms 딜레이 시작...\n");
    TIM_Delay_ms(TIM5, 500);
    printf("500ms 딜레이 완료\n");
    
    printf("1000ms 딜레이 시작...\n");
    TIM_Delay_ms(TIM5, 1000);
    printf("1000ms 딜레이 완료\n");
    
    printf("===== 타이머 딜레이 함수 테스트 완료 =====\n\n");
}

/**
 * @brief 모든 타이머 테스트 실행
 */
void TIM_Test(void)
{
    printf("\n\n===== 타이머 드라이버 테스트 시작 =====\n\n");
    
    // 기본 기능 테스트
    TIM_Base_Test();
    
    // PWM 출력 테스트
    TIM_PWM_Test();
    
    // 입력 캡처 테스트
    TIM_InputCapture_Test();
    
    // 딜레이 함수 테스트
    TIM_Delay_Test();
    
    printf("===== 타이머 드라이버 테스트 완료 =====\n\n");
}
