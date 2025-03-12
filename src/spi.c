#include "spi.h"

/* SPI 초기화 함수 */
void SPI_Init(SPI_TypeDef *SPIx, SPI_Config *config)
{
    uint32_t pclk = 16000000; // PCLK 클럭 주파수 (기본값 16MHz)
    uint32_t br_div;

    /* SPI 클럭 활성화 */
    if (SPIx == SPI1)
    {
        RCC->APB2ENR.b.SPI1EN = 1;
    }
    else if (SPIx == SPI2)
    {
        RCC->APB1ENR.b.SPI2EN = 1;
    }
    else if (SPIx == SPI3)
    {
        RCC->APB1ENR.b.SPI3EN = 1;
    }

    /* SPI 비활성화 */
    SPIx->CR1.b.SPE = 0;

    /* SPI 설정 초기화 */
    SPIx->CR1.w = 0;
    SPIx->CR2.w = 0;

    /* 마스터 모드 설정 */
    SPIx->CR1.b.MSTR = 1;

    /* 클럭 분주비 설정 */
    br_div = pclk / config->ClockSpeed;
    if (br_div >= 256)
        SPIx->CR1.b.BR = 7; // PCLK/256
    else if (br_div >= 128)
        SPIx->CR1.b.BR = 6; // PCLK/128
    else if (br_div >= 64)
        SPIx->CR1.b.BR = 5; // PCLK/64
    else if (br_div >= 32)
        SPIx->CR1.b.BR = 4; // PCLK/32
    else if (br_div >= 16)
        SPIx->CR1.b.BR = 3; // PCLK/16
    else if (br_div >= 8)
        SPIx->CR1.b.BR = 2; // PCLK/8
    else if (br_div >= 4)
        SPIx->CR1.b.BR = 1; // PCLK/4
    else
        SPIx->CR1.b.BR = 0; // PCLK/2

    /* SPI 모드 설정 */
    switch (config->Mode)
    {
    case SPI_MODE_0:
        SPIx->CR1.b.CPOL = 0;
        SPIx->CR1.b.CPHA = 0;
        break;
    case SPI_MODE_1:
        SPIx->CR1.b.CPOL = 0;
        SPIx->CR1.b.CPHA = 1;
        break;
    case SPI_MODE_2:
        SPIx->CR1.b.CPOL = 1;
        SPIx->CR1.b.CPHA = 0;
        break;
    case SPI_MODE_3:
        SPIx->CR1.b.CPOL = 1;
        SPIx->CR1.b.CPHA = 1;
        break;
    }

    /* 데이터 크기 설정 */
    SPIx->CR1.b.DFF = config->DataSize;

    /* 첫 비트 전송 순서 설정 */
    SPIx->CR1.b.LSBFIRST = config->FirstBit;

    /* NSS 핀 관리 방식 설정 */
    if (config->NSS)
    {
        SPIx->CR1.b.SSM = 1; // 소프트웨어 NSS 관리
        SPIx->CR1.b.SSI = 1; // 내부 슬레이브 선택 신호를 High로 설정
    }
    else
    {
        SPIx->CR2.b.SSOE = 1; // 하드웨어 NSS 출력 활성화
    }

    /* SPI 활성화 */
    SPIx->CR1.b.SPE = 1;
}

/* SPI 비활성화 함수 */
void SPI_DeInit(SPI_TypeDef *SPIx)
{
    /* SPI 비활성화 */
    SPIx->CR1.b.SPE = 0;

    /* SPI 클럭 비활성화 */
    if (SPIx == SPI1)
    {
        RCC->APB2ENR.b.SPI1EN = 0;
    }
    else if (SPIx == SPI2)
    {
        RCC->APB1ENR.b.SPI2EN = 0;
    }
    else if (SPIx == SPI3)
    {
        RCC->APB1ENR.b.SPI3EN = 0;
    }
}

/* 1바이트 데이터 쓰기 */
SPI_Status SPI_WriteByte(SPI_TypeDef *SPIx, uint8_t data)
{
    uint32_t timeout = 10000;

    /* TXE 플래그 대기 */
    while (!SPIx->SR.b.TXE)
    {
        if (--timeout == 0)
        {
            return SPI_TIMEOUT;
        }
    }

    /* 데이터 전송 */
    SPIx->DR = data;

    /* BSY 플래그 대기 */
    timeout = 10000;
    while (SPIx->SR.b.BSY)
    {
        if (--timeout == 0)
        {
            return SPI_TIMEOUT;
        }
    }

    return SPI_OK;
}

/* 1바이트 데이터 읽기 */
SPI_Status SPI_ReadByte(SPI_TypeDef *SPIx, uint8_t *data)
{
    uint32_t timeout = 10000;

    /* 더미 데이터 전송으로 클럭 생성 */
    SPIx->DR = 0xFF;

    /* RXNE 플래그 대기 */
    while (!SPIx->SR.b.RXNE)
    {
        if (--timeout == 0)
        {
            return SPI_TIMEOUT;
        }
    }

    /* 데이터 읽기 */
    *data = SPIx->DR;

    return SPI_OK;
}

/* 여러 바이트 데이터 쓰기 */
SPI_Status SPI_WriteData(SPI_TypeDef *SPIx, uint8_t *data, uint16_t len)
{
    SPI_Status status;

    while (len--)
    {
        status = SPI_WriteByte(SPIx, *data++);
        if (status != SPI_OK)
            return status;
    }

    return SPI_OK;
}

/* 여러 바이트 데이터 읽기 */
SPI_Status SPI_ReadData(SPI_TypeDef *SPIx, uint8_t *data, uint16_t len)
{
    SPI_Status status;

    while (len--)
    {
        status = SPI_ReadByte(SPIx, data++);
        if (status != SPI_OK)
            return status;
    }

    return SPI_OK;
}

/* 데이터 동시 송수신 */
SPI_Status SPI_TransferData(SPI_TypeDef *SPIx, uint8_t *txData, uint8_t *rxData, uint16_t len)
{
    uint32_t timeout;

    while (len--)
    {
        /* TXE 플래그 대기 */
        timeout = 10000;
        while (!SPIx->SR.b.TXE)
        {
            if (--timeout == 0)
            {
                return SPI_TIMEOUT;
            }
        }

        /* 데이터 전송 */
        SPIx->DR = *txData++;

        /* RXNE 플래그 대기 */
        timeout = 10000;
        while (!SPIx->SR.b.RXNE)
        {
            if (--timeout == 0)
            {
                return SPI_TIMEOUT;
            }
        }

        /* 데이터 읽기 */
        *rxData++ = SPIx->DR;
    }

    /* BSY 플래그 대기 */
    timeout = 10000;
    while (SPIx->SR.b.BSY)
    {
        if (--timeout == 0)
        {
            return SPI_TIMEOUT;
        }
    }

    return SPI_OK;
}

/* NSS 핀 제어 */
void SPI_SetNSS(SPI_TypeDef *SPIx, uint8_t state)
{
    if (SPIx->CR1.b.SSM)
    {
        SPIx->CR1.b.SSI = state;
    }
}