#ifndef __RCC_SFR_H
#define __RCC_SFR_H

#include <stdint.h>

/**
 * @brief RCC 레지스터 구조체
 */
typedef struct
{
    volatile uint32_t CR;            /*!< RCC clock control register,                  Address offset: 0x00 */
    volatile uint32_t PLLCFGR;       /*!< RCC PLL configuration register,             Address offset: 0x04 */
    volatile uint32_t CFGR;          /*!< RCC clock configuration register,           Address offset: 0x08 */
    volatile uint32_t CIR;           /*!< RCC clock interrupt register,               Address offset: 0x0C */
    volatile uint32_t AHB1RSTR;      /*!< RCC AHB1 peripheral reset register,         Address offset: 0x10 */
    volatile uint32_t AHB2RSTR;      /*!< RCC AHB2 peripheral reset register,         Address offset: 0x14 */
    volatile uint32_t RESERVED0[2];  /*!< Reserved, 0x18-0x1C                                            */
    volatile uint32_t APB1RSTR;      /*!< RCC APB1 peripheral reset register,         Address offset: 0x20 */
    volatile uint32_t APB2RSTR;      /*!< RCC APB2 peripheral reset register,         Address offset: 0x24 */
    volatile uint32_t RESERVED1[2];  /*!< Reserved, 0x28-0x2C                                            */
    volatile uint32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock enable register,  Address offset: 0x30 */
    volatile uint32_t AHB2ENR;       /*!< RCC AHB2 peripheral clock enable register,  Address offset: 0x34 */
    volatile uint32_t RESERVED2[2];  /*!< Reserved, 0x38-0x3C                                            */
    volatile uint32_t APB1ENR;       /*!< RCC APB1 peripheral clock enable register,  Address offset: 0x40 */
    volatile uint32_t APB2ENR;       /*!< RCC APB2 peripheral clock enable register,  Address offset: 0x44 */
} RCC_TypeDef;

#endif /* __RCC_SFR_H */
