#include "i2c.h"

/* I2C 클럭 설정 내부 함수 */
static void I2C_ClockConfig(I2C_TypeDef *I2Cx, I2C_Config *config, uint32_t pclk1)
{
    uint32_t ccr_val;

    /* I2C 설정 초기화 */
    I2Cx->CR1.w = 0;
    I2Cx->CR2 = (pclk1 / 1000000); // FREQ 필드 설정

    /* CCR 설정 */
    if (config->ClockSpeed <= I2C_STANDARD_MODE_SPEED)
    { // 표준 모드 (100KHz)
        ccr_val = pclk1 / (config->ClockSpeed * 2);
        I2Cx->CCR.w = ccr_val;
    }
    else
    { // 고속 모드 (400KHz)
        if (config->DutyCycle == I2C_DUTYCYCLE_2)
        { // Duty Cycle 2
            ccr_val = pclk1 / (config->ClockSpeed * 3);
            I2Cx->CCR.b.CCR = ccr_val;
            I2Cx->CCR.b.FS = 1;
        }
        else
        { // Duty Cycle 16:9
            ccr_val = pclk1 / (config->ClockSpeed * 25);
            I2Cx->CCR.b.CCR = ccr_val;
            I2Cx->CCR.b.FS = 1;
            I2Cx->CCR.b.DUTY = 1;
        }
    }

    /* TRISE 설정 */
    if (config->ClockSpeed <= I2C_STANDARD_MODE_SPEED)
    {
        I2Cx->TRISE = (pclk1 / 1000000) + 1;
    }
    else
    {
        I2Cx->TRISE = ((pclk1 / 1000000) * 300 / 1000) + 1;
    }
}

/* I2C 초기화 함수 */
void I2C_Init(I2C_TypeDef *I2Cx, I2C_Config *config)
{
    uint32_t pclk1 = SYSTEM_CLOCK_DEFAULT; // PCLK1 클럭 주파수

    /* I2C 클럭 활성화 */
    if (I2Cx == I2C1)
    {
        RCC->APB1ENR.b.I2C1EN = 1;
    }
    else if (I2Cx == I2C2)
    {
        RCC->APB1ENR.b.I2C2EN = 1;
    }

    /* I2C 비활성화 */
    I2Cx->CR1.b.PE = 0;

    /* I2C 클럭 설정 */
    I2C_ClockConfig(I2Cx, config, pclk1);

    /* 자신의 주소 설정 */
    I2Cx->OAR1 = (config->OwnAddress << 1);

    /* General Call 및 Clock Stretching 설정 */
    if (config->GeneralCall)
    {
        I2Cx->CR1.b.ENGC = 1;
    }
    if (config->NoStretchMode)
    {
        I2Cx->CR1.b.NOSTRETCH = 1;
    }

    /* I2C 활성화 */
    I2Cx->CR1.b.PE = 1;
}

/* I2C 비활성화 함수 */
void I2C_DeInit(I2C_TypeDef *I2Cx)
{
    I2Cx->CR1.b.PE = 0;

    if (I2Cx == I2C1)
    {
        RCC->APB1ENR.b.I2C1EN = 0;
    }
    else if (I2Cx == I2C2)
    {
        RCC->APB1ENR.b.I2C2EN = 0;
    }
}

/* I2C 시작 조건 생성 */
I2C_Status I2C_Start(I2C_TypeDef *I2Cx)
{
    uint32_t timeout = I2C_TIMEOUT_DEFAULT;

    /* 시작 조건 생성 */
    I2Cx->CR1.b.START = 1;

    /* 시작 조건 생성 완료 대기 */
    while (!I2Cx->SR1.b.SB)
    {
        if (--timeout == 0)
        {
            return I2C_TIMEOUT;
        }
    }

    return I2C_OK;
}

/* I2C 정지 조건 생성 */
void I2C_Stop(I2C_TypeDef *I2Cx)
{
    I2Cx->CR1.b.STOP = 1;
}

