#include "../i2c.h"
#include "../gpio.h"
#include <stdio.h>

/*
 * Large-transfer stress test configuration
 *
 * NOTE: 0x50 is commonly used by EEPROM-like devices.
 * Many EEPROMs do NOT support 1MB continuous read/write without addressing/page handling.
 * For a meaningful 1MB stress test, use a slave that supports bulk streaming (e.g., FRAM/RAM,
 * a test fixture device, or a driver stub).
 */
#ifndef I2C_STRESS_SLAVE_ADDR
#define I2C_STRESS_SLAVE_ADDR 0x50
#endif

#ifndef I2C_STRESS_TOTAL_SIZE
#define I2C_STRESS_TOTAL_SIZE (1024u * 1024u) /* 1MB */
#endif

#ifndef I2C_STRESS_CHUNK_SIZE
#define I2C_STRESS_CHUNK_SIZE 4096u /* 4KB */
#endif

/* Optional delay between chunks (busy-wait loop). Useful for EEPROM write-cycle, etc. */
#ifndef I2C_STRESS_DELAY_CYCLES
#define I2C_STRESS_DELAY_CYCLES 0u
#endif

/* Progress print step in percent (0 to disable). */
#ifndef I2C_STRESS_PROGRESS_STEP_PCT
#define I2C_STRESS_PROGRESS_STEP_PCT 25u
#endif

static void I2C_StressDelay(void) {
#if (I2C_STRESS_DELAY_CYCLES > 0)
    volatile uint32_t n = (uint32_t)I2C_STRESS_DELAY_CYCLES;
    while (n--) {
        __asm__ volatile ("nop");
    }
#endif
}

static uint32_t I2C_Checksum32(const uint8_t* data, uint32_t len) {
    /* Lightweight checksum (not cryptographic). */
    uint32_t sum = 0;
    for (uint32_t i = 0; i < len; ++i) {
        sum += data[i];
    }
    return sum;
}

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

/**
 * @brief  1MB(약 1,048,576바이트) 대용량 I2C 송수신 스트레스 테스트
 *
 * 전체 버퍼를 청크 단위로 나누어 쓰고 읽으면서 드라이버의 연속 전송 안정성을 검증합니다.
 * 슬레이브 디바이스의 특성에 따라 데이터가 보존되지 않을 수 있으므로,
 * 기본 동작에서는 상태 코드(I2C_Status)만 검증합니다.
 *
 * @note
 *  - I2C_WriteData / I2C_ReadData의 길이 인자가 uint16_t 이므로 청크 단위로 쪼개 호출합니다.
 *  - 대용량 버퍼는 스택 오버플로우 방지를 위해 static 영역에 할당합니다.
 */
