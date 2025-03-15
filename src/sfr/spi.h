#ifndef __SPI_SFR_H
#define __SPI_SFR_H

#include <stdint.h>

/**
 * @brief SPI 제어 레지스터1 구조체
 */
typedef union
{
    struct
    {
        uint32_t CPHA : 1;    /*!< Clock Phase */
        uint32_t CPOL : 1;    /*!< Clock Polarity */
        uint32_t MSTR : 1;    /*!< Master Selection */
        uint32_t BR : 3;      /*!< Baud Rate Control */
        uint32_t SPE : 1;     /*!< SPI Enable */
        uint32_t LSBFIRST : 1; /*!< Frame Format */
        uint32_t SSI : 1;     /*!< Internal Slave Select */
        uint32_t SSM : 1;     /*!< Software Slave Management */
        uint32_t RXONLY : 1;  /*!< Receive Only */
        uint32_t DFF : 1;     /*!< Data Frame Format */
        uint32_t CRCNEXT : 1; /*!< CRC Transfer Next */
        uint32_t CRCEN : 1;   /*!< Hardware CRC Calculation Enable */
        uint32_t BIDIOE : 1;  /*!< Output Enable in Bidirectional Mode */
        uint32_t BIDIMODE : 1; /*!< Bidirectional Data Mode Enable */
        uint32_t RESERVED : 16;
    } b;
    uint32_t w;
} SPI_CR1_TypeDef;

/**
 * @brief SPI 제어 레지스터2 구조체
 */
typedef union
{
    struct
    {
        uint32_t RXDMAEN : 1; /*!< Rx Buffer DMA Enable */
        uint32_t TXDMAEN : 1; /*!< Tx Buffer DMA Enable */
        uint32_t SSOE : 1;    /*!< SS Output Enable */
        uint32_t RESERVED1 : 1;
        uint32_t FRF : 1;     /*!< Frame Format */
        uint32_t ERRIE : 1;   /*!< Error Interrupt Enable */
        uint32_t RXNEIE : 1;  /*!< RX Buffer Not Empty Interrupt Enable */
        uint32_t TXEIE : 1;   /*!< Tx Buffer Empty Interrupt Enable */
        uint32_t RESERVED2 : 24;
    } b;
    uint32_t w;
} SPI_CR2_TypeDef;

/**
 * @brief SPI 상태 레지스터 구조체
 */
typedef union
{
    struct
    {
        uint32_t RXNE : 1;    /*!< Receive Buffer Not Empty */
        uint32_t TXE : 1;     /*!< Transmit Buffer Empty */
        uint32_t CHSIDE : 1;  /*!< Channel Side */
        uint32_t UDR : 1;     /*!< Underrun Flag */
        uint32_t CRCERR : 1;  /*!< CRC Error Flag */
        uint32_t MODF : 1;    /*!< Mode Fault */
        uint32_t OVR : 1;     /*!< Overrun Flag */
        uint32_t BSY : 1;     /*!< Busy Flag */
        uint32_t FRE : 1;     /*!< Frame Format Error */
        uint32_t RESERVED : 23;
    } b;
    uint32_t w;
} SPI_SR_TypeDef;

/**
 * @brief SPI 레지스터 구조체
 */
typedef struct
{
    volatile SPI_CR1_TypeDef CR1;    /*!< SPI Control register 1,  Address offset: 0x00 */
    volatile SPI_CR2_TypeDef CR2;    /*!< SPI Control register 2,  Address offset: 0x04 */
    volatile SPI_SR_TypeDef SR;      /*!< SPI Status register,     Address offset: 0x08 */
    volatile uint32_t DR;            /*!< SPI Data register,       Address offset: 0x0C */
    volatile uint32_t CRCPR;         /*!< SPI CRC polynomial register, Address offset: 0x10 */
    volatile uint32_t RXCRCR;        /*!< SPI Rx CRC register,     Address offset: 0x14 */
    volatile uint32_t TXCRCR;        /*!< SPI Tx CRC register,     Address offset: 0x18 */
} SPI_TypeDef;

#endif /* __SPI_SFR_H */
