#include "../spi.h"
#include "../gpio.h"
#include <stdio.h>

/**
 * @brief 테스트 결과를 출력하는 헬퍼 함수
 */
static void PrintTestResult(const char* test_name, SPI_Status status) {
    if (status == SPI_OK) {
        printf("%s: 성공\n", test_name);
    } else {
        printf("%s: 실패 (상태: %d)\n", test_name, status);
    }
}

/**
 * @brief SPI 데이터 송수신 테스트를 수행하는 헬퍼 함수
 */
static void TestSPITransfer(SPI_TypeDef* SPIx, const char* test_name, uint16_t data) {
    uint16_t rx_data;
    SPI_Status status = SPI_TransferData(SPIx, data, &rx_data);
    PrintTestResult(test_name, status);
    if (status == SPI_OK) {
        printf("송신: 0x%04X, 수신: 0x%04X\n", data, rx_data);
    }
}

/**
 * @brief SPI 통신 모드 테스트
 */
static void Test_SPI_Mode_Functions(SPI_TypeDef* SPIx) {
    printf("\n=== SPI 통신 모드 테스트 ===\n");
    
    SPI_Config config = {
        .Mode = SPI_MODE_MASTER,
        .Direction = SPI_DIRECTION_2LINES,
        .DataSize = SPI_DATASIZE_8BIT,
        .CPOL = SPI_CPOL_LOW,
        .CPHA = SPI_CPHA_1EDGE,
        .NSS = SPI_NSS_SOFT,
        .BaudRate = SPI_BAUDRATE_DIV8,
        .FirstBit = SPI_FIRSTBIT_MSB
    };
    
    // 모드 0 테스트 (CPOL=0, CPHA=0)
    printf("\n모드 0 테스트 (CPOL=0, CPHA=0)...\n");
    config.CPOL = SPI_CPOL_LOW;
    config.CPHA = SPI_CPHA_1EDGE;
    SPI_Init(SPIx, &config);
    TestSPITransfer(SPIx, "모드 0", 0xA5);
    
    // 모드 1 테스트 (CPOL=0, CPHA=1)
    printf("\n모드 1 테스트 (CPOL=0, CPHA=1)...\n");
    config.CPHA = SPI_CPHA_2EDGE;
    SPI_Init(SPIx, &config);
    TestSPITransfer(SPIx, "모드 1", 0x5A);
    
    // 모드 2 테스트 (CPOL=1, CPHA=0)
    printf("\n모드 2 테스트 (CPOL=1, CPHA=0)...\n");
    config.CPOL = SPI_CPOL_HIGH;
    config.CPHA = SPI_CPHA_1EDGE;
    SPI_Init(SPIx, &config);
    TestSPITransfer(SPIx, "모드 2", 0x33);
    
    // 모드 3 테스트 (CPOL=1, CPHA=1)
    printf("\n모드 3 테스트 (CPOL=1, CPHA=1)...\n");
    config.CPHA = SPI_CPHA_2EDGE;
    SPI_Init(SPIx, &config);
    TestSPITransfer(SPIx, "모드 3", 0xCC);
}

/**
 * @brief SPI 데이터 크기 및 전송 테스트
 */
static void Test_SPI_Data_Functions(SPI_TypeDef* SPIx) {
    printf("\n=== SPI 데이터 전송 테스트 ===\n");
    
    SPI_Config config = {
        .Mode = SPI_MODE_MASTER,
        .Direction = SPI_DIRECTION_2LINES,
        .CPOL = SPI_CPOL_LOW,
        .CPHA = SPI_CPHA_1EDGE,
        .NSS = SPI_NSS_SOFT,
        .BaudRate = SPI_BAUDRATE_DIV8,
        .FirstBit = SPI_FIRSTBIT_MSB
    };
    
    // 8비트 데이터 전송 테스트
    printf("\n8비트 데이터 전송 테스트...\n");
    config.DataSize = SPI_DATASIZE_8BIT;
    SPI_Init(SPIx, &config);
    TestSPITransfer(SPIx, "8비트 전송", 0xA5);
    
    // 16비트 데이터 전송 테스트
    printf("\n16비트 데이터 전송 테스트...\n");
    config.DataSize = SPI_DATASIZE_16BIT;
    SPI_Init(SPIx, &config);
    TestSPITransfer(SPIx, "16비트 전송", 0xA55A);
    
    // LSB/MSB 테스트
    printf("\nLSB/MSB 테스트...\n");
    config.FirstBit = SPI_FIRSTBIT_LSB;
    SPI_Init(SPIx, &config);
    TestSPITransfer(SPIx, "LSB 우선", 0x5AA5);
}

/**
 * @brief SPI 에러 처리 테스트
 */
static void Test_SPI_Error_Functions(SPI_TypeDef* SPIx) {
    printf("\n=== SPI 에러 처리 테스트 ===\n");
    
    // 통신 중 에러 테스트
    printf("\n통신 타임아웃 테스트...\n");
    uint16_t dummy_rx;
    SPI_Status status = SPI_TransferData(SPIx, 0xFF, &dummy_rx);
    PrintTestResult("타임아웃", status);
    
    // 오버런 에러 테스트
    printf("\n오버런 에러 테스트...\n");
    for (int i = 0; i < 3; i++) {
        status = SPI_TransferData(SPIx, 0xFF, &dummy_rx);
        if (status == SPI_ERROR_OVERRUN) {
            printf("시도 %d: 오버런 에러 발생\n", i + 1);
        }
    }
}

void SPI_Test(void) {
    printf("===== SPI 드라이버 테스트 시작 =====\n");
    
    // GPIO 설정
    GPIO_Config gpio_config = {
        .Mode = GPIO_MODE_ALT,
        .Otype = GPIO_OTYPE_PUSHPULL,
        .Speed = GPIO_SPEED_HIGH,
        .PuPd = GPIO_PUPD_NOPULL,
        .AF = GPIO_AF5
    };
    
    // SCK 설정 (PA5)
    gpio_config.Pin = (1 << 5);
    GPIO_Init(GPIOA, &gpio_config);
    
    // MISO 설정 (PA6)
    gpio_config.Pin = (1 << 6);
    GPIO_Init(GPIOA, &gpio_config);
    
    // MOSI 설정 (PA7)
    gpio_config.Pin = (1 << 7);
    GPIO_Init(GPIOA, &gpio_config);
    
    printf("SPI1 GPIO 핀 설정 완료 (PA5=SCK, PA6=MISO, PA7=MOSI)\n");
    
    // 기본 SPI 설정으로 초기화
    SPI_Config spi_config = {
        .Mode = SPI_MODE_MASTER,
        .Direction = SPI_DIRECTION_2LINES,
        .DataSize = SPI_DATASIZE_8BIT,
        .CPOL = SPI_CPOL_LOW,
        .CPHA = SPI_CPHA_1EDGE,
        .NSS = SPI_NSS_SOFT,
        .BaudRate = SPI_BAUDRATE_DIV8,
        .FirstBit = SPI_FIRSTBIT_MSB
    };
    SPI_Init(SPI1, &spi_config);
    
    // 테스트 실행
    Test_SPI_Mode_Functions(SPI1);
    Test_SPI_Data_Functions(SPI1);
    Test_SPI_Error_Functions(SPI1);
    
    // 정리
    SPI_DeInit(SPI1);
    printf("\n===== SPI 드라이버 테스트 완료 =====\n");
}
