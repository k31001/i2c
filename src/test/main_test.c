#include <stdio.h>
#include "../stm32f411xe.h"

// 각 테스트 함수 선언
extern void GPIO_Test(void);
extern void RCC_Test(void);
extern void I2C_Test(void);
extern void USART_Test(void);
extern void SPI_Test(void);

/**
 * @brief 메인 테스트 함수
 * 
 * 모든 주변장치 드라이버에 대한 테스트를 순차적으로 실행합니다.
 */
int main(void)
{
    printf("====================================================\n");
    printf("  STM32F411 주변장치 드라이버 통합 테스트 시작\n");
    printf("====================================================\n\n");
    
    // RCC 테스트 (다른 모든 주변장치가 RCC에 의존하므로 먼저 테스트)
    RCC_Test();
    
    // GPIO 테스트
    GPIO_Test();
    
    // I2C 테스트
    I2C_Test();
    
    // SPI 테스트
    SPI_Test();
    
    // USART 테스트
    USART_Test();
    
    printf("====================================================\n");
    printf("  모든 테스트 완료\n");
    printf("====================================================\n");
    
    return 0;
}
