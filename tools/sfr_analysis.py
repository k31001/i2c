#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
SFR 레지스터 접근 분석 도구
테스트 코드에서 SFR 레지스터의 읽기/쓰기 접근 패턴을 분석합니다.
드라이버 함수 호출을 따라가 내부에서의 SFR 접근까지 분석합니다.
"""

import os
import re
import glob
import argparse
from collections import defaultdict

# SFR 레지스터 접근을 감지하기 위한 패턴
READ_PATTERNS = [
    r'->([A-Z0-9_]+)\b',     # 구조체 포인터를 통한 레지스터 읽기 (예: DMA->CCR)
    r'([A-Z0-9_]+)\.([A-Z0-9_]+)\b',  # 직접 구조체를 통한 레지스터 읽기 (예: DMA.CCR)
    r'READ_REG\((.*?)\)',    # READ_REG 매크로를 통한 읽기
    r'GET_BIT\((.*?)\)',     # GET_BIT 매크로를 통한 읽기
]

WRITE_PATTERNS = [
    r'->([A-Z0-9_]+)\s*=',     # 구조체 포인터를 통한 레지스터 쓰기 (예: DMA->CCR = value)
    r'([A-Z0-9_]+)\.([A-Z0-9_]+)\s*=',  # 직접 구조체를 통한 레지스터 쓰기 (예: DMA.CCR = value)
    r'WRITE_REG\((.*?),',    # WRITE_REG 매크로를 통한 쓰기
    r'SET_BIT\((.*?),',      # SET_BIT 매크로를 통한 쓰기
    r'CLEAR_BIT\((.*?),',    # CLEAR_BIT 매크로를 통한 쓰기
    r'MODIFY_REG\((.*?),',   # MODIFY_REG 매크로를 통한 쓰기
]

# 함수 호출 패턴
FUNCTION_CALL_PATTERN = r'([A-Za-z0-9_]+)\s*\('

# 함수 정의 패턴
FUNCTION_DEF_PATTERN = r'(?:void|int|uint\d+_t|float|double|char|bool|static)\s+([A-Za-z0-9_]+)\s*\([^)]*\)\s*(?:;|{)'

class FunctionDefinition:
    """함수 정의 정보를 저장하는 클래스"""
    def __init__(self, name, file_path, content):
        self.name = name
        self.file_path = file_path
        self.content = content
        self.reads = []
        self.writes = []
        self.calls = []
        self.analyze()
    
    def analyze(self):
        """함수 내용에서 SFR 접근 및 다른 함수 호출을 분석"""
        # 읽기 패턴 검색
        for pattern in READ_PATTERNS:
            matches = re.findall(pattern, self.content)
            if matches:
                self.reads.extend(matches)
        
        # 쓰기 패턴 검색
        for pattern in WRITE_PATTERNS:
            matches = re.findall(pattern, self.content)
            if matches:
                self.writes.extend(matches)
        
        # 함수 호출 검색
        self.calls = re.findall(FUNCTION_CALL_PATTERN, self.content)

def extract_function_content(file_content, function_name):
    """파일 내용에서 특정 함수의 정의 내용을 추출합니다."""
    # 함수 정의 시작 위치 찾기
    pattern = r'(?:void|int|uint\d+_t|float|double|char|bool|static)\s+' + re.escape(function_name) + r'\s*\([^)]*\)\s*{'
    match = re.search(pattern, file_content)
    if not match:
        return None
    
    start_pos = match.start()
    
    # 함수 본문 끝 찾기 (중괄호 대응)
    brace_count = 0
    in_string = False
    in_char = False
    in_line_comment = False
    in_block_comment = False
    end_pos = start_pos
    
    for i in range(start_pos, len(file_content)):
        c = file_content[i]
        
        # 문자열과 주석 처리
        if not in_string and not in_char and not in_line_comment and not in_block_comment:
            if c == '"':
                in_string = True
            elif c == "'":
                in_char = True
            elif c == '/' and i + 1 < len(file_content) and file_content[i + 1] == '/':
                in_line_comment = True
            elif c == '/' and i + 1 < len(file_content) and file_content[i + 1] == '*':
                in_block_comment = True
            elif c == '{':
                brace_count += 1
            elif c == '}':
                brace_count -= 1
                if brace_count == 0:
                    end_pos = i + 1
                    break
        elif in_string:
            if c == '"' and file_content[i - 1] != '\\':
                in_string = False
        elif in_char:
            if c == "'" and file_content[i - 1] != '\\':
                in_char = False
        elif in_line_comment:
            if c == '\n':
                in_line_comment = False
        elif in_block_comment:
            if c == '*' and i + 1 < len(file_content) and file_content[i + 1] == '/':
                in_block_comment = False
                i += 1  # 주석 종료 문자 '/' 건너뛰기
    
    if end_pos > start_pos:
        return file_content[start_pos:end_pos]
    return None

def collect_all_functions(src_dir):
    """소스 디렉토리에서 모든 함수 정의를 수집합니다."""
    functions = {}
    
    # 모든 .c 파일 스캔
    for c_file in glob.glob(os.path.join(src_dir, '**', '*.c'), recursive=True):
        with open(c_file, 'r', encoding='utf-8', errors='ignore') as f:
            file_content = f.read()
        
        # 함수 정의 찾기
        function_matches = re.finditer(FUNCTION_DEF_PATTERN, file_content)
        for match in function_matches:
            function_name = match.group(1)
            function_content = extract_function_content(file_content, function_name)
            if function_content:
                functions[function_name] = FunctionDefinition(function_name, c_file, function_content)
    
    return functions

def collect_all_sfr_registers(sfr_dir):
    """SFR 디렉토리에서 모든 레지스터 이름을 수집합니다."""
    all_registers = set()
    
    # SFR 헤더 파일에서 레지스터 구조체를 찾습니다
    for header_file in glob.glob(os.path.join(sfr_dir, '*.h')):
        with open(header_file, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
            
        # 레지스터 이름 추출 (구조체 필드)
        struct_fields = re.findall(r'uint32_t\s+([A-Z0-9_]+)\s*;', content)
        all_registers.update(struct_fields)
        
        # 레지스터 유니온/구조체 이름
        typedefs = re.findall(r'typedef\s+(?:union|struct)\s+{.*?}\s+([A-Z0-9_]+_TypeDef);', content, re.DOTALL)
        all_registers.update(typedefs)
    
    return all_registers

def analyze_file_with_function_calls(file_path, all_functions):
    """파일에서 SFR 직접 접근 및 함수 호출을 통한 간접 접근을 분석합니다."""
    with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()
    
    # 직접 접근
    direct_reads = []
    direct_writes = []
    
    for pattern in READ_PATTERNS:
        matches = re.findall(pattern, content)
        if matches:
            direct_reads.extend(matches)
    
    for pattern in WRITE_PATTERNS:
        matches = re.findall(pattern, content)
        if matches:
            direct_writes.extend(matches)
    
    # 함수 호출을 통한 간접 접근
    function_calls = re.findall(FUNCTION_CALL_PATTERN, content)
    
    # 호출된 함수가 접근하는 SFR 추적
    indirect_reads = []
    indirect_writes = []
    analyzed_functions = set()  # 순환 호출 방지
    
    def trace_function_calls(func_name, depth=0, max_depth=5):
        """재귀적으로 함수 호출을 추적하고 SFR 접근을 수집합니다."""
        if depth > max_depth or func_name in analyzed_functions:
            return [], []
        
        analyzed_functions.add(func_name)
        reads = []
        writes = []
        
        if func_name in all_functions:
            func_def = all_functions[func_name]
            reads.extend(func_def.reads)
            writes.extend(func_def.writes)
            
            # 해당 함수가 호출하는 다른 함수들 추적
            for called_func in func_def.calls:
                if called_func in all_functions and called_func != func_name:  # 자기 자신 호출 방지
                    sub_reads, sub_writes = trace_function_calls(called_func, depth + 1, max_depth)
                    reads.extend(sub_reads)
                    writes.extend(sub_writes)
        
        return reads, writes
    
    for func_name in function_calls:
        sub_reads, sub_writes = trace_function_calls(func_name)
        indirect_reads.extend(sub_reads)
        indirect_writes.extend(sub_writes)
    
    # 결과 합치기
    all_reads = direct_reads + indirect_reads
    all_writes = direct_writes + indirect_writes
    
    # 중복 제거
    unique_reads = set(all_reads)
    unique_writes = set(all_writes)
    
    return {
        'direct_reads': direct_reads,
        'direct_writes': direct_writes,
        'indirect_reads': indirect_reads,
        'indirect_writes': indirect_writes,
        'reads': list(unique_reads),
        'writes': list(unique_writes),
        'total_reads': len(all_reads),
        'total_writes': len(all_writes),
        'unique_reads': len(unique_reads),
        'unique_writes': len(unique_writes),
    }

def analyze_test_directory(test_dir, src_dir, sfr_dir):
    """테스트 디렉토리의 모든 파일을 분석합니다."""
    results = {}
    all_sfr_registers = collect_all_sfr_registers(sfr_dir)
    all_functions = collect_all_functions(src_dir)
    total_registers = len(all_sfr_registers)
    
    print(f"SFR 디렉토리에서 총 {total_registers}개의 레지스터를 찾았습니다.")
    print(f"소스 디렉토리에서 총 {len(all_functions)}개의 함수 정의를 찾았습니다.")
    
    # 테스트 파일 분석
    for test_file in glob.glob(os.path.join(test_dir, '*.c')):
        file_name = os.path.basename(test_file)
        print(f"{file_name} 분석 중...")
        
        analysis = analyze_file_with_function_calls(test_file, all_functions)
        
        # SFR 레지스터 접근 비율 계산
        covered_reads = sum(1 for r in analysis['reads'] if r in all_sfr_registers)
        covered_writes = sum(1 for w in analysis['writes'] if w in all_sfr_registers)
        
        # 직접 vs 간접 접근 구분
        direct_reads = sum(1 for r in analysis['direct_reads'] if r in all_sfr_registers)
        direct_writes = sum(1 for w in analysis['direct_writes'] if w in all_sfr_registers)
        indirect_reads = sum(1 for r in analysis['indirect_reads'] if r in all_sfr_registers)
        indirect_writes = sum(1 for w in analysis['indirect_writes'] if w in all_sfr_registers)
        
        # 직접 레지스터 접근과 간접 레지스터 접근 정보 저장
        direct_read_registers = set(r for r in analysis['direct_reads'] if r in all_sfr_registers)
        direct_write_registers = set(w for w in analysis['direct_writes'] if w in all_sfr_registers)
        indirect_read_registers = set(r for r in analysis['indirect_reads'] if r in all_sfr_registers)
        indirect_write_registers = set(w for w in analysis['indirect_writes'] if w in all_sfr_registers)
        
        if total_registers > 0:
            read_coverage = (covered_reads / total_registers) * 100
            write_coverage = (covered_writes / total_registers) * 100
            total_coverage = ((covered_reads + covered_writes) / (total_registers * 2)) * 100
            
            direct_read_coverage = (direct_reads / total_registers) * 100
            direct_write_coverage = (direct_writes / total_registers) * 100
            indirect_read_coverage = (indirect_reads / total_registers) * 100
            indirect_write_coverage = (indirect_writes / total_registers) * 100
        else:
            read_coverage = write_coverage = total_coverage = 0
            direct_read_coverage = direct_write_coverage = indirect_read_coverage = indirect_write_coverage = 0
        
        # 결과 저장
        results[file_name] = {
            'total_reads': analysis['total_reads'],
            'total_writes': analysis['total_writes'],
            'direct_reads': direct_reads,
            'direct_writes': direct_writes,
            'indirect_reads': indirect_reads,
            'indirect_writes': indirect_writes,
            'unique_reads': analysis['unique_reads'],
            'unique_writes': analysis['unique_writes'],
            'covered_reads': covered_reads,
            'covered_writes': covered_writes,
            'read_coverage': read_coverage,
            'write_coverage': write_coverage,
            'total_coverage': total_coverage,
            'direct_read_coverage': direct_read_coverage,
            'direct_write_coverage': direct_write_coverage,
            'indirect_read_coverage': indirect_read_coverage,
            'indirect_write_coverage': indirect_write_coverage,
            # 레지스터 목록 추가
            'direct_read_registers': direct_read_registers,
            'direct_write_registers': direct_write_registers,
            'indirect_read_registers': indirect_read_registers,
            'indirect_write_registers': indirect_write_registers,
        }
    
    return results, all_sfr_registers

def print_results(results, all_sfr_registers=None):
    """분석 결과를 마크다운 형식으로 출력합니다."""
    print("\n# SFR 레지스터 접근 분석 결과\n")
    
    # 전체 통계 계산
    total_direct_reads = sum(data['direct_reads'] for data in results.values())
    total_indirect_reads = sum(data['indirect_reads'] for data in results.values())
    total_direct_writes = sum(data['direct_writes'] for data in results.values())
    total_indirect_writes = sum(data['indirect_writes'] for data in results.values())
    avg_read_coverage = sum(data['read_coverage'] for data in results.values()) / len(results) if results else 0
    avg_write_coverage = sum(data['write_coverage'] for data in results.values()) / len(results) if results else 0
    avg_total_coverage = sum(data['total_coverage'] for data in results.values()) / len(results) if results else 0
    
    # 메인 결과 테이블
    print("## 종합 분석 결과\n")
    print("| 파일 이름 | 읽기 (직접/간접) | 쓰기 (직접/간접) | 읽기 커버리지 | 쓰기 커버리지 | 총 커버리지 |")
    print("|:----------|:---------------:|:---------------:|:------------:|:------------:|:----------:|")
    
    # 각 파일별 결과 출력
    for file_name, data in sorted(results.items()):
        print(f"| {file_name} | {data['direct_reads']}/{data['indirect_reads']} | {data['direct_writes']}/{data['indirect_writes']} | {data['read_coverage']:.2f}% | {data['write_coverage']:.2f}% | {data['total_coverage']:.2f}% |")
    
    # 합계 행 추가
    print(f"| **전체 합계** | **{total_direct_reads}/{total_indirect_reads}** | **{total_direct_writes}/{total_indirect_writes}** | **{avg_read_coverage:.2f}%** | **{avg_write_coverage:.2f}%** | **{avg_total_coverage:.2f}%** |")
    
    # 범례
    print("\n> **범례**:")
    print("> - **직접 접근**: 테스트 코드에서 직접 SFR 레지스터에 접근한 횟수")
    print("> - **간접 접근**: 드라이버 함수를 통해 간접적으로 SFR 레지스터에 접근한 횟수")
    
    # 세부 분석 결과
    print("\n## 세부 분석 결과\n")
    
    # 세부 결과 테이블
    print("| 파일 이름 | 직접 읽기 커버리지 | 간접 읽기 커버리지 | 직접 쓰기 커버리지 | 간접 쓰기 커버리지 |")
    print("|:----------|:------------------:|:------------------:|:------------------:|:------------------:|")
    
    for file_name, data in sorted(results.items()):
        print(f"| {file_name} | {data['direct_read_coverage']:.2f}% | {data['indirect_read_coverage']:.2f}% | {data['direct_write_coverage']:.2f}% | {data['indirect_write_coverage']:.2f}% |")
        
    # 접근 유형별 시각화 (간략한 그래프 표현)
    print("\n## 접근 유형별 분석\n")
    
    # 종합 차트
    print("```")
    print("직접 읽기 vs 간접 읽기 vs 직접 쓰기 vs 간접 쓰기 비율:")
    total_accesses = total_direct_reads + total_indirect_reads + total_direct_writes + total_indirect_writes
    if total_accesses > 0:
        direct_read_percent = total_direct_reads / total_accesses * 100
        indirect_read_percent = total_indirect_reads / total_accesses * 100
        direct_write_percent = total_direct_writes / total_accesses * 100
        indirect_write_percent = total_indirect_writes / total_accesses * 100
        
        bar_length = 50
        direct_read_chars = int(direct_read_percent * bar_length / 100)
        indirect_read_chars = int(indirect_read_percent * bar_length / 100)
        direct_write_chars = int(direct_write_percent * bar_length / 100)
        indirect_write_chars = int(indirect_write_percent * bar_length / 100)
        
        print(f"직접 읽기  ({direct_read_percent:.1f}%): {'#' * direct_read_chars}")
        print(f"간접 읽기  ({indirect_read_percent:.1f}%): {'#' * indirect_read_chars}")
        print(f"직접 쓰기  ({direct_write_percent:.1f}%): {'#' * direct_write_chars}")
        print(f"간접 쓰기  ({indirect_write_percent:.1f}%): {'#' * indirect_write_chars}")
    print("```")
    
    # 각 파일별 접근하지 않은 SFR 레지스터 목록 출력 (추가된 부분)
    if all_sfr_registers:
        print("\n## 접근하지 않은 SFR 레지스터 목록\n")
        
        for file_name, data in sorted(results.items()):
            # 모든 접근(직접 + 간접) 추출
            all_accessed_registers = set()
            
            # 직접 접근 레지스터 추가
            for register in data.get('direct_read_registers', []):
                all_accessed_registers.add(register)
            for register in data.get('direct_write_registers', []):
                all_accessed_registers.add(register)
                
            # 간접 접근 레지스터 추가
            for register in data.get('indirect_read_registers', []):
                all_accessed_registers.add(register)
            for register in data.get('indirect_write_registers', []):
                all_accessed_registers.add(register)
            
            # 접근하지 않은 레지스터 찾기
            not_accessed_registers = all_sfr_registers - all_accessed_registers
            
            # 출력
            print(f"### {file_name}의 접근하지 않은 SFR 레지스터 ({len(not_accessed_registers)}개)")
            if not_accessed_registers:
                print("\n```")
                # 접근하지 않은 레지스터 목록을 여러 열로 정렬하여 출력
                registers_list = sorted(list(not_accessed_registers))
                col_width = max(len(reg) for reg in registers_list) + 2  # 여백 추가
                cols = 3  # 열 수
                
                # 행 수 계산 (올림)
                rows = (len(registers_list) + cols - 1) // cols
                
                for row in range(rows):
                    line = ""
                    for col in range(cols):
                        idx = row + col * rows
                        if idx < len(registers_list):
                            line += registers_list[idx].ljust(col_width)
                    print(line)
                print("```\n")
            else:
                print("모든 SFR 레지스터에 접근했습니다.\n")
    
    print("\n분석 완료되었습니다.")

def main():
    parser = argparse.ArgumentParser(description='SFR 레지스터 접근 분석 도구')
    parser.add_argument('--test-dir', default='../src/test', help='테스트 디렉토리 경로')
    parser.add_argument('--src-dir', default='../src', help='소스 디렉토리 경로')
    parser.add_argument('--sfr-dir', default='../src/sfr', help='SFR 디렉토리 경로')
    
    args = parser.parse_args()
    
    # 경로를 절대 경로로 변환
    test_dir = os.path.abspath(args.test_dir)
    src_dir = os.path.abspath(args.src_dir)
    sfr_dir = os.path.abspath(args.sfr_dir)
    
    print(f"테스트 디렉토리: {test_dir}")
    print(f"소스 디렉토리: {src_dir}")
    print(f"SFR 디렉토리: {sfr_dir}")
    
    # 분석 실행
    results, all_sfr_registers = analyze_test_directory(test_dir, src_dir, sfr_dir)
    
    # 결과 출력
    print_results(results, all_sfr_registers)
    
    print("\n분석이 완료되었습니다.")

if __name__ == "__main__":
    main()
