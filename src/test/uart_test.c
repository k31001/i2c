#include "uart.h"
#include "gpio.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief 테스트 결과를 출력하는 헬퍼 함수
 */
static void PrintTestResult(const char* test_name, UART_Status status) {
    if (status == UART_OK) {
        printf("%s: 성공\n", test_name);
    } else {
        printf("%s: 실패 (상태: %d)\n", test_name, status);
    }
}

/**
 * @brief UART 송신 테스트를 수행하는 헬퍼 함수
 */
static void TestUARTTransmit(UART_TypeDef* UARTx, const char* test_name, const uint8_t* data, uint16_t size) {
    UART_Status status = UART_Transmit(UARTx, data, size);
    PrintTestResult(test_name, status);
}

/**
 * @brief UART 수신 테스트를 수행하는 헬퍼 함수
 */
static void TestUARTReceive(UART_TypeDef* UARTx, const char* test_name, uint8_t* buffer, uint16_t size) {
    UART_Status status = UART_Receive(UARTx, buffer, size);
    PrintTestResult(test_name, status);
    if (status == UART_OK) {
        printf("수신 데이터: ");
        for (int i = 0; i < size; i++) {
            printf("0x%02X ", buffer[i]);
        }
        printf("\n");
    }
}

/**
 * @brief UART 기본 통신 테스트
 */
static void Test_UART_Basic_Functions(UART_TypeDef* UARTx) {
    printf("\n=== UART 기본 통신 테스트 ===\n");
    
    // 단일 바이트 송수신 테스트
    printf("\n단일 바이트 송수신 테스트...\n");
    uint8_t tx_byte = 0xA5;
    uint8_t rx_byte;
    
    TestUARTTransmit(UARTx, "단일 바이트 송신", &tx_byte, 1);
    TestUARTReceive(UARTx, "단일 바이트 수신", &rx_byte, 1);
    
    // 문자열 송수신 테스트
    printf("\n문자열 송수신 테스트...\n");
    const char* test_str = "Hello UART!";
    char rx_buffer[32];
    
    TestUARTTransmit(UARTx, "문자열 송신", (uint8_t*)test_str, strlen(test_str));
    TestUARTReceive(UARTx, "문자열 수신", (uint8_t*)rx_buffer, strlen(test_str));
}

/**
 * @brief UART 통신 속도 테스트
 */
static void Test_UART_Speed_Functions(UART_TypeDef* UARTx) {
    printf("\n=== UART 통신 속도 테스트 ===\n");
    
    UART_Config config = {
        .BaudRate = UART_BAUDRATE_9600,
        .WordLength = UART_WORDLENGTH_8B,
        .StopBits = UART_STOPBITS_1,
        .Parity = UART_PARITY_NONE,
        .Mode = UART_MODE_TX_RX,
        .HwFlowCtl = UART_HWCONTROL_NONE,
        .OverSampling = UART_OVERSAMPLING_16
    };
    
    // 다양한 통신 속도 테스트
    const uint32_t baudrates[] = {
        UART_BAUDRATE_9600,
        UART_BAUDRATE_19200,
        UART_BAUDRATE_38400,
        UART_BAUDRATE_115200
    };
    
    const char* test_data = "UART Speed Test";
    uint8_t rx_buffer[32];
    
    for (int i = 0; i < sizeof(baudrates)/sizeof(baudrates[0]); i++) {
        config.BaudRate = baudrates[i];
        UART_Init(UARTx, &config);
        
        printf("\n%d bps 테스트...\n", baudrates[i]);
        TestUARTTransmit(UARTx, "데이터 송신", (uint8_t*)test_data, strlen(test_data));
        TestUARTReceive(UARTx, "데이터 수신", rx_buffer, strlen(test_data));
    }
}

/**
 * @brief UART 에러 처리 테스트
 */
static void Test_UART_Error_Functions(UART_TypeDef* UARTx) {
    printf("\n=== UART 에러 처리 테스트 ===\n");
    
    // 패리티 에러 테스트
    printf("\n패리티 에러 테스트...\n");
    UART_Config config = {
        .BaudRate = UART_BAUDRATE_9600,
        .WordLength = UART_WORDLENGTH_9B,
        .StopBits = UART_STOPBITS_1,
        .Parity = UART_PARITY_EVEN,
        .Mode = UART_MODE_TX_RX,
        .HwFlowCtl = UART_HWCONTROL_NONE,
        .OverSampling = UART_OVERSAMPLING_16
    };
    UART_Init(UARTx, &config);
    
    uint8_t test_byte = 0xA5;
    uint8_t rx_byte;
    TestUARTTransmit(UARTx, "패리티 에러 테스트 송신", &test_byte, 1);
    TestUARTReceive(UARTx, "패리티 에러 테스트 수신", &rx_byte, 1);
    
    // 프레이밍 에러 테스트
    printf("\n프레이밍 에러 테스트...\n");
    config.StopBits = UART_STOPBITS_2;
    UART_Init(UARTx, &config);
    
    TestUARTTransmit(UARTx, "프레이밍 에러 테스트 송신", &test_byte, 1);
    TestUARTReceive(UARTx, "프레이밍 에러 테스트 수신", &rx_byte, 1);
}

void UART_Test(void) {
    printf("===== UART 드라이버 테스트 시작 =====\n");
    
    // GPIO 설정
    GPIO_Config gpio_config = {
        .Mode = GPIO_MODE_ALT,
        .Otype = GPIO_OTYPE_PUSHPULL,
        .Speed = GPIO_SPEED_HIGH,
        .PuPd = GPIO_PUPD_PULLUP,
        .AF = GPIO_AF7
    };
    
    // TX 핀 설정 (PA9)
    gpio_config.Pin = (1 << 9);
    GPIO_Init(GPIOA, &gpio_config);
    
    // RX 핀 설정 (PA10)
    gpio_config.Pin = (1 << 10);
    GPIO_Init(GPIOA, &gpio_config);
    
    printf("UART1 GPIO 핀 설정 완료 (PA9=TX, PA10=RX)\n");
    
    // 기본 UART 설정으로 초기화
    UART_Config uart_config = {
        .BaudRate = UART_BAUDRATE_115200,
        .WordLength = UART_WORDLENGTH_8B,
        .StopBits = UART_STOPBITS_1,
        .Parity = UART_PARITY_NONE,
        .Mode = UART_MODE_TX_RX,
        .HwFlowCtl = UART_HWCONTROL_NONE,
        .OverSampling = UART_OVERSAMPLING_16
    };
    UART_Init(UART1, &uart_config);
    
    // 테스트 실행
    Test_UART_Basic_Functions(UART1);
    Test_UART_Speed_Functions(UART1);
    Test_UART_Error_Functions(UART1);
    
    // 정리
    UART_DeInit(UART1);
    printf("\n===== UART 드라이버 테스트 완료 =====\n");
}
