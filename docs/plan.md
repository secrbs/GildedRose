# Gilded Rose C++ Refactoring Plan

## 현황

- `gilded_rose.cpp`의 `updateQuality()`는 깊은 중첩 if-else 단일 메서드로 구현됨
- 기존 테스트(`gilded_rose_test.cpp`) 12개가 `gilded_rose.cpp` 100% 커버 중
- 별도 유닛 테스트 추가 없이 기존 테스트를 안전망으로 사용

---

## 각 Phase 작업 절차

각 Phase는 아래 순서를 반드시 따른다.

1. **Design 문서 작성** — `docs/phase{N}-design.md`에 변경 내용, 인터페이스, 예상 결과 기술
2. **사람이 Design 검토** — 검토 완료 후 구현 진행
3. **구현** — Design 문서 기준으로 코드 작성, 빌드·테스트·커버리지 확인 후 커밋
4. **사람이 코드 검토** — 검토 완료 후 다음 Phase로 이동

---

## 단계별 리팩토링 계획

### Phase 1 — 매직 스트링 상수화

**변경 범위:** `gilded_rose.cpp` 상단

반복 등장하는 아이템 이름 문자열을 상수로 추출한다.

```cpp
static const std::string AGED_BRIE       = "Aged Brie";
static const std::string SULFURAS        = "Sulfuras, Hand of Ragnaros";
static const std::string BACKSTAGE_PASS  = "Backstage passes to a TAFKAL80ETC concert";
```

- 동작 변경 없음
- 오타로 인한 버그 방지, 이후 단계의 가독성 향상

---

### Phase 2 — Quality 경계 처리 헬퍼 추출

**변경 범위:** `GildedRose` 클래스에 private 메서드 추가

quality 증감 시 0~50 범위를 매번 직접 확인하는 코드를 헬퍼로 일원화한다.

```cpp
void increaseQuality(Item& item, int amount = 1);
void decreaseQuality(Item& item, int amount = 1);
```

- 범위 체크 로직 중복 제거
- 이후 단계에서 각 분기가 단순해짐

---

### Phase 3 — 아이템 타입별 private 메서드 추출

**변경 범위:** `updateQuality()` 분해 → 4개의 private 메서드

```cpp
void updateNormalItem(Item& item);
void updateAgedBrie(Item& item);
void updateSulfuras(Item& item);
void updateBackstagePass(Item& item);
```

`updateQuality()`는 아이템 이름으로 분기하여 각 메서드를 호출하는 디스패처로만 남긴다.  
각 메서드는 Phase 2의 헬퍼를 사용하므로 단순하고 평탄한 구조가 된다.

---

### Phase 4 — 분기 디스패치를 (predicate, updater) 테이블로 교체

**변경 범위:** `updateQuality()` 내부 if-else 체인 → `std::vector<pair<Predicate, Updater>>`

이름 문자열을 키로 쓰는 맵 대신, 술어(predicate)와 업데이터를 묶은 목록을 사용한다.  
일반 아이템도 명시적인 술어(`true`)로 등록하여 모든 항목을 동등하게 취급한다.

```cpp
using Updater   = std::function<void(Item&)>;
using Predicate = std::function<bool(const Item&)>;

const std::vector<std::pair<Predicate, Updater>> updaters = {
    { [](const Item& i){ return i.name == SULFURAS;       }, [this](Item& i){ updateSulfuras(i);      } },
    { [](const Item& i){ return i.name == AGED_BRIE;      }, [this](Item& i){ updateAgedBrie(i);      } },
    { [](const Item& i){ return i.name == BACKSTAGE_PASS; }, [this](Item& i){ updateBackstagePass(i); } },
    { [](const Item& i){ return true;                     }, [this](Item& i){ updateNormalItem(i);    } },
};
// 순서대로 predicate를 평가하여 처음 일치하는 updater 호출
```

- 일반 아이템이 "나머지 처리(fallback)"가 아닌 명시적 규칙으로 등록됨
- 새 아이템 종류(예: Conjured) 추가 시 목록에 항목 삽입만 하면 됨
- `Item` 클래스를 수정하지 않으면서 확장에 열린 구조 확보

---

## 단계별 커밋 전략

각 Phase 완료 후 독립적으로 커밋한다. 커밋 전 반드시:
1. 빌드 성공 확인: `MSBuild.exe GildedRose.sln /p:Configuration=Debug`
2. 전체 테스트 통과 확인: `.\x64\Debug\GildedRose.exe`
3. 커버리지 재측정으로 100% 유지 확인
