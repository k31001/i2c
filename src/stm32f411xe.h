#ifndef __STM32F411xE_H
#define __STM32F411xE_H

#include <stdint.h>

/* 메모리 맵 기본 주소 */
#define FLASH_BASE 0x08000000UL  /* FLASH base address */
#define SRAM_BASE 0x20000000UL   /* SRAM base address */
#define PERIPH_BASE 0x40000000UL /* Peripheral base address */

/* APB1 주변장치 */
#define APB1PERIPH_BASE (PERIPH_BASE)
#define I2C1_BASE (APB1PERIPH_BASE + 0x5400UL)
#define I2C2_BASE (APB1PERIPH_BASE + 0x5800UL)
#define SPI2_BASE (APB1PERIPH_BASE + 0x3800UL)
#define SPI3_BASE (APB1PERIPH_BASE + 0x3C00UL)

/* APB2 주변장치 */
#define APB2PERIPH_BASE (PERIPH_BASE + 0x00010000UL)
#define SPI1_BASE (APB2PERIPH_BASE + 0x3000UL)

/* AHB1 주변장치 */
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000UL)
#define RCC_BASE (AHB1PERIPH_BASE + 0x3800UL)

/* 시스템 클럭 설정 */
#define SYSTEM_CLOCK_DEFAULT 16000000UL /* 기본 시스템 클럭 (16MHz) */

/* 타임아웃 설정 */
#define I2C_TIMEOUT_DEFAULT 10000UL /* I2C 기본 타임아웃 값 */
#define SPI_TIMEOUT_DEFAULT 10000UL /* SPI 기본 타임아웃 값 */

/* I2C 속도 설정 */
#define I2C_STANDARD_MODE_SPEED 100000UL /* 표준 모드 속도 (100KHz) */
#define I2C_FAST_MODE_SPEED 400000UL     /* 고속 모드 속도 (400KHz) */

/* I2C 듀티 사이클 설정 */
#define I2C_DUTYCYCLE_2 0    /* 2:1 듀티 사이클 */
#define I2C_DUTYCYCLE_16_9 1 /* 16:9 듀티 사이클 */

/* I2C 주소 설정 */
#define I2C_ADDRESSING_7BIT 0  /* 7비트 주소 모드 */
#define I2C_ADDRESSING_10BIT 1 /* 10비트 주소 모드 */

/* I2C CR1 레지스터 구조체 */
typedef union
{
    struct
    {
        uint32_t PE : 1;    /* Peripheral Enable */
        uint32_t SMBUS : 1; /* SMBus Mode */
        uint32_t RESERVED1 : 1;
        uint32_t SMBTYPE : 1;   /* SMBus Type */
        uint32_t ENARP : 1;     /* ARP Enable */
        uint32_t ENPEC : 1;     /* PEC Enable */
        uint32_t ENGC : 1;      /* General Call Enable */
        uint32_t NOSTRETCH : 1; /* Clock Stretching Disable */
        uint32_t START : 1;     /* Start Generation */
        uint32_t STOP : 1;      /* Stop Generation */
        uint32_t ACK : 1;       /* Acknowledge Enable */
        uint32_t POS : 1;       /* Acknowledge/PEC Position */
        uint32_t PEC : 1;       /* Packet Error Checking */
        uint32_t ALERT : 1;     /* SMBus Alert */
        uint32_t RESERVED2 : 1;
        uint32_t SWRST : 1; /* Software Reset */
        uint32_t RESERVED3 : 16;
    } b;
    uint32_t w;
} I2C_CR1_TypeDef;

/* I2C SR1 레지스터 구조체 */
typedef union
{
    struct
    {
        uint32_t SB : 1;    /* Start Bit */
        uint32_t ADDR : 1;  /* Address sent/matched */
        uint32_t BTF : 1;   /* Byte Transfer Finished */
        uint32_t ADD10 : 1; /* 10-bit header sent */
        uint32_t STOPF : 1; /* Stop detection */
        uint32_t RESERVED1 : 1;
        uint32_t RXNE : 1;   /* Data Register Not Empty */
        uint32_t TXE : 1;    /* Data Register Empty */
        uint32_t BERR : 1;   /* Bus Error */
        uint32_t ARLO : 1;   /* Arbitration Lost */
        uint32_t AF : 1;     /* Acknowledge Failure */
        uint32_t OVR : 1;    /* Overrun/Underrun */
        uint32_t PECERR : 1; /* PEC Error in reception */
        uint32_t RESERVED2 : 1;
        uint32_t TIMEOUT : 1;  /* Timeout or Tlow Error */
        uint32_t SMBALERT : 1; /* SMBus Alert */
        uint32_t RESERVED3 : 16;
    } b;
    uint32_t w;
} I2C_SR1_TypeDef;

