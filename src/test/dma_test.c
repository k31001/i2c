#include "dma.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief 테스트 결과를 출력하는 헬퍼 함수
 */
static void PrintTestResult(const char* test_name, DMA_Status status) {
    if (status == DMA_OK) {
        printf("%s: 성공\n", test_name);
    } else {
        printf("%s: 실패 (상태: %d)\n", test_name, status);
    }
}

/**
 * @brief DMA 메모리-메모리 전송 테스트를 수행하는 헬퍼 함수
 */
static void TestDMAMemToMem(DMA_Stream_TypeDef* DMAx_Stream, const char* test_name, 
                           const uint32_t* src, uint32_t* dst, uint32_t size) {
    DMA_Status status = DMA_MemToMem(DMAx_Stream, (uint32_t)src, (uint32_t)dst, size);
    PrintTestResult(test_name, status);
    
    if (status == DMA_OK) {
        printf("전송 데이터 확인:\n");
        printf("원본: ");
        for (uint32_t i = 0; i < size; i++) {
            printf("0x%08X ", src[i]);
        }
        printf("\n복사본: ");
        for (uint32_t i = 0; i < size; i++) {
            printf("0x%08X ", dst[i]);
        }
        printf("\n");
    }
}

/**
 * @brief DMA 기본 전송 테스트
 */
static void Test_DMA_Basic_Transfer(DMA_Stream_TypeDef* DMAx_Stream) {
    printf("\n=== DMA 기본 전송 테스트 ===\n");
    
    // 단일 워드 전송 테스트
    printf("\n단일 워드 전송 테스트...\n");
    uint32_t src_word = 0x12345678;
    uint32_t dst_word = 0;
    TestDMAMemToMem(DMAx_Stream, "단일 워드 전송", &src_word, &dst_word, 1);
    
    // 다중 워드 전송 테스트
    printf("\n다중 워드 전송 테스트...\n");
    uint32_t src_array[4] = {0xAABBCCDD, 0x11223344, 0x55667788, 0x99AABBCC};
    uint32_t dst_array[4] = {0};
    TestDMAMemToMem(DMAx_Stream, "다중 워드 전송", src_array, dst_array, 4);
}

/**
 * @brief DMA 버스트 전송 테스트
 */
static void Test_DMA_Burst_Transfer(DMA_Stream_TypeDef* DMAx_Stream) {
    printf("\n=== DMA 버스트 전송 테스트 ===\n");
    
    DMA_Config config = {
        .Channel = DMA_CHANNEL_0,
        .Direction = DMA_DIRECTION_MEM_TO_MEM,
        .PeriphInc = DMA_PINC_ENABLE,
        .MemInc = DMA_MINC_ENABLE,
        .PeriphDataAlign = DMA_PDATAALIGN_WORD,
        .MemDataAlign = DMA_MDATAALIGN_WORD,
        .Mode = DMA_MODE_NORMAL,
        .Priority = DMA_PRIORITY_HIGH,
        .FIFOMode = DMA_FIFOMODE_ENABLE,
        .FIFOThreshold = DMA_FIFO_THRESHOLD_FULL,
        .MemBurst = DMA_MBURST_INCR4,
        .PeriphBurst = DMA_PBURST_INCR4
    };
    
    // 버스트 모드로 DMA 초기화
    DMA_Init(DMAx_Stream, &config);
    
    // 큰 데이터 블록 전송 테스트
    printf("\n버스트 모드 전송 테스트...\n");
    uint32_t src_block[16];
    uint32_t dst_block[16] = {0};
    
    // 테스트 데이터 초기화
    for (int i = 0; i < 16; i++) {
        src_block[i] = 0xA0000000 | i;
    }
    
    TestDMAMemToMem(DMAx_Stream, "버스트 전송", src_block, dst_block, 16);
}

/**
 * @brief DMA 에러 처리 테스트
 */
static void Test_DMA_Error_Functions(DMA_Stream_TypeDef* DMAx_Stream) {
    printf("\n=== DMA 에러 처리 테스트 ===\n");
    
    // 잘못된 정렬 테스트
    printf("\n잘못된 정렬 테스트...\n");
    uint8_t* src_unaligned = (uint8_t*)0x20000001;  // 의도적으로 잘못된 정렬
    uint32_t dst_word = 0;
    DMA_Status status = DMA_MemToMem(DMAx_Stream, (uint32_t)src_unaligned, (uint32_t)&dst_word, 1);
    PrintTestResult("잘못된 정렬", status);
    
    // FIFO 오버플로우 테스트
    printf("\nFIFO 오버플로우 테스트...\n");
    DMA_Config config = {
        .Channel = DMA_CHANNEL_0,
        .Direction = DMA_DIRECTION_MEM_TO_MEM,
        .PeriphInc = DMA_PINC_ENABLE,
        .MemInc = DMA_MINC_ENABLE,
        .PeriphDataAlign = DMA_PDATAALIGN_BYTE,
        .MemDataAlign = DMA_MDATAALIGN_WORD,  // 의도적인 불일치
        .Mode = DMA_MODE_NORMAL,
        .Priority = DMA_PRIORITY_HIGH,
        .FIFOMode = DMA_FIFOMODE_ENABLE,
        .FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL  // 낮은 임계값
    };
    DMA_Init(DMAx_Stream, &config);
    
    uint8_t src_bytes[32];
    uint32_t dst_words[8];
    status = DMA_MemToMem(DMAx_Stream, (uint32_t)src_bytes, (uint32_t)dst_words, 32);
    PrintTestResult("FIFO 오버플로우", status);
}

void DMA_Test(void) {
    printf("===== DMA 드라이버 테스트 시작 =====\n");
    
    // 기본 DMA 설정으로 초기화
    DMA_Config dma_config = {
        .Channel = DMA_CHANNEL_0,
        .Direction = DMA_DIRECTION_MEM_TO_MEM,
        .PeriphInc = DMA_PINC_ENABLE,
        .MemInc = DMA_MINC_ENABLE,
        .PeriphDataAlign = DMA_PDATAALIGN_WORD,
        .MemDataAlign = DMA_MDATAALIGN_WORD,
        .Mode = DMA_MODE_NORMAL,
        .Priority = DMA_PRIORITY_HIGH,
        .FIFOMode = DMA_FIFOMODE_DISABLE
    };
    DMA_Init(DMA2_Stream0, &dma_config);
    
    // 테스트 실행
    Test_DMA_Basic_Transfer(DMA2_Stream0);
    Test_DMA_Burst_Transfer(DMA2_Stream0);
    Test_DMA_Error_Functions(DMA2_Stream0);
    
    // 정리
    DMA_DeInit(DMA2_Stream0);
    printf("\n===== DMA 드라이버 테스트 완료 =====\n");
}
