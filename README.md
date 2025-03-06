# STM32F411 I2C 드라이버

STM32F411 마이크로컨트롤러를 위한 I2C(Inter-Integrated Circuit) 통신 드라이버입니다. 이 드라이버는 표준 모드(100KHz)와 고속 모드(400KHz)를 모두 지원하며, 마스터 모드 동작을 구현합니다.

## 특징

- 표준 모드(100KHz) 및 고속 모드(400KHz) 지원
- 7비트 주소 지원
- 타임아웃 처리를 통한 안정성 확보
- Clock Stretching 지원
- General Call 지원
- 다양한 듀티 사이클 설정 (2:1 또는 16:9)

## 파일 구조

```
.
├── src/
│   ├── stm32f411xe.h  - STM32F411 레지스터 정의
│   ├── i2c.h          - I2C 드라이버 헤더
│   └── i2c.c          - I2C 드라이버 구현
└── doc/
    └── STM32F411xC-E-advanced-arm-mcu.pdf  - STM32F411 레퍼런스 매뉴얼
```

### 문서

- `doc/STM32F411xC-E-advanced-arm-mcu.pdf`: STM32F411 마이크로컨트롤러의 상세 레퍼런스 매뉴얼입니다. I2C 주변장치의 레지스터 맵과 동작 방식에 대한 자세한 설명이 포함되어 있습니다.

## 사용 방법

### 1. 초기화

```c
/* I2C 설정 구조체 초기화 */
I2C_Config config = {
    .ClockSpeed = 100000,    // 100KHz (표준 모드)
    .OwnAddress = 0x00,      // 자신의 슬레이브 주소
    .DutyCycle = 0,          // 2:1 듀티 사이클
    .GeneralCall = 0,        // General Call 비활성화
    .NoStretchMode = 0       // Clock Stretching 활성화
};

/* I2C 초기화 */
I2C_Init(I2C1, &config);
```

### 2. 데이터 송신

```c
uint8_t data[] = {0x01, 0x02, 0x03};
I2C_Status status;

/* 단일 바이트 전송 */
status = I2C_WriteByte(I2C1, 0x01);

/* 다중 바이트 전송 */
status = I2C_WriteData(I2C1, 0x50, data, 3);  // 슬레이브 주소 0x50에 3바이트 전송
```

### 3. 데이터 수신

```c
uint8_t data[3];
I2C_Status status;

/* 단일 바이트 수신 */
status = I2C_ReadByte(I2C1, &data[0], 0);  // NACK로 수신

/* 다중 바이트 수신 */
status = I2C_ReadData(I2C1, 0x50, data, 3);  // 슬레이브 주소 0x50에서 3바이트 수신
```

## 주의사항

1. 초기화 전 준비사항

   - GPIO 핀이 I2C 기능으로 설정되어 있어야 함
   - 적절한 풀업 저항이 연결되어 있어야 함
   - PCLK1(APB1) 클럭이 활성화되어 있어야 함

2. 클럭 설정

   - 현재 코드는 PCLK1이 16MHz로 설정되어 있다고 가정
   - 다른 클럭 주파수를 사용하는 경우 `I2C_Init` 함수의 `pclk1` 값을 수정해야 함

3. 버퍼 크기

   - 데이터 송수신 시 버퍼는 지정된 길이만큼의 크기를 반드시 확보해야 함
   - 버퍼 오버플로우 방지를 위해 충분한 크기 할당 필요

4. 에러 처리
   - 모든 통신 함수는 상태값을 반환하므로 반드시 확인 필요
   - 타임아웃 발생 시 적절한 복구 처리 구현 필요

## 반환값

모든 통신 함수는 다음 상태값 중 하나를 반환합니다:

- `I2C_OK`: 정상 동작 완료
- `I2C_ERROR`: 일반적인 오류 발생
- `I2C_BUSY`: I2C 버스가 사용 중
- `I2C_TIMEOUT`: 타임아웃 발생

## 제한사항

1. 현재 구현된 기능

   - 마스터 모드 송수신
   - 7비트 주소 지원
   - 표준 모드 및 고속 모드

2. 구현되지 않은 기능
   - 슬레이브 모드 동작
   - 10비트 주소 지원
   - DMA 전송
   - 인터럽트 기반 비동기 전송

## 라이선스

이 코드는 자유롭게 사용, 수정 및 배포가 가능합니다.