/* I2C CCR 레지스터 구조체 */
typedef union
{
    struct
    {
        uint32_t CCR : 12; /* Clock Control Register */
        uint32_t RESERVED : 2;
        uint32_t DUTY : 1; /* Fast Mode Duty Cycle */
        uint32_t FS : 1;   /* I2C Master Mode Selection */
        uint32_t RESERVED2 : 16;
    } b;
    uint32_t w;
} I2C_CCR_TypeDef;

/* I2C 레지스터 구조체 */
typedef struct
{
    volatile I2C_CR1_TypeDef CR1; /* Control register 1,        Address offset: 0x00 */
    volatile uint32_t CR2;        /* Control register 2,        Address offset: 0x04 */
    volatile uint32_t OAR1;       /* Own address register 1,    Address offset: 0x08 */
    volatile uint32_t OAR2;       /* Own address register 2,    Address offset: 0x0C */
    volatile uint32_t DR;         /* Data register,             Address offset: 0x10 */
    volatile I2C_SR1_TypeDef SR1; /* Status register 1,         Address offset: 0x14 */
    volatile uint32_t SR2;        /* Status register 2,         Address offset: 0x18 */
    volatile I2C_CCR_TypeDef CCR; /* Clock control register,    Address offset: 0x1C */
    volatile uint32_t TRISE;      /* TRISE register,           Address offset: 0x20 */
} I2C_TypeDef;

/* SPI CR1 레지스터 구조체 */
typedef union
{
    struct
    {
        uint32_t CPHA : 1;     /* Clock Phase */
        uint32_t CPOL : 1;     /* Clock Polarity */
        uint32_t MSTR : 1;     /* Master Selection */
        uint32_t BR : 3;       /* Baud Rate Control */
        uint32_t SPE : 1;      /* SPI Enable */
        uint32_t LSBFIRST : 1; /* Frame Format */
        uint32_t SSI : 1;      /* Internal Slave Select */
        uint32_t SSM : 1;      /* Software Slave Management */
        uint32_t RXONLY : 1;   /* Receive Only */
        uint32_t DFF : 1;      /* Data Frame Format */
        uint32_t CRCNEXT : 1;  /* CRC Transfer Next */
        uint32_t CRCEN : 1;    /* Hardware CRC Calculation Enable */
        uint32_t BIDIOE : 1;   /* Output Enable in Bidirectional Mode */
        uint32_t BIDIMODE : 1; /* Bidirectional Data Mode Enable */
        uint32_t RESERVED : 16;
    } b;
    uint32_t w;
} SPI_CR1_TypeDef;

/* SPI CR2 레지스터 구조체 */
typedef union
{
    struct
    {
        uint32_t RXDMAEN : 1; /* Rx Buffer DMA Enable */
        uint32_t TXDMAEN : 1; /* Tx Buffer DMA Enable */
        uint32_t SSOE : 1;    /* SS Output Enable */
        uint32_t RESERVED1 : 1;
        uint32_t FRF : 1;    /* Frame Format */
        uint32_t ERRIE : 1;  /* Error Interrupt Enable */
        uint32_t RXNEIE : 1; /* RX Buffer Not Empty Interrupt Enable */
        uint32_t TXEIE : 1;  /* Tx Buffer Empty Interrupt Enable */
        uint32_t RESERVED2 : 24;
    } b;
    uint32_t w;
} SPI_CR2_TypeDef;

/* SPI SR 레지스터 구조체 */
typedef union
{
    struct
    {
        uint32_t RXNE : 1;   /* Receive Buffer Not Empty */
        uint32_t TXE : 1;    /* Transmit Buffer Empty */
        uint32_t CHSIDE : 1; /* Channel Side */
        uint32_t UDR : 1;    /* Underrun Flag */
        uint32_t CRCERR : 1; /* CRC Error Flag */
        uint32_t MODF : 1;   /* Mode Fault */
        uint32_t OVR : 1;    /* Overrun Flag */
        uint32_t BSY : 1;    /* Busy Flag */
        uint32_t FRE : 1;    /* Frame Format Error */
        uint32_t RESERVED : 23;
    } b;
    uint32_t w;
} SPI_SR_TypeDef;

/* SPI 레지스터 구조체 */
typedef struct
{
    volatile SPI_CR1_TypeDef CR1; /* Control register 1,      Address offset: 0x00 */
    volatile SPI_CR2_TypeDef CR2; /* Control register 2,      Address offset: 0x04 */
    volatile SPI_SR_TypeDef SR;   /* Status register,         Address offset: 0x08 */
    volatile uint32_t DR;         /* Data register,           Address offset: 0x0C */
    volatile uint32_t CRCPR;      /* CRC polynomial register, Address offset: 0x10 */
    volatile uint32_t RXCRCR;     /* RX CRC register,        Address offset: 0x14 */
    volatile uint32_t TXCRCR;     /* TX CRC register,        Address offset: 0x18 */
    volatile uint32_t I2SCFGR;    /* I2S configuration register, Address offset: 0x1C */
    volatile uint32_t I2SPR;      /* I2S prescaler register,    Address offset: 0x20 */
} SPI_TypeDef;

