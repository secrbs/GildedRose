# Phase 3 Design — 아이템 타입별 static 함수 추출

## 목표

`updateQuality()`의 아이템별 업데이트 로직을 타입별 `static` 함수로 분리한다.  
`updateQuality()`는 순회 및 함수 호출만 담당하는 디스패처로 축소한다.

---

## 현황

현재 `updateQuality()`는 하나의 루프 안에 모든 아이템 타입의 로직이 혼재한다.  
sellIn 감소 전/후 처리가 섞여 있어 아이템별 동작을 한눈에 파악하기 어렵다.

---

## 변경 내용

### 추출할 static 함수 (`gilded_rose.cpp`)

헬퍼와 동일하게 `.cpp` 파일 스코프 `static` 함수로 선언한다.  
각 함수는 sellIn 감소를 포함한 해당 아이템의 하루치 업데이트 전체를 담당한다.

```cpp
static void updateNormalItem(Item& item)
{
    item.sellIn -= 1;
    decreaseQuality(item, item.sellIn < 0 ? 2 : 1);
}

static void updateAgedBrie(Item& item)
{
    item.sellIn -= 1;
    increaseQuality(item, item.sellIn < 0 ? 2 : 1);
}

static void updateSulfuras(Item& /*item*/)
{
    // sellIn, quality 모두 변화 없음
}

static void updateBackstagePass(Item& item)
{
    item.sellIn -= 1;
    if (item.sellIn < 0)
    {
        item.quality = 0;
        return;
    }
    if (item.sellIn < 5)       increaseQuality(item, 3);
    else if (item.sellIn < 10) increaseQuality(item, 2);
    else                       increaseQuality(item, 1);
}
```

### 변경 후 `updateQuality()`

```cpp
void GildedRose::updateQuality()
{
    for (auto& item : items)
    {
        if      (item.name == SULFURAS)       updateSulfuras(item);
        else if (item.name == AGED_BRIE)      updateAgedBrie(item);
        else if (item.name == BACKSTAGE_PASS) updateBackstagePass(item);
        else                                  updateNormalItem(item);
    }
}
```

- 범위 기반 for로 교체 (`items[i]` → `item`)
- 분기는 이름 비교만 남고 로직은 모두 각 함수 안으로 이동

### `gilded_rose.h` 변경 없음

static 함수는 모두 `.cpp` 내부에만 존재한다.

---

## Backstage pass sellIn 경계 검토

현재 코드의 동작:
- `sellIn >= 11` (감소 전 기준): +1
- `sellIn < 11` (감소 전 기준): +2  → 감소 후 `sellIn < 10` 과 동치
- `sellIn < 6`  (감소 전 기준): +3  → 감소 후 `sellIn < 5` 와 동치
- `sellIn == 0` (감소 전 기준): 만료 → 감소 후 `sellIn < 0` 과 동치

새 함수에서는 **sellIn을 먼저 감소**시킨 뒤 경계를 판단하므로 위 동치 관계를 적용한다.  
기존 테스트(`BackstagePassesQuality*` 5개)가 이를 검증한다.

---

## 변경하지 않는 것

- `gilded_rose.h` — 인터페이스 변경 없음
- `gilded_rose_test.cpp` — 동작이 동일하므로 변경 불필요

---

## 검증 방법

1. 빌드: `MSBuild.exe GildedRose.sln /p:Configuration=Debug`
2. 테스트 전체 통과: `.\x64\Debug\GildedRose.exe` (15/15 PASSED)
3. 커버리지 유지: `gilded_rose.cpp` 100% 확인
