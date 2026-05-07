# Phase 2 Design — Quality 경계 처리 헬퍼 추출

## 목표

quality 증감 시 매번 직접 작성하는 범위 체크(`< 50`, `> 0`)를 파일 스코프 `static` 헬퍼 함수로 일원화한다.  
동작 변경 없이 중복을 제거하고, 이후 Phase에서 각 아이템 업데이트 함수가 단순해지는 기반을 만든다.

---

## 현황 — 중복 패턴

`gilded_rose.cpp`에서 quality 조작 패턴이 아래와 같이 반복된다.

**증가 패턴** (quality < 50 체크 후 +1) — 4곳
```cpp
if (items[i].quality < 50)
    items[i].quality = items[i].quality + 1;
```

**감소 패턴** (quality > 0 체크 후 -1) — 2곳
```cpp
if (items[i].quality > 0)
    items[i].quality = items[i].quality - 1;
```

**0으로 초기화** (Backstage pass 만료) — 1곳
```cpp
items[i].quality = items[i].quality - items[i].quality;
```

---

## 변경 내용

### 헬퍼 함수 위치

`gilded_rose.h`는 변경하지 않는다.  
헬퍼는 `GildedRose`의 멤버 변수를 사용하지 않고 `Item&`만 받아 동작하므로, 클래스 멤버로 만들 이유가 없다.  
Phase 1의 `static const` 상수와 동일하게 `gilded_rose.cpp` 파일 스코프 `static` 함수로 선언한다.

```cpp
// gilded_rose.cpp — 상수 선언 아래에 추가
static void increaseQuality(Item& item, int amount = 1)
{
    item.quality = std::min(item.quality + amount, 50);
}

static void decreaseQuality(Item& item, int amount = 1)
{
    item.quality = std::max(item.quality - amount, 0);
}
```

- `increaseQuality`: quality를 amount만큼 올리되 50을 초과하지 않음
- `decreaseQuality`: quality를 amount만큼 내리되 0 미만이 되지 않음
- 기존 코드에서 `< 50`, `> 0` 체크를 매번 쓰던 것을 헬퍼 내부로 흡수
- 헤더에 구현 세부사항을 노출하지 않음

### `updateQuality()` 치환 목록

| 변경 전 | 변경 후 |
|---------|---------|
| `if (items[i].quality < 50) items[i].quality = items[i].quality + 1;` | `increaseQuality(items[i]);` |
| `if (items[i].quality > 0) items[i].quality = items[i].quality - 1;` | `decreaseQuality(items[i]);` |
| `items[i].quality = items[i].quality - items[i].quality;` | `items[i].quality = 0;` |

적용 후 `updateQuality()` 내 quality 조작은 모두 헬퍼 호출로 대체되며, 범위 체크 코드가 사라진다.

---

## 변경하지 않는 것

- `Item` 클래스 — 수정 금지
- `updateQuality()`의 분기 구조 — Phase 3에서 다룸
- `gilded_rose_test.cpp` — 동작이 동일하므로 변경 불필요

---

## 검증 방법

1. 빌드: `MSBuild.exe GildedRose.sln /p:Configuration=Debug`
2. 테스트 전체 통과: `.\x64\Debug\GildedRose.exe` (15/15 PASSED)
3. 커버리지 유지: `gilded_rose.cpp` 100% 확인