static void Test_I2C_1MB_Transfer(I2C_TypeDef* I2Cx) {
    printf("\n=== 1MB 대용량 송수신 테스트 ===\n");

    static uint8_t tx_buffer[I2C_STRESS_TOTAL_SIZE];
    static uint8_t rx_buffer[I2C_STRESS_TOTAL_SIZE];

    const uint8_t slave_addr = (uint8_t)I2C_STRESS_SLAVE_ADDR;

    /* 송신 버퍼 패턴 채우기 */
    for (uint32_t i = 0; i < (uint32_t)I2C_STRESS_TOTAL_SIZE; ++i) {
        tx_buffer[i] = (uint8_t)(i & 0xFF);
    }

    I2C_Status status = I2C_OK;
    uint32_t offset = 0;

    const uint32_t total = (uint32_t)I2C_STRESS_TOTAL_SIZE;
#if (I2C_STRESS_PROGRESS_STEP_PCT > 0)
    uint32_t next_progress = (uint32_t)I2C_STRESS_PROGRESS_STEP_PCT;
#endif

    /* 1MB 전체를 청크 단위로 쓰기 */
    while (offset < total && status == I2C_OK) {
        uint32_t remaining = total - offset;
        uint16_t chunk_len = (remaining > (uint32_t)I2C_STRESS_CHUNK_SIZE) ?
                             (uint16_t)I2C_STRESS_CHUNK_SIZE :
                             (uint16_t)remaining;

        status = I2C_WriteData(I2Cx, slave_addr, &tx_buffer[offset], chunk_len);
        if (status != I2C_OK) {
            printf("1MB 쓰기 중 오류 발생 (offset=%lu, len=%u, status=%d)\n",
                   (unsigned long)offset, chunk_len, status);
            break;
        }

        offset += (uint32_t)chunk_len;
        I2C_StressDelay();

#if (I2C_STRESS_PROGRESS_STEP_PCT > 0)
        uint32_t pct = (offset * 100u) / total;
        if (pct >= next_progress) {
            printf("[WRITE] progress: %lu%%\n", (unsigned long)next_progress);
            next_progress += (uint32_t)I2C_STRESS_PROGRESS_STEP_PCT;
        }
#endif
    }

    if (status != I2C_OK) {
        PrintTestResult("1MB 대용량 쓰기", status);
        return;
    }

#if (I2C_STRESS_PROGRESS_STEP_PCT > 0)
    next_progress = (uint32_t)I2C_STRESS_PROGRESS_STEP_PCT;
#endif

    /* 읽기 전에 오프셋 초기화 */
    offset = 0;

    /* 1MB 전체를 청크 단위로 읽기 */
    while (offset < total && status == I2C_OK) {
        uint32_t remaining = total - offset;
        uint16_t chunk_len = (remaining > (uint32_t)I2C_STRESS_CHUNK_SIZE) ?
                             (uint16_t)I2C_STRESS_CHUNK_SIZE :
                             (uint16_t)remaining;

        status = I2C_ReadData(I2Cx, slave_addr, &rx_buffer[offset], chunk_len);
        if (status != I2C_OK) {
            printf("1MB 읽기 중 오류 발생 (offset=%lu, len=%u, status=%d)\n",
                   (unsigned long)offset, chunk_len, status);
            break;
        }

        offset += (uint32_t)chunk_len;
        I2C_StressDelay();

#if (I2C_STRESS_PROGRESS_STEP_PCT > 0)
        uint32_t pct = (offset * 100u) / total;
        if (pct >= next_progress) {
            printf("[READ ] progress: %lu%%\n", (unsigned long)next_progress);
            next_progress += (uint32_t)I2C_STRESS_PROGRESS_STEP_PCT;
        }
#endif
    }

    PrintTestResult("1MB 대용량 송수신", status);

#ifdef VERIFY_I2C_1MB_CHECKSUM
    if (status == I2C_OK) {
        uint32_t tx_sum = I2C_Checksum32(tx_buffer, total);
        uint32_t rx_sum = I2C_Checksum32(rx_buffer, total);
        if (tx_sum == rx_sum) {
            printf("1MB 체크섬 검증: 성공 (sum=0x%08lX)\n", (unsigned long)tx_sum);
        } else {
            printf("1MB 체크섬 검증: 실패 (tx=0x%08lX, rx=0x%08lX)\n",
                   (unsigned long)tx_sum, (unsigned long)rx_sum);
        }
    }
#endif

#ifdef VERIFY_I2C_1MB_DATA
    /* memcmp requires string.h; keep it local/optional for constrained libc environments. */
    #include <string.h>
    if (status == I2C_OK) {
        if (memcmp(tx_buffer, rx_buffer, total) == 0) {
            printf("1MB 데이터 검증: 성공 (TX/RX 일치)\n");
        } else {
            printf("1MB 데이터 검증: 실패 (TX/RX 불일치)\n");
        }
    }
#endif
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
    Test_I2C_1MB_Transfer(I2C1);

    // 정리
    I2C_DeInit(I2C1);
    printf("\n===== I2C 드라이버 테스트 완료 =====\n");
}
