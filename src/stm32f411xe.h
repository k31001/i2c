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

/* AHB1 주변장치 */
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000UL)
#define RCC_BASE (AHB1PERIPH_BASE + 0x3800UL)

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
#define I2C1 ((I2C_TypeDef *)I2C1_BASE)
#define I2C2 ((I2C_TypeDef *)I2C2_BASE)
#define RCC ((RCC_TypeDef *)RCC_BASE)

#endif /* __STM32F411xE_H */