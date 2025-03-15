#ifndef __STM32F411xE_H
#define __STM32F411xE_H

#include <stdint.h>
#include "sfr/gpio.h"
#include "sfr/i2c.h"
#include "sfr/rcc.h"
#include "sfr/spi.h"
#include "sfr/usart.h"

/* 메모리 맵 기본 주소 */
#define FLASH_BASE          ((uint32_t)0x08000000UL) /*!< FLASH base address */
#define SRAM_BASE          ((uint32_t)0x20000000UL) /*!< SRAM base address */
#define PERIPH_BASE        ((uint32_t)0x40000000UL) /*!< Peripheral base address */

/* 버스 도메인 기본 주소 */
#define APB1PERIPH_BASE     (PERIPH_BASE)
#define APB2PERIPH_BASE     (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE     (PERIPH_BASE + 0x00020000UL)

/* APB1 주변장치 */
#define I2C1_BASE          (APB1PERIPH_BASE + 0x5400UL)
#define I2C2_BASE          (APB1PERIPH_BASE + 0x5800UL)
#define SPI2_BASE          (APB1PERIPH_BASE + 0x3800UL)
#define SPI3_BASE          (APB1PERIPH_BASE + 0x3C00UL)
#define USART2_BASE        (APB1PERIPH_BASE + 0x4400UL)

/* APB2 주변장치 */
#define SPI1_BASE          (APB2PERIPH_BASE + 0x3000UL)
#define USART1_BASE        (APB2PERIPH_BASE + 0x1000UL)
#define USART6_BASE        (APB2PERIPH_BASE + 0x1400UL)

/* AHB1 주변장치 */
#define GPIOA_BASE         (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE         (AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE         (AHB1PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE         (AHB1PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE         (AHB1PERIPH_BASE + 0x1000UL)
#define GPIOH_BASE         (AHB1PERIPH_BASE + 0x1C00UL)
#define RCC_BASE           (AHB1PERIPH_BASE + 0x3800UL)

/* 시스템 클럭 설정 */
#define SYSTEM_CLOCK_DEFAULT 16000000UL /* 기본 시스템 클럭 (16MHz) */

/* 주변장치 인스턴스 정의 */
#define GPIOA             ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB             ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC             ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD             ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE             ((GPIO_TypeDef *)GPIOE_BASE)
#define GPIOH             ((GPIO_TypeDef *)GPIOH_BASE)
#define SPI1              ((SPI_TypeDef *)SPI1_BASE)
#define SPI2              ((SPI_TypeDef *)SPI2_BASE)
#define SPI3              ((SPI_TypeDef *)SPI3_BASE)
#define I2C1              ((I2C_TypeDef *)I2C1_BASE)
#define I2C2              ((I2C_TypeDef *)I2C2_BASE)
#define RCC               ((RCC_TypeDef *)RCC_BASE)
#define USART1            ((USART_TypeDef *)USART1_BASE)
#define USART2            ((USART_TypeDef *)USART2_BASE)
#define USART6            ((USART_TypeDef *)USART6_BASE)

#endif /* __STM32F411xE_H */