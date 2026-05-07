# Phase 1 Design — 매직 스트링 상수화

## 목표

`gilded_rose.cpp` 내 하드코딩된 아이템 이름 문자열을 named constant로 추출한다.  
동작 변경 없이 가독성과 오타 방지를 확보하고, 이후 Phase의 기반을 마련한다.

---

## 현황

`gilded_rose.cpp`에 동일한 문자열이 아래와 같이 반복 등장한다.

| 문자열 | 등장 횟수 | 위치 (line) |
|--------|-----------|-------------|
| `"Aged Brie"` | 2 | 14, 58 |
| `"Backstage passes to a TAFKAL80ETC concert"` | 3 | 14, 30, 60 |
| `"Sulfuras, Hand of Ragnaros"` | 3 | 18, 51, 64 |

---

## 변경 내용

### 상수 선언 위치

`gilded_rose.cpp` 상단 (`using` 선언 바로 아래)에 파일 스코프 상수로 선언한다.  
`gilded_rose.h`에 두지 않는 이유: 현재 상수는 `updateQuality()` 구현 내부에서만 사용되며,  
외부(테스트 포함)에서 직접 참조할 필요가 없기 때문이다.

```cpp
// gilded_rose.cpp
using std::vector;
using std::string;

static const string AGED_BRIE      = "Aged Brie";
static const string SULFURAS       = "Sulfuras, Hand of Ragnaros";
static const string BACKSTAGE_PASS = "Backstage passes to a TAFKAL80ETC concert";
```

### 치환 목록

| 변경 전 | 변경 후 |
|---------|---------|
| `"Aged Brie"` | `AGED_BRIE` |
| `"Sulfuras, Hand of Ragnaros"` | `SULFURAS` |
| `"Backstage passes to a TAFKAL80ETC concert"` | `BACKSTAGE_PASS` |

적용 대상: `gilded_rose.cpp` line 14, 18, 30, 51, 58, 60, 64 (총 8개 치환)

---

## 변경하지 않는 것

- `gilded_rose.h` — 인터페이스 변경 없음
- `gilded_rose_test.cpp` — 테스트 코드는 문자열 리터럴을 그대로 유지  
  (테스트는 구현 내부 상수가 아닌 실제 아이템 이름으로 시나리오를 표현하는 것이 맞음)
- `updateQuality()` 로직 — 동작 변경 없음

---

## 검증 방법

1. 빌드: `MSBuild.exe GildedRose.sln /p:Configuration=Debug`
2. 테스트 전체 통과: `.\x64\Debug\GildedRose.exe` (15/15 PASSED)
3. 커버리지 유지: `gilded_rose.cpp` 100% 확인
