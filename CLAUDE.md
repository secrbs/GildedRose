# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 프로젝트 개요

Gilded Rose 리팩토링 카타(Kata)입니다. 레거시 코드(`updateQuality` 메서드)를 리팩토링하고, "Conjured" 아이템 지원을 추가하는 것이 목표입니다. Python, Java, C++ 세 가지 언어로 동일한 구현이 존재합니다.

### 핵심 제약사항
- `Item` 클래스와 `items` 속성은 절대 수정하지 말 것 (고블린의 소유)
- `updateQuality()` 메서드와 `GildedRose` 클래스는 자유롭게 수정 가능
- `Quality`는 0 미만, 50 초과가 될 수 없음 (단, Sulfuras는 예외로 항상 80)

## 언어별 명령어

### Python (`python/`)
```bash
# 테스트 실행
cd python && python -m pytest test_gilded_rose.py

# 단일 테스트 실행
cd python && python -m pytest test_gilded_rose.py::test_foo

# 텍스트 픽스처 실행 (시뮬레이션 출력)
cd python && python texttest_fixture.py
```

### Java (`Java/`)
```bash
# 테스트 실행
cd Java && ./gradlew test

# 텍스트 픽스처 실행 (30일 시뮬레이션)
cd Java && ./gradlew texttest

# Windows에서는 gradlew.bat 사용
cd Java && gradlew.bat test
```

### C++ (`cpp/`)
Visual Studio 솔루션(`GildedRose.sln`)을 사용하여 빌드합니다.
- Debug 빌드: Google Mock 기반 테스트 실행 (`main.cpp`의 `#ifdef _DEBUG` 분기)
- Release 빌드: 텍스트 픽스처 출력 실행

```bash
# 빌드 (프로젝트 루트에서)
MSBuild.exe GildedRose.sln /p:Configuration=Debug

# 테스트 실행
.\x64\Debug\GildedRose.exe

# 커버리지 측정 (결과는 .\Coverage\index.html 에서 확인)
OpenCppCoverage.exe --sources cpp\*.cpp --export_type=html:Coverage -- .\x64\Debug\GildedRose.exe
```

## 개발 워크플로우

새로운 기능 개발 또는 리팩토링 작업 시 아래 순서를 따른다.

1. **개발 목표 설정** — 작업 범위와 목표를 명확히 정의
2. **플랜 작성** — `docs/plan.md`에 Phase별 계획 작성
3. **CLAUDE.md 반영** — 진행 중인 플랜 파일 경로를 CLAUDE.md `진행 대상 플랜` 항목에 추가
4. **각 Phase 작업** — 아래 Phase 작업 절차에 따라 진행
5. **개발 완료 확인** — 사람이 완료를 확인하면 `plan.md`에 완료 표시
6. **커밋 작성**

### Phase 작업 절차

각 Phase는 반드시 아래 순서를 따른다.

1. **Design 문서 작성** — `docs/design/phase{N}.md`에 변경 내용, 인터페이스, 예상 결과 기술
2. **사람이 Design 검토** — 검토 완료 후 구현 진행
3. **구현** — Design 문서 기준으로 코드 작성, 빌드·테스트·커버리지 확인 후 커밋
4. **사람이 코드 검토** — 검토 완료 후 다음 Phase로 이동

## 진행 대상 플랜

`docs/plan.md`

---

## 아키텍처

세 언어 모두 동일한 구조를 가집니다:

- **`Item`** — 불변 데이터 클래스. `name`, `sellIn`, `quality` 세 필드만 보유. 수정 금지.
- **`GildedRose`** — `items` 배열/리스트를 받아 `updateQuality()`로 하루치 품질 업데이트를 수행.
- **`TexttestFixture`** — 여러 아이템을 며칠간 시뮬레이션하여 콘솔에 출력하는 수동 검증 도구.

### 아이템 종류별 업데이트 규칙
| 아이템 | 규칙 |
|--------|------|
| 일반 아이템 | `sellIn` 감소, `quality` 1 감소. `sellIn < 0`이면 2 감소 |
| Aged Brie | `quality` 증가 (만료 후 2씩 증가) |
| Sulfuras, Hand of Ragnaros | `sellIn`, `quality` 변화 없음 |
| Backstage passes | `sellIn > 10`: +1, `sellIn 6~10`: +2, `sellIn 1~5`: +3, 만료 후: 0 |
| Conjured (미구현) | 일반 아이템의 2배 속도로 `quality` 감소 |

현재 코드는 깊은 중첩 if문으로 구현되어 있으며, 리팩토링이 필요한 상태입니다.
