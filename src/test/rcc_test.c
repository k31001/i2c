#include "../rcc.h"
#include <stdio.h>

/**
 * @brief RCC 모든 기능 테스트
 * 
 * 이 테스트는 다음을 검증합니다:
 * 1. 시스템 클럭 설정 (HSI, HSE, PLL)
 * 2. 주변장치 클럭 활성화/비활성화
 * 3. 주변장치 리셋
 * 4. 클럭 주파수 획득
 */
void RCC_Test(void)
{
    printf("===== RCC 드라이버 테스트 시작 =====\n");
    
    // 1. 기본 HSI 클럭 정보 확인
    printf("현재 시스템 클럭 정보:\n");
    printf("시스템 클럭: %lu Hz\n", RCC_GetSystemClock());
    printf("HCLK: %lu Hz\n", RCC_GetHCLK());
    printf("PCLK1: %lu Hz\n", RCC_GetPCLK1());
    printf("PCLK2: %lu Hz\n", RCC_GetPCLK2());
    
    // 2. HSE 클럭으로 전환
    printf("\nHSE 클럭으로 전환 시도...\n");
    RCC_ClockConfig hse_config = {
        .ClockSource = RCC_CLOCKSOURCE_HSE,
        .AHBPrescaler = RCC_AHB_PRESCALER_1,    // 분주 없음
        .APB1Prescaler = RCC_APB_PRESCALER_2,   // APB1은 2분주 (최대 42MHz)
        .APB2Prescaler = RCC_APB_PRESCALER_1    // APB2는 분주 없음
    };
    
    uint8_t result = RCC_SetSystemClock(&hse_config);
    if (result == 0)
        printf("HSE 클럭 설정 성공\n");
    else
        printf("HSE 클럭 설정 실패 (HSE 오실레이터가 연결되지 않았을 수 있음)\n");
    
    // HSE 설정 성공 여부와 관계없이 계속 진행
    
    // 3. PLL 클럭으로 전환 (84MHz 설정)
    printf("\nPLL 클럭으로 전환 시도 (84MHz)...\n");
    RCC_ClockConfig pll_config = {
        .ClockSource = RCC_CLOCKSOURCE_PLL,
        .PLLSource = RCC_PLLSOURCE_HSI,  // HSI를 PLL 소스로 사용
        .PLLM = 16,                      // HSI 16MHz / 16 = 1MHz
        .PLLN = 336,                     // 1MHz * 336 = 336MHz
        .PLLP = 4,                       // 336MHz / 4 = 84MHz (SYSCLK)
        .AHBPrescaler = RCC_AHB_PRESCALER_1,    // HCLK = 84MHz
        .APB1Prescaler = RCC_APB_PRESCALER_2,   // PCLK1 = 42MHz (APB1 최대)
        .APB2Prescaler = RCC_APB_PRESCALER_1    // PCLK2 = 84MHz
    };
    
    result = RCC_SetSystemClock(&pll_config);
    if (result == 0)
        printf("PLL 클럭 설정 성공\n");
    else
        printf("PLL 클럭 설정 실패\n");
    
    // PLL 설정 후 클럭 정보 확인
    printf("\n새로운 시스템 클럭 정보:\n");
    printf("시스템 클럭: %lu Hz\n", RCC_GetSystemClock());
    printf("HCLK: %lu Hz\n", RCC_GetHCLK());
    printf("PCLK1: %lu Hz\n", RCC_GetPCLK1());
    printf("PCLK2: %lu Hz\n", RCC_GetPCLK2());
    
    // 4. 주변장치 클럭 활성화/비활성화 테스트
    printf("\n주변장치 클럭 제어 테스트:\n");
    
    // AHB1 주변장치 (GPIO) 테스트
    printf("GPIOA 클럭 활성화\n");
    RCC_EnableAHB1Peripheral(RCC_AHB1_GPIOA);
    
    printf("GPIOA 클럭 비활성화\n");
    RCC_DisableAHB1Peripheral(RCC_AHB1_GPIOA);
    
    // APB1 주변장치 (I2C, SPI) 테스트
    printf("I2C1 클럭 활성화\n");
    RCC_EnableAPB1Peripheral(RCC_APB1_I2C1);
    
    printf("SPI2 클럭 활성화\n");
    RCC_EnableAPB1Peripheral(RCC_APB1_SPI2);
    
    printf("I2C1, SPI2 클럭 비활성화\n");
    RCC_DisableAPB1Peripheral(RCC_APB1_I2C1 | RCC_APB1_SPI2);
    
    // APB2 주변장치 (USART, SPI) 테스트
    printf("USART1 클럭 활성화\n");
    RCC_EnableAPB2Peripheral(RCC_APB2_USART1);
    
    printf("SPI1 클럭 활성화\n");
    RCC_EnableAPB2Peripheral(RCC_APB2_SPI1);
    
    printf("USART1, SPI1 클럭 비활성화\n");
    RCC_DisableAPB2Peripheral(RCC_APB2_USART1 | RCC_APB2_SPI1);
    
    // 5. 주변장치 리셋 테스트
    printf("\n주변장치 리셋 테스트:\n");
    
    printf("GPIOB 리셋\n");
    RCC_ResetAHB1Peripheral(RCC_AHB1_GPIOB);
    
    printf("I2C2 리셋\n");
    RCC_ResetAPB1Peripheral(RCC_APB1_I2C2);
    
    printf("USART6 리셋\n");
    RCC_ResetAPB2Peripheral(RCC_APB2_USART6);
    
    // 6. 다시 기본 HSI 클럭으로 복원
    printf("\nHSI 클럭으로 복원 시도...\n");
    RCC_ClockConfig hsi_config = {
        .ClockSource = RCC_CLOCKSOURCE_HSI,
        .AHBPrescaler = RCC_AHB_PRESCALER_1,
        .APB1Prescaler = RCC_APB_PRESCALER_1,
        .APB2Prescaler = RCC_APB_PRESCALER_1
    };
    
    result = RCC_SetSystemClock(&hsi_config);
    if (result == 0)
        printf("HSI 클럭 설정 성공\n");
    else
        printf("HSI 클럭 설정 실패\n");
    
    printf("===== RCC 드라이버 테스트 완료 =====\n\n");
}
