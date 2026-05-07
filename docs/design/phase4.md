# Phase 4 Design — 분기 디스패치를 (predicate, updater) 테이블로 교체

## 목표

`updateQuality()`의 if-else 체인을 `(predicate, updater)` 쌍의 테이블로 교체한다.  
일반 아이템을 `return true` 술어로 명시적으로 등록하여 모든 아이템 타입을 동등하게 취급한다.  
새 아이템 종류 추가 시 테이블에 한 줄 삽입만으로 대응 가능한 구조를 확보한다.

---

## 변경 내용

### `updateQuality()` 교체 (`gilded_rose.cpp`)

```cpp
void GildedRose::updateQuality()
{
    using Predicate = bool (*)(const Item&);
    using Updater   = void (*)(Item&);

    static const std::pair<Predicate, Updater> updaters[] = {
        { [](const Item& i){ return i.name == SULFURAS;       }, updateSulfuras      },
        { [](const Item& i){ return i.name == AGED_BRIE;      }, updateAgedBrie      },
        { [](const Item& i){ return i.name == BACKSTAGE_PASS; }, updateBackstagePass },
        { [](const Item& i){ return true;                     }, updateNormalItem    },
    };

    for (auto& item : items)
    {
        for (const auto& [pred, update] : updaters)
        {
            if (pred(item))
            {
                update(item);
                break;
            }
        }
    }
}
```

- `static const` 배열로 선언하여 호출마다 재생성하지 않음
- 캡처 없는 람다는 함수 포인터로 변환 가능하므로 `std::function` 불필요
- 순서대로 predicate를 평가하여 처음 일치하는 updater 호출 후 break
- 일반 아이템은 `return true`로 마지막에 명시적으로 등록

### `gilded_rose.h` 변경 없음

---

## Phase 5를 위한 확장 예시

Conjured 아이템 추가 시 테이블에 한 줄 삽입하면 된다.

```cpp
{ [](const Item& i){ return i.name == CONJURED; }, updateConjured },
{ [](const Item& i){ return true;               }, updateNormalItem },
```

---

## 변경하지 않는 것

- 개별 update 함수(`updateNormalItem`, `updateAgedBrie` 등) — 변경 없음
- `gilded_rose.h` — 인터페이스 변경 없음
- `gilded_rose_test.cpp` — 동작이 동일하므로 변경 불필요

---

## 검증 방법

1. 빌드: `MSBuild.exe GildedRose.sln /p:Configuration=Debug`
2. 테스트 전체 통과: `.\x64\Debug\GildedRose.exe` (12/12 PASSED)
3. 커버리지 유지: `gilded_rose.cpp` 100% 확인