/* 1바이트 데이터 쓰기 */
I2C_Status I2C_WriteByte(I2C_TypeDef *I2Cx, uint8_t data)
{
    uint32_t timeout = I2C_TIMEOUT_DEFAULT;

    /* TXE 플래그 대기 */
    while (!I2Cx->SR1.b.TXE)
    {
        if (--timeout == 0)
        {
            return I2C_TIMEOUT;
        }
    }

    /* 데이터 전송 */
    I2Cx->DR = data;

    /* BTF 플래그 대기 */
    timeout = I2C_TIMEOUT_DEFAULT;
    while (!I2Cx->SR1.b.BTF)
    {
        if (--timeout == 0)
        {
            return I2C_TIMEOUT;
        }
    }

    return I2C_OK;
}

/* 1바이트 데이터 읽기 */
I2C_Status I2C_ReadByte(I2C_TypeDef *I2Cx, uint8_t *data, uint8_t ack)
{
    uint32_t timeout = I2C_TIMEOUT_DEFAULT;

    /* ACK 설정 */
    if (ack)
    {
        I2Cx->CR1.b.ACK = 1;
    }
    else
    {
        I2Cx->CR1.b.ACK = 0;
    }

    /* RXNE 플래그 대기 */
    while (!I2Cx->SR1.b.RXNE)
    {
        if (--timeout == 0)
        {
            return I2C_TIMEOUT;
        }
    }

    /* 데이터 읽기 */
    *data = I2Cx->DR;

    return I2C_OK;
}

/* 여러 바이트 데이터 쓰기 */
I2C_Status I2C_WriteData(I2C_TypeDef *I2Cx, uint8_t slaveAddr, uint8_t *data, uint16_t len)
{
    I2C_Status status;
    uint32_t timeout = I2C_TIMEOUT_DEFAULT;

    /* 시작 조건 생성 */
    status = I2C_Start(I2Cx);
    if (status != I2C_OK)
        return status;

    /* 슬레이브 주소 전송 (쓰기) */
    I2Cx->DR = (slaveAddr << 1) & 0xFE;

    /* ADDR 플래그 대기 */
    while (!I2Cx->SR1.b.ADDR)
    {
        if (--timeout == 0)
        {
            return I2C_TIMEOUT;
        }
    }
    /* ADDR 플래그 클리어 */
    (void)I2Cx->SR2;

    /* 데이터 전송 */
    while (len--)
    {
        status = I2C_WriteByte(I2Cx, *data++);
        if (status != I2C_OK)
            return status;
    }

    /* 정지 조건 생성 */
    I2C_Stop(I2Cx);

    return I2C_OK;
}

/* 여러 바이트 데이터 읽기 */
I2C_Status I2C_ReadData(I2C_TypeDef *I2Cx, uint8_t slaveAddr, uint8_t *data, uint16_t len)
{
    I2C_Status status;
    uint32_t timeout = I2C_TIMEOUT_DEFAULT;

    /* 시작 조건 생성 */
    status = I2C_Start(I2Cx);
    if (status != I2C_OK)
        return status;

    /* 슬레이브 주소 전송 (읽기) */
    I2Cx->DR = (slaveAddr << 1) | 0x01;

    /* ADDR 플래그 대기 */
    while (!I2Cx->SR1.b.ADDR)
    {
        if (--timeout == 0)
        {
            return I2C_TIMEOUT;
        }
    }
    /* ADDR 플래그 클리어 */
    (void)I2Cx->SR2;

    /* 데이터 수신 */
    while (len)
    {
        if (len == 1)
        {                                         // 마지막 바이트
            status = I2C_ReadByte(I2Cx, data, 0); // NACK
        }
        else
        {
            status = I2C_ReadByte(I2Cx, data, 1); // ACK
        }
        if (status != I2C_OK)
            return status;
        data++;
        len--;
    }

    /* 정지 조건 생성 */
    I2C_Stop(I2Cx);

    return I2C_OK;
}