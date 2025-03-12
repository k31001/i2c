# STM32F411 드라이버

STM32F411 마이크로컨트롤러를 위한 통신 드라이버입니다. 이 프로젝트는 I2C와 SPI 통신을 모두 지원합니다.

## I2C 드라이버

I2C(Inter-Integrated Circuit) 통신 드라이버는 표준 모드(100KHz)와 고속 모드(400KHz)를 모두 지원하며, 마스터 모드 동작을 구현합니다.

### I2C 특징

- 표준 모드(100KHz) 및 고속 모드(400KHz) 지원
- 7비트 주소 지원
- 타임아웃 처리를 통한 안정성 확보
- Clock Stretching 지원
- General Call 지원
- 다양한 듀티 사이클 설정 (2:1 또는 16:9)

## SPI 드라이버

SPI(Serial Peripheral Interface) 통신 드라이버는 마스터 모드 동작을 구현하며, 모든 SPI 모드(0-3)와 다양한 데이터 크기를 지원합니다.

### SPI 특징

- 마스터 모드 동작
- 모든 SPI 모드(0-3) 지원
- 8비트 및 16비트 데이터 크기 지원
- MSB first 및 LSB first 전송 순서 지원
- 하드웨어 및 소프트웨어 NSS 관리 지원
- 타임아웃 처리를 통한 안정성 확보
- 최대 PCLK/2 속도 지원

## 파일 구조

```
.
├── src/
│   ├── stm32f411xe.h  - STM32F411 레지스터 정의
│   ├── i2c.h          - I2C 드라이버 헤더
│   ├── i2c.c          - I2C 드라이버 구현
│   ├── spi.h          - SPI 드라이버 헤더
│   └── spi.c          - SPI 드라이버 구현
└── doc/
    └── STM32F411xC-E-advanced-arm-mcu.pdf  - STM32F411 레퍼런스 매뉴얼
```

## 사용 방법

### I2C 사용 예시

```c
/* I2C 설정 구조체 초기화 */
I2C_Config i2c_config = {
    .ClockSpeed = 100000,    // 100KHz (표준 모드)
    .OwnAddress = 0x00,      // 자신의 슬레이브 주소
    .DutyCycle = 0,          // 2:1 듀티 사이클
    .GeneralCall = 0,        // General Call 비활성화
    .NoStretchMode = 0       // Clock Stretching 활성화
};

/* I2C 초기화 */
I2C_Init(I2C1, &i2c_config);

/* 데이터 송신 */
uint8_t data[] = {0x01, 0x02, 0x03};
I2C_WriteData(I2C1, 0x50, data, 3);  // 슬레이브 주소 0x50에 3바이트 전송
```

### SPI 사용 예시

```c
/* SPI 설정 구조체 초기화 */
SPI_Config spi_config = {
    .ClockSpeed = 1000000,    // 1MHz
    .Mode = SPI_MODE_0,       // CPOL = 0, CPHA = 0
    .DataSize = 0,            // 8비트 데이터
    .FirstBit = 0,            // MSB first
    .NSS = 1                  // 소프트웨어 NSS 관리
};

/* SPI 초기화 */
SPI_Init(SPI1, &spi_config);

/* 데이터 송신 */
uint8_t tx_data[] = {0x01, 0x02, 0x03};
uint8_t rx_data[3];

/* 데이터 송수신 */
SPI_TransferData(SPI1, tx_data, rx_data, 3);
```

## 주의사항

### I2C 사용 시 주의사항

1. 초기화 전 준비사항

   - GPIO 핀이 I2C 기능으로 설정되어 있어야 함
   - 적절한 풀업 저항이 연결되어 있어야 함
   - PCLK1(APB1) 클럭이 활성화되어 있어야 함

2. 클럭 설정
   - 현재 코드는 PCLK1이 16MHz로 설정되어 있다고 가정
   - 다른 클럭 주파수를 사용하는 경우 `I2C_Init` 함수의 `pclk1` 값을 수정해야 함

### SPI 사용 시 주의사항

1. 초기화 전 준비사항

   - GPIO 핀이 SPI 기능으로 설정되어 있어야 함
   - PCLK1(APB1) 또는 PCLK2(APB2) 클럭이 활성화되어 있어야 함
   - SPI1은 APB2, SPI2/3은 APB1에 연결되어 있음

2. 클럭 설정

   - 현재 코드는 PCLK가 16MHz로 설정되어 있다고 가정
   - 다른 클럭 주파수를 사용하는 경우 `SPI_Init` 함수의 `pclk` 값을 수정해야 함

3. NSS 핀 관리
   - 소프트웨어 NSS 모드에서는 SPI_SetNSS() 함수로 제어
   - 하드웨어 NSS 모드에서는 자동으로 관리됨

## 제한사항

### I2C 제한사항

1. 현재 구현된 기능

   - 마스터 모드 송수신
   - 7비트 주소 지원
   - 표준 모드 및 고속 모드

2. 구현되지 않은 기능
   - 슬레이브 모드 동작
   - 10비트 주소 지원
   - DMA 전송
   - 인터럽트 기반 비동기 전송

### SPI 제한사항

1. 현재 구현된 기능

   - 마스터 모드 송수신
   - 모든 SPI 모드 지원
   - 8비트 및 16비트 데이터 크기
   - 하드웨어/소프트웨어 NSS 관리

2. 구현되지 않은 기능
   - 슬레이브 모드 동작
   - DMA 전송
   - 인터럽트 기반 비동기 전송
   - CRC 기능

## 라이선스

이 코드는 자유롭게 사용, 수정 및 배포가 가능합니다.
