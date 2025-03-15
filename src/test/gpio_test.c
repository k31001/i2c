#include "../gpio.h"
#include "../rcc.h"
#include <stdio.h>

/**
 * @brief GPIO 모든 기능 테스트
 * 
 * 이 테스트는 다음을 검증합니다:
 * 1. GPIO 초기화 (다양한 모드로)
 * 2. 디지털 출력 및 입력
 * 3. 토글
 * 4. 포트 단위 읽기/쓰기
 * 5. 대체 기능 설정
 */
void GPIO_Test(void)
{
    printf("===== GPIO 드라이버 테스트 시작 =====\n");
    
    // GPIO 클럭 활성화
    RCC_EnableAHB1Peripheral(RCC_AHB1_GPIOA | RCC_AHB1_GPIOB);
    
    // 1. 출력 모드 테스트 (GPIOA Pin 0 - LED 가정)
    GPIO_Config led_config = {
        .Pin = (1 << 0),            // PA0
        .Mode = GPIO_MODE_OUTPUT,   // 출력 모드
        .Otype = GPIO_OTYPE_PUSHPULL, // 푸시풀
        .Speed = GPIO_SPEED_HIGH,   // 고속
        .PuPd = GPIO_PUPD_NONE      // 풀업/풀다운 없음
    };
    GPIO_Init(GPIOA, &led_config);
    
    // 디지털 출력 테스트
    printf("LED ON 설정\n");
    GPIO_WritePin(GPIOA, (1 << 0), 1);
    
    printf("LED OFF 설정\n");
    GPIO_WritePin(GPIOA, (1 << 0), 0);
    
    // 토글 테스트
    printf("LED 토글 3회 실행\n");
    for (int i = 0; i < 3; i++) {
        GPIO_TogglePin(GPIOA, (1 << 0));
        // 실제로는 지연 함수가 필요함
    }
    
    // 2. 입력 모드 테스트 (GPIOB Pin 1 - 버튼 가정)
    GPIO_Config button_config = {
        .Pin = (1 << 1),           // PB1
        .Mode = GPIO_MODE_INPUT,   // 입력 모드
        .PuPd = GPIO_PUPD_PULLUP   // 풀업 (버튼의 일반적인 연결 방식)
    };
    GPIO_Init(GPIOB, &button_config);
    
    // 입력 읽기 테스트
    uint8_t button_state = GPIO_ReadPin(GPIOB, (1 << 1));
    printf("버튼 상태 읽기: %s\n", button_state ? "눌리지 않음" : "눌림");
    
    // 3. 포트 단위 조작 테스트
    // GPIOA의 핀 4-7을 출력으로 설정
    GPIO_Config port_config = {
        .Pin = (0xF << 4),          // PA4-7
        .Mode = GPIO_MODE_OUTPUT,   // 출력 모드
        .Otype = GPIO_OTYPE_PUSHPULL,
        .Speed = GPIO_SPEED_MEDIUM,
        .PuPd = GPIO_PUPD_NONE
    };
    GPIO_Init(GPIOA, &port_config);
    
    // 포트 쓰기 테스트
    printf("GPIOA 포트의 여러 핀에 값 쓰기\n");
    // 현재 포트 값 읽기
    uint16_t port_value = GPIO_ReadPort(GPIOA);
    // PA4-7에 0xA (1010b) 쓰기
    port_value = (port_value & ~(0xF << 4)) | (0xA << 4);
    GPIO_WritePort(GPIOA, port_value);
    
    // 포트 읽기 테스트
    printf("GPIOA 포트 전체 읽기: 0x%04X\n", GPIO_ReadPort(GPIOA));
    
    // 4. 대체 기능 모드 테스트 (GPIOA Pin 9,10 - USART1)
    GPIO_Config usart_tx_config = {
        .Pin = (1 << 9),           // PA9 (USART1 TX)
        .Mode = GPIO_MODE_ALT,     // 대체 기능 모드
        .Otype = GPIO_OTYPE_PUSHPULL,
        .Speed = GPIO_SPEED_HIGH,
        .PuPd = GPIO_PUPD_PULLUP,
        .AF = GPIO_AF7            // USART1/2 AF는 AF7
    };
    GPIO_Init(GPIOA, &usart_tx_config);
    
    GPIO_Config usart_rx_config = {
        .Pin = (1 << 10),          // PA10 (USART1 RX)
        .Mode = GPIO_MODE_ALT,     // 대체 기능 모드
        .Otype = GPIO_OTYPE_PUSHPULL,
        .Speed = GPIO_SPEED_HIGH,
        .PuPd = GPIO_PUPD_PULLUP,
        .AF = GPIO_AF7            // USART1/2 AF는 AF7
    };
    GPIO_Init(GPIOA, &usart_rx_config);
    printf("USART1용 핀 PA9, PA10을 대체 기능 모드로 설정 완료\n");
    
    // 5. 아날로그 모드 테스트 (GPIOA Pin 0 - ADC)
    GPIO_Config adc_config = {
        .Pin = (1 << 0),          // PA0 (ADC 채널 0)
        .Mode = GPIO_MODE_ANALOG, // 아날로그 모드
        .PuPd = GPIO_PUPD_NONE    // 아날로그 모드에서는 풀업/풀다운이 없어야 함
    };
    GPIO_Init(GPIOA, &adc_config);
    printf("ADC용 PA0 핀을 아날로그 모드로 설정 완료\n");
    
    // 6. GPIO 초기화 해제 테스트
    GPIO_DeInit(GPIOA, (1 << 0));
    printf("PA0 핀 초기화 해제 완료\n");
    
    printf("===== GPIO 드라이버 테스트 완료 =====\n\n");
}
