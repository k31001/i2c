#include "../i2c.h"
#include "../gpio.h"
#include <stdio.h>

/**
 * @brief 테스트 결과를 출력하는 헬퍼 함수
 */
static void PrintTestResult(const char* test_name, I2C_Status status) {
    if (status == I2C_OK) {
        printf("%s: 성공\n", test_name);
    } else {
        printf("%s: 실패 (상태: %d)\n", test_name, status);
    }
}

/**
 * @brief I2C 송수신 테스트를 수행하는 헬퍼 함수
 */
static void TestI2CTransfer(I2C_TypeDef* I2Cx, uint8_t slave_addr, const char* test_name) {
    uint8_t tx_data[] = {0x01, 0x02, 0x03};
    uint8_t rx_data[3];
    I2C_Status status = I2C_WriteReadData(I2Cx, slave_addr, tx_data, 3, rx_data, 3);
    PrintTestResult(test_name, status);
}

/**
 * @brief I2C 초기화 및 속도 설정 테스트
 */
static void Test_I2C_Speed_Functions(I2C_TypeDef* I2Cx) {
    printf("\n=== 통신 속도 테스트 ===\n");
    
    uint8_t slave_addr = 0x50;
    I2C_Config config = {
        .OwnAddress = 0x42,
        .GeneralCall = 0,
        .NoStretchMode = 0
    };
    
    // 표준 모드 테스트 (100KHz)
    config.ClockSpeed = 100000;
    config.DutyCycle = 0;
    I2C_Init(I2Cx, &config);
    TestI2CTransfer(I2Cx, slave_addr, "표준 모드 (100KHz)");
    
    // 고속 모드 테스트 (400KHz, 듀티 2:1)
    config.ClockSpeed = 400000;
    I2C_Init(I2Cx, &config);
    TestI2CTransfer(I2Cx, slave_addr, "고속 모드 (400KHz, 듀티 2:1)");
    
    // 고속 모드 테스트 (400KHz, 듀티 16:9)
    config.DutyCycle = 1;
    I2C_Init(I2Cx, &config);
    TestI2CTransfer(I2Cx, slave_addr, "고속 모드 (400KHz, 듀티 16:9)");
}

/**
 * @brief I2C 데이터 송수신 테스트
 */
static void Test_I2C_Data_Functions(I2C_TypeDef* I2Cx) {
    printf("\n=== 데이터 송수신 테스트 ===\n");
    
    uint8_t slave_addr = 0x50;
    I2C_Status status;
    
    // 단일 바이트 송수신 테스트
    printf("단일 바이트 송수신 테스트...\n");
    uint8_t tx_byte = 0xAA;
    uint8_t rx_byte;
    
    status = I2C_Start(I2Cx);
    if (status == I2C_OK) {
        status = I2C_WriteByte(I2Cx, (slave_addr << 1) | 0);
        if (status == I2C_OK) {
            status = I2C_WriteByte(I2Cx, tx_byte);
            PrintTestResult("단일 바이트 쓰기", status);
        }
        I2C_Stop(I2Cx);
    }
    
    status = I2C_Start(I2Cx);
    if (status == I2C_OK) {
        status = I2C_WriteByte(I2Cx, (slave_addr << 1) | 1);
        if (status == I2C_OK) {
            status = I2C_ReadByte(I2Cx, &rx_byte, 0);
            PrintTestResult("단일 바이트 읽기", status);
        }
        I2C_Stop(I2Cx);
    }
    
    // 다중 바이트 송수신 테스트
    printf("\n다중 바이트 송수신 테스트...\n");
    uint8_t tx_data[] = {0x11, 0x22, 0x33, 0x44};
    uint8_t rx_data[4];
    
    status = I2C_WriteData(I2Cx, slave_addr, tx_data, sizeof(tx_data));
    PrintTestResult("다중 바이트 쓰기", status);
    
    status = I2C_ReadData(I2Cx, slave_addr, rx_data, sizeof(rx_data));
    PrintTestResult("다중 바이트 읽기", status);
}

/**
 * @brief I2C 에러 처리 테스트
 */
static void Test_I2C_Error_Functions(I2C_TypeDef* I2Cx) {
    printf("\n=== 에러 처리 테스트 ===\n");
    
    // 존재하지 않는 슬레이브 주소 테스트
    uint8_t invalid_addr = 0x7F;
    uint8_t dummy_data = 0x00;
    I2C_Status status = I2C_WriteData(I2Cx, invalid_addr, &dummy_data, 1);
    PrintTestResult("존재하지 않는 슬레이브 주소", status);
    
    // 연속 통신 시도 테스트
    printf("\n연속 통신 시도 테스트...\n");
    for (int i = 0; i < 3; i++) {
        status = I2C_Start(I2Cx);
        if (status == I2C_BUSY) {
            printf("시도 %d: 버스 사용중\n", i + 1);
        }
        I2C_Stop(I2Cx);
    }
}

void I2C_Test(void) {
    printf("===== I2C 드라이버 테스트 시작 =====\n");
    
    // GPIO 설정
    GPIO_Config gpio_config = {
        .Mode = GPIO_MODE_ALT,
        .Otype = GPIO_OTYPE_OPENDRAIN,
        .Speed = GPIO_SPEED_HIGH,
        .PuPd = GPIO_PUPD_PULLUP,
        .AF = GPIO_AF4
    };
    
    // SCL 설정
    gpio_config.Pin = (1 << 6);  // PB6
    GPIO_Init(GPIOB, &gpio_config);
    
    // SDA 설정
    gpio_config.Pin = (1 << 7);  // PB7
    GPIO_Init(GPIOB, &gpio_config);
    
    printf("I2C1 GPIO 핀 설정 완료 (PB6=SCL, PB7=SDA)\n");
    
    // 기본 I2C 설정으로 초기화
    I2C_Config i2c_config = {
        .ClockSpeed = 100000,    // 100KHz
        .OwnAddress = 0x42,
        .DutyCycle = 0,
        .GeneralCall = 0,
        .NoStretchMode = 0
    };
    I2C_Init(I2C1, &i2c_config);
    
    // 테스트 실행
    Test_I2C_Speed_Functions(I2C1);
    Test_I2C_Data_Functions(I2C1);
    Test_I2C_Error_Functions(I2C1);
    
    // 정리
    I2C_DeInit(I2C1);
    printf("\n===== I2C 드라이버 테스트 완료 =====\n");
}
