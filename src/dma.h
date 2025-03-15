#ifndef __DMA_H
#define __DMA_H

#include "stm32f411xe.h"

/**
 * @brief DMA 스트림 정의
 */
typedef enum
{
    DMA_STREAM_0 = 0,
    DMA_STREAM_1,
    DMA_STREAM_2,
    DMA_STREAM_3,
    DMA_STREAM_4,
    DMA_STREAM_5,
    DMA_STREAM_6,
    DMA_STREAM_7
} DMA_Stream;

/**
 * @brief DMA 채널 정의
 */
typedef enum
{
    DMA_CHANNEL_0 = 0,
    DMA_CHANNEL_1,
    DMA_CHANNEL_2,
    DMA_CHANNEL_3,
    DMA_CHANNEL_4,
    DMA_CHANNEL_5,
    DMA_CHANNEL_6,
    DMA_CHANNEL_7
} DMA_Channel;

/**
 * @brief DMA 우선순위 정의
 */
typedef enum
{
    DMA_PRIORITY_LOW = 0,
    DMA_PRIORITY_MEDIUM,
    DMA_PRIORITY_HIGH,
    DMA_PRIORITY_VERY_HIGH
} DMA_Priority;

/**
 * @brief DMA 방향 정의
 */
typedef enum
{
    DMA_DIR_PERIPH_TO_MEMORY = 0,
    DMA_DIR_MEMORY_TO_PERIPH,
    DMA_DIR_MEMORY_TO_MEMORY
} DMA_Direction;

/**
 * @brief DMA 메모리 및 주변장치 데이터 크기 정의
 */
typedef enum
{
    DMA_SIZE_BYTE = 0,
    DMA_SIZE_HALF_WORD,
    DMA_SIZE_WORD
} DMA_DataSize;

/**
 * @brief DMA 증분 모드 정의
 */
typedef enum
{
    DMA_INCREMENT_DISABLE = 0,
    DMA_INCREMENT_ENABLE
} DMA_Increment;

/**
 * @brief DMA 모드 정의
 */
typedef enum
{
    DMA_MODE_NORMAL = 0,
    DMA_MODE_CIRCULAR,
    DMA_MODE_PFCTRL    // 주변장치 흐름 제어
} DMA_Mode;

/**
 * @brief DMA 버스트 모드 정의
 */
typedef enum
{
    DMA_BURST_SINGLE = 0,
    DMA_BURST_INCR4,
    DMA_BURST_INCR8,
    DMA_BURST_INCR16
} DMA_Burst;

/**
 * @brief DMA FIFO 임계값 정의
 */
typedef enum
{
    DMA_FIFO_THRESHOLD_1_4 = 0,
    DMA_FIFO_THRESHOLD_1_2,
    DMA_FIFO_THRESHOLD_3_4,
    DMA_FIFO_THRESHOLD_FULL
} DMA_FifoThreshold;

/**
 * @brief DMA 초기화 구조체
 */
typedef struct
{
    DMA_Channel     Channel;            /*!< DMA 채널 선택 */
    DMA_Direction   Direction;          /*!< 데이터 전송 방향 */
    DMA_Increment   MemInc;             /*!< 메모리 주소 증가 모드 */
    DMA_Increment   PeriphInc;          /*!< 주변장치 주소 증가 모드 */
    DMA_DataSize    MemDataSize;        /*!< 메모리 데이터 크기 */
    DMA_DataSize    PeriphDataSize;     /*!< 주변장치 데이터 크기 */
    DMA_Mode        Mode;               /*!< DMA 동작 모드 */
    DMA_Priority    Priority;           /*!< 스트림 우선순위 */
    uint8_t         FIFOMode;           /*!< FIFO 모드 (0: 직접 모드, 1: FIFO 모드) */
    DMA_FifoThreshold FIFOThreshold;    /*!< FIFO 임계값 */
    DMA_Burst       MemBurst;           /*!< 메모리 버스트 전송 설정 */
    DMA_Burst       PeriphBurst;        /*!< 주변장치 버스트 전송 설정 */
} DMA_Config;

/**
 * @brief  DMA 스트림을 초기화합니다.
 * @param  DMAx: 초기화할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 초기화할 DMA 스트림
 * @param  config: DMA 초기화 구조체 포인터
 * @return None
 */
