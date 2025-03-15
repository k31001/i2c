#ifndef __USART_SFR_H
#define __USART_SFR_H

#include <stdint.h>

/**
 * @brief USART 상태 레지스터 구조체
 */
typedef union
{
    struct
    {
        uint32_t PE : 1;      /*!< Parity Error */
        uint32_t FE : 1;      /*!< Framing Error */
        uint32_t NE : 1;      /*!< Noise Error Flag */
        uint32_t ORE : 1;     /*!< OverRun Error */
        uint32_t IDLE : 1;    /*!< IDLE line detected */
        uint32_t RXNE : 1;    /*!< Read Data Register Not Empty */
        uint32_t TC : 1;      /*!< Transmission Complete */
        uint32_t TXE : 1;     /*!< Transmit Data Register Empty */
        uint32_t LBD : 1;     /*!< LIN Break Detection Flag */
        uint32_t CTS : 1;     /*!< CTS Flag */
        uint32_t RESERVED : 22;
    } b;
    uint32_t w;
} USART_SR_TypeDef;

/**
 * @brief USART 제어 레지스터1 구조체
 */
typedef union
{
    struct
    {
        uint32_t SBK : 1;     /*!< Send Break */
        uint32_t RWU : 1;     /*!< Receiver wakeup */
        uint32_t RE : 1;      /*!< Receiver Enable */
        uint32_t TE : 1;      /*!< Transmitter Enable */
        uint32_t IDLEIE : 1;  /*!< IDLE Interrupt Enable */
        uint32_t RXNEIE : 1;  /*!< RXNE Interrupt Enable */
        uint32_t TCIE : 1;    /*!< Transmission Complete Interrupt Enable */
        uint32_t TXEIE : 1;   /*!< TXE Interrupt Enable */
        uint32_t PEIE : 1;    /*!< PE Interrupt Enable */
        uint32_t PS : 1;      /*!< Parity Selection */
        uint32_t PCE : 1;     /*!< Parity Control Enable */
        uint32_t WAKE : 1;    /*!< Wakeup method */
        uint32_t M : 1;       /*!< Word length */
        uint32_t UE : 1;      /*!< USART Enable */
        uint32_t RESERVED : 18;
    } b;
    uint32_t w;
} USART_CR1_TypeDef;

/**
 * @brief USART 제어 레지스터2 구조체
 */
typedef union
{
    struct
    {
        uint32_t ADD : 4;     /*!< Address of the USART node */
        uint32_t RESERVED1 : 1;
        uint32_t LBDL : 1;    /*!< LIN Break Detection Length */
        uint32_t LBDIE : 1;   /*!< LIN Break Detection Interrupt Enable */
        uint32_t RESERVED2 : 1;
        uint32_t LBCL : 1;    /*!< Last Bit Clock pulse */
        uint32_t CPHA : 1;    /*!< Clock Phase */
        uint32_t CPOL : 1;    /*!< Clock Polarity */
        uint32_t CLKEN : 1;   /*!< Clock Enable */
        uint32_t STOP : 2;    /*!< STOP bits */
        uint32_t LINEN : 1;   /*!< LIN mode enable */
        uint32_t RESERVED3 : 17;
    } b;
    uint32_t w;
} USART_CR2_TypeDef;

/**
 * @brief USART 제어 레지스터3 구조체
 */
typedef union
{
    struct
    {
        uint32_t EIE : 1;     /*!< Error Interrupt Enable */
        uint32_t IREN : 1;    /*!< IrDA mode Enable */
        uint32_t IRLP : 1;    /*!< IrDA Low-Power */
        uint32_t HDSEL : 1;   /*!< Half-Duplex Selection */
        uint32_t NACK : 1;    /*!< Smartcard NACK enable */
        uint32_t SCEN : 1;    /*!< Smartcard mode enable */
        uint32_t DMAR : 1;    /*!< DMA Enable Receiver */
        uint32_t DMAT : 1;    /*!< DMA Enable Transmitter */
        uint32_t RTSE : 1;    /*!< RTS Enable */
        uint32_t CTSE : 1;    /*!< CTS Enable */
        uint32_t CTSIE : 1;   /*!< CTS Interrupt Enable */
        uint32_t ONEBIT : 1;  /*!< One sample bit method enable */
        uint32_t RESERVED : 20;
    } b;
    uint32_t w;
} USART_CR3_TypeDef;

/**
 * @brief USART 레지스터 구조체
 */
typedef struct
{
    volatile USART_SR_TypeDef SR;    /*!< Status register,        Address offset: 0x00 */
    volatile uint32_t DR;            /*!< Data register,          Address offset: 0x04 */
    volatile uint32_t BRR;           /*!< Baud rate register,     Address offset: 0x08 */
    volatile USART_CR1_TypeDef CR1;  /*!< Control register 1,     Address offset: 0x0C */
    volatile USART_CR2_TypeDef CR2;  /*!< Control register 2,     Address offset: 0x10 */
    volatile USART_CR3_TypeDef CR3;  /*!< Control register 3,     Address offset: 0x14 */
    volatile uint32_t GTPR;          /*!< Guard time and prescaler register, Address offset: 0x18 */
} USART_TypeDef;

#endif /* __USART_SFR_H */
