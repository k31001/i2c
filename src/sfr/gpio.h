#ifndef __GPIO_SFR_H
#define __GPIO_SFR_H

#include <stdint.h>

/**
 * @brief GPIO 모드 레지스터 구조체
 */
typedef union
{
    struct
    {
        uint32_t MODER0 : 2;   /*!< Port x configuration bits for pin 0 */
        uint32_t MODER1 : 2;   /*!< Port x configuration bits for pin 1 */
        uint32_t MODER2 : 2;   /*!< Port x configuration bits for pin 2 */
        uint32_t MODER3 : 2;   /*!< Port x configuration bits for pin 3 */
        uint32_t MODER4 : 2;   /*!< Port x configuration bits for pin 4 */
        uint32_t MODER5 : 2;   /*!< Port x configuration bits for pin 5 */
        uint32_t MODER6 : 2;   /*!< Port x configuration bits for pin 6 */
        uint32_t MODER7 : 2;   /*!< Port x configuration bits for pin 7 */
        uint32_t MODER8 : 2;   /*!< Port x configuration bits for pin 8 */
        uint32_t MODER9 : 2;   /*!< Port x configuration bits for pin 9 */
        uint32_t MODER10 : 2;  /*!< Port x configuration bits for pin 10 */
        uint32_t MODER11 : 2;  /*!< Port x configuration bits for pin 11 */
        uint32_t MODER12 : 2;  /*!< Port x configuration bits for pin 12 */
        uint32_t MODER13 : 2;  /*!< Port x configuration bits for pin 13 */
        uint32_t MODER14 : 2;  /*!< Port x configuration bits for pin 14 */
        uint32_t MODER15 : 2;  /*!< Port x configuration bits for pin 15 */
    } b;
    uint32_t w;
} GPIO_MODER_TypeDef;

/**
 * @brief GPIO 출력 타입 레지스터 구조체
 */
typedef union
{
    struct
    {
        uint32_t OT0 : 1;     /*!< Port x configuration bit for pin 0 */
        uint32_t OT1 : 1;     /*!< Port x configuration bit for pin 1 */
        uint32_t OT2 : 1;     /*!< Port x configuration bit for pin 2 */
        uint32_t OT3 : 1;     /*!< Port x configuration bit for pin 3 */
        uint32_t OT4 : 1;     /*!< Port x configuration bit for pin 4 */
        uint32_t OT5 : 1;     /*!< Port x configuration bit for pin 5 */
        uint32_t OT6 : 1;     /*!< Port x configuration bit for pin 6 */
        uint32_t OT7 : 1;     /*!< Port x configuration bit for pin 7 */
        uint32_t OT8 : 1;     /*!< Port x configuration bit for pin 8 */
        uint32_t OT9 : 1;     /*!< Port x configuration bit for pin 9 */
        uint32_t OT10 : 1;    /*!< Port x configuration bit for pin 10 */
        uint32_t OT11 : 1;    /*!< Port x configuration bit for pin 11 */
        uint32_t OT12 : 1;    /*!< Port x configuration bit for pin 12 */
        uint32_t OT13 : 1;    /*!< Port x configuration bit for pin 13 */
        uint32_t OT14 : 1;    /*!< Port x configuration bit for pin 14 */
        uint32_t OT15 : 1;    /*!< Port x configuration bit for pin 15 */
        uint32_t RESERVED : 16;
    } b;
    uint32_t w;
} GPIO_OTYPER_TypeDef;

/**
 * @brief GPIO 레지스터 구조체
 */
typedef struct
{
    volatile GPIO_MODER_TypeDef MODER;   /*!< GPIO port mode register,               Address offset: 0x00 */
    volatile GPIO_OTYPER_TypeDef OTYPER; /*!< GPIO port output type register,        Address offset: 0x04 */
    volatile uint32_t OSPEEDR;           /*!< GPIO port output speed register,       Address offset: 0x08 */
    volatile uint32_t PUPDR;             /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C */
    volatile uint32_t IDR;               /*!< GPIO port input data register,         Address offset: 0x10 */
    volatile uint32_t ODR;               /*!< GPIO port output data register,        Address offset: 0x14 */
    volatile uint32_t BSRR;              /*!< GPIO port bit set/reset register,      Address offset: 0x18 */
    volatile uint32_t LCKR;              /*!< GPIO port configuration lock register,  Address offset: 0x1C */
    volatile uint32_t AFR[2];            /*!< GPIO alternate function registers,      Address offset: 0x20-0x24 */
} GPIO_TypeDef;

#endif /* __GPIO_SFR_H */