void DMA_Init(DMA_TypeDef *DMAx, DMA_Stream stream, DMA_Config *config);

/**
 * @brief  DMA 스트림을 비활성화합니다.
 * @param  DMAx: 비활성화할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 비활성화할 DMA 스트림
 * @return None
 */
void DMA_DeInit(DMA_TypeDef *DMAx, DMA_Stream stream);

/**
 * @brief  DMA 전송을 구성합니다.
 * @param  DMAx: 구성할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 구성할 DMA 스트림
 * @param  SrcAddress: 소스 주소
 * @param  DstAddress: 대상 주소
 * @param  DataLength: 전송할 데이터 항목 수
 * @return None
 */
void DMA_ConfigTransfer(DMA_TypeDef *DMAx, DMA_Stream stream, uint32_t SrcAddress, uint32_t DstAddress, uint16_t DataLength);

/**
 * @brief  DMA 스트림을 활성화합니다.
 * @param  DMAx: 활성화할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 활성화할 DMA 스트림
 * @return None
 */
void DMA_Enable(DMA_TypeDef *DMAx, DMA_Stream stream);

/**
 * @brief  DMA 스트림을 비활성화합니다.
 * @param  DMAx: 비활성화할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 비활성화할 DMA 스트림
 * @return None
 */
void DMA_Disable(DMA_TypeDef *DMAx, DMA_Stream stream);

/**
 * @brief  DMA 전송 완료 플래그를 확인합니다.
 * @param  DMAx: 확인할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 확인할 DMA 스트림
 * @return 0: 전송 중, 1: 전송 완료
 */
uint8_t DMA_IsTransferComplete(DMA_TypeDef *DMAx, DMA_Stream stream);

/**
 * @brief  DMA 전송 오류 플래그를 확인합니다.
 * @param  DMAx: 확인할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 확인할 DMA 스트림
 * @return 0: 오류 없음, 1: 전송 오류
 */
uint8_t DMA_IsTransferError(DMA_TypeDef *DMAx, DMA_Stream stream);

/**
 * @brief  DMA 전송 중단 플래그를 확인합니다.
 * @param  DMAx: 확인할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 확인할 DMA 스트림
 * @return 0: 중단 없음, 1: 전송 중단
 */
uint8_t DMA_IsTransferAborted(DMA_TypeDef *DMAx, DMA_Stream stream);

/**
 * @brief  DMA 절반 전송 완료 플래그를 확인합니다.
 * @param  DMAx: 확인할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 확인할 DMA 스트림
 * @return 0: 절반 전송 완료 아님, 1: 절반 전송 완료
 */
uint8_t DMA_IsHalfTransferComplete(DMA_TypeDef *DMAx, DMA_Stream stream);

/**
 * @brief  DMA 플래그를 지웁니다.
 * @param  DMAx: 플래그를 지울 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 플래그를 지울 DMA 스트림
 * @return None
 */
void DMA_ClearFlags(DMA_TypeDef *DMAx, DMA_Stream stream);

/**
 * @brief  DMA 인터럽트를 활성화합니다.
 * @param  DMAx: 인터럽트를 활성화할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 인터럽트를 활성화할 DMA 스트림
 * @param  transferComplete: 전송 완료 인터럽트 (0: 비활성화, 1: 활성화)
 * @param  halfTransfer: 절반 전송 인터럽트 (0: 비활성화, 1: 활성화)
 * @param  transferError: 전송 오류 인터럽트 (0: 비활성화, 1: 활성화)
 * @param  directModeError: 직접 모드 오류 인터럽트 (0: 비활성화, 1: 활성화)
 * @return None
 */
void DMA_EnableInterrupts(DMA_TypeDef *DMAx, DMA_Stream stream, uint8_t transferComplete, uint8_t halfTransfer, uint8_t transferError, uint8_t directModeError);

/**
 * @brief  DMA 인터럽트를 비활성화합니다.
 * @param  DMAx: 인터럽트를 비활성화할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 인터럽트를 비활성화할 DMA 스트림
 * @return None
 */
void DMA_DisableInterrupts(DMA_TypeDef *DMAx, DMA_Stream stream);

#endif /* __DMA_H */