/* RCC APB1ENR 레지스터 구조체 */
typedef union
{
    struct
    {
        uint32_t TIM2EN : 1; /* Timer 2 clock enable */
        uint32_t TIM3EN : 1; /* Timer 3 clock enable */
        uint32_t TIM4EN : 1; /* Timer 4 clock enable */
        uint32_t TIM5EN : 1; /* Timer 5 clock enable */
        uint32_t RESERVED1 : 7;
        uint32_t WWDGEN : 1; /* Window watchdog clock enable */
        uint32_t RESERVED2 : 2;
        uint32_t SPI2EN : 1; /* SPI2 clock enable */
        uint32_t SPI3EN : 1; /* SPI3 clock enable */
        uint32_t RESERVED3 : 1;
        uint32_t USART2EN : 1; /* USART2 clock enable */
        uint32_t RESERVED4 : 3;
        uint32_t I2C1EN : 1; /* I2C1 clock enable */
        uint32_t I2C2EN : 1; /* I2C2 clock enable */
        uint32_t I2C3EN : 1; /* I2C3 clock enable */
        uint32_t RESERVED5 : 9;
    } b;
    uint32_t w;
} RCC_APB1ENR_TypeDef;

/* RCC APB2ENR 레지스터 구조체 */
typedef union
{
    struct
    {
        uint32_t TIM1EN : 1; /* Timer 1 clock enable */
        uint32_t RESERVED1 : 3;
        uint32_t USART1EN : 1; /* USART1 clock enable */
        uint32_t USART6EN : 1; /* USART6 clock enable */
        uint32_t RESERVED2 : 2;
        uint32_t ADC1EN : 1; /* ADC1 clock enable */
        uint32_t RESERVED3 : 2;
        uint32_t SPI1EN : 1;   /* SPI1 clock enable */
        uint32_t SPI4EN : 1;   /* SPI4 clock enable */
        uint32_t SYSCFGEN : 1; /* System configuration controller clock enable */
        uint32_t RESERVED4 : 1;
        uint32_t TIM9EN : 1;  /* Timer 9 clock enable */
        uint32_t TIM10EN : 1; /* Timer 10 clock enable */
        uint32_t TIM11EN : 1; /* Timer 11 clock enable */
        uint32_t RESERVED5 : 13;
    } b;
    uint32_t w;
} RCC_APB2ENR_TypeDef;

/* RCC 레지스터 구조체 */
typedef struct
{
    volatile uint32_t CR;                 /* Clock control register,                    Address offset: 0x00 */
    volatile uint32_t PLLCFGR;            /* PLL configuration register,               Address offset: 0x04 */
    volatile uint32_t CFGR;               /* Clock configuration register,             Address offset: 0x08 */
    volatile uint32_t CIR;                /* Clock interrupt register,                 Address offset: 0x0C */
    volatile uint32_t AHB1RSTR;           /* AHB1 peripheral reset register,          Address offset: 0x10 */
    volatile uint32_t AHB2RSTR;           /* AHB2 peripheral reset register,          Address offset: 0x14 */
    volatile uint32_t RESERVED0[2];       /* Reserved,                                Address offset: 0x18-0x1C */
    volatile uint32_t APB1RSTR;           /* APB1 peripheral reset register,          Address offset: 0x20 */
    volatile uint32_t APB2RSTR;           /* APB2 peripheral reset register,          Address offset: 0x24 */
    volatile uint32_t RESERVED1[2];       /* Reserved,                                Address offset: 0x28-0x2C */
    volatile uint32_t AHB1ENR;            /* AHB1 peripheral clock enable register,   Address offset: 0x30 */
    volatile uint32_t AHB2ENR;            /* AHB2 peripheral clock enable register,   Address offset: 0x34 */
    volatile uint32_t RESERVED2[2];       /* Reserved,                                Address offset: 0x38-0x3C */
    volatile RCC_APB1ENR_TypeDef APB1ENR; /* APB1 peripheral clock enable register,   Address offset: 0x40 */
    volatile uint32_t APB2ENR;            /* APB2 peripheral clock enable register,   Address offset: 0x44 */
} RCC_TypeDef;

/* 주변장치 인스턴스 정의 */
#define SPI1 ((SPI_TypeDef *)SPI1_BASE)
#define SPI2 ((SPI_TypeDef *)SPI2_BASE)
#define SPI3 ((SPI_TypeDef *)SPI3_BASE)
#define I2C1 ((I2C_TypeDef *)I2C1_BASE)
#define I2C2 ((I2C_TypeDef *)I2C2_BASE)
#define RCC ((RCC_TypeDef *)RCC_BASE)

#endif /* __STM32F411xE_H */