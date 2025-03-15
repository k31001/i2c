#ifndef __I2C_SFR_H
#define __I2C_SFR_H

#include <stdint.h>

/**
 * @brief I2C 제어 레지스터1 구조체
 */
typedef union
{
    struct
    {
        uint32_t PE : 1;      /*!< Peripheral Enable */
        uint32_t SMBUS : 1;   /*!< SMBus Mode */
        uint32_t RESERVED1 : 1;
        uint32_t SMBTYPE : 1; /*!< SMBus Type */
        uint32_t ENARP : 1;   /*!< ARP Enable */
        uint32_t ENPEC : 1;   /*!< PEC Enable */
        uint32_t ENGC : 1;    /*!< General Call Enable */
        uint32_t NOSTRETCH : 1; /*!< Clock Stretching Disable */
        uint32_t START : 1;   /*!< Start Generation */
        uint32_t STOP : 1;    /*!< Stop Generation */
        uint32_t ACK : 1;     /*!< Acknowledge Enable */
        uint32_t POS : 1;     /*!< Acknowledge/PEC Position */
        uint32_t PEC : 1;     /*!< Packet Error Checking */
        uint32_t ALERT : 1;   /*!< SMBus Alert */
        uint32_t RESERVED2 : 1;
        uint32_t SWRST : 1;   /*!< Software Reset */
        uint32_t RESERVED3 : 16;
    } b;
    uint32_t w;
} I2C_CR1_TypeDef;

/**
 * @brief I2C 제어 레지스터2 구조체
 */
typedef union
{
    struct
    {
        uint32_t FREQ : 6;    /*!< Peripheral Clock Frequency */
        uint32_t RESERVED1 : 2;
        uint32_t ITERREN : 1; /*!< Error Interrupt Enable */
        uint32_t ITEVTEN : 1; /*!< Event Interrupt Enable */
        uint32_t ITBUFEN : 1; /*!< Buffer Interrupt Enable */
        uint32_t DMAEN : 1;   /*!< DMA Requests Enable */
        uint32_t LAST : 1;    /*!< DMA Last Transfer */
        uint32_t RESERVED2 : 19;
    } b;
    uint32_t w;
} I2C_CR2_TypeDef;

/**
 * @brief I2C 상태 레지스터1 구조체
 */
typedef union
{
    struct
    {
        uint32_t SB : 1;      /*!< Start Bit */
        uint32_t ADDR : 1;    /*!< Address sent/matched */
        uint32_t BTF : 1;     /*!< Byte Transfer Finished */
        uint32_t ADD10 : 1;   /*!< 10-bit header sent */
        uint32_t STOPF : 1;   /*!< Stop detection */
        uint32_t RESERVED1 : 1;
        uint32_t RxNE : 1;    /*!< Data Register not Empty */
        uint32_t TxE : 1;     /*!< Data Register Empty */
        uint32_t BERR : 1;    /*!< Bus Error */
        uint32_t ARLO : 1;    /*!< Arbitration Lost */
        uint32_t AF : 1;      /*!< Acknowledge Failure */
        uint32_t OVR : 1;     /*!< Overrun/Underrun */
        uint32_t PECERR : 1;  /*!< PEC Error in reception */
        uint32_t RESERVED2 : 1;
        uint32_t TIMEOUT : 1; /*!< Timeout or Tlow Error */
        uint32_t SMBALERT : 1; /*!< SMBus Alert */
        uint32_t RESERVED3 : 16;
    } b;
    uint32_t w;
} I2C_SR1_TypeDef;

/**
 * @brief I2C 상태 레지스터2 구조체
 */
typedef union
{
    struct
    {
        uint32_t MSL : 1;     /*!< Master/Slave */
        uint32_t BUSY : 1;    /*!< Bus Busy */
        uint32_t TRA : 1;     /*!< Transmitter/Receiver */
        uint32_t RESERVED1 : 1;
        uint32_t GENCALL : 1; /*!< General Call Address */
        uint32_t SMBDEFAULT : 1; /*!< SMBus Device Default Address */
        uint32_t SMBHOST : 1; /*!< SMBus Host Header */
        uint32_t DUALF : 1;   /*!< Dual Flag */
        uint32_t PEC : 8;     /*!< Packet Error Checking Register */
        uint32_t RESERVED2 : 16;
    } b;
    uint32_t w;
} I2C_SR2_TypeDef;

/**
 * @brief I2C 레지스터 구조체
 */
typedef struct
{
    volatile I2C_CR1_TypeDef CR1;  /*!< I2C Control register 1,     Address offset: 0x00 */
    volatile I2C_CR2_TypeDef CR2;  /*!< I2C Control register 2,     Address offset: 0x04 */
    volatile uint32_t OAR1;        /*!< I2C Own address register 1, Address offset: 0x08 */
    volatile uint32_t OAR2;        /*!< I2C Own address register 2, Address offset: 0x0C */
    volatile uint32_t DR;          /*!< I2C Data register,          Address offset: 0x10 */
    volatile I2C_SR1_TypeDef SR1;  /*!< I2C Status register 1,      Address offset: 0x14 */
    volatile I2C_SR2_TypeDef SR2;  /*!< I2C Status register 2,      Address offset: 0x18 */
    volatile uint32_t CCR;         /*!< I2C Clock control register, Address offset: 0x1C */
    volatile uint32_t TRISE;       /*!< I2C TRISE register,         Address offset: 0x20 */
} I2C_TypeDef;

#endif /* __I2C_SFR_H */
