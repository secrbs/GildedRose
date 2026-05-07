#include "gilded_rose.h"
#include <algorithm>

using std::vector;
using std::string;

static const string AGED_BRIE      = "Aged Brie";
static const string SULFURAS       = "Sulfuras, Hand of Ragnaros";
static const string BACKSTAGE_PASS = "Backstage passes to a TAFKAL80ETC concert";

static void increaseQuality(Item& item, int amount = 1)
{
    item.quality = std::min(item.quality + amount, 50);
}

static void decreaseQuality(Item& item, int amount = 1)
{
    item.quality = std::max(item.quality - amount, 0);
}

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
}

static void updateBackstagePass(Item& item)
{
    item.sellIn -= 1;
    if (item.sellIn < 0)
    {
        item.quality = 0;
        return;
    }
    if      (item.sellIn < 5)  increaseQuality(item, 3);
    else if (item.sellIn < 10) increaseQuality(item, 2);
    else                       increaseQuality(item, 1);
}

GildedRose::GildedRose(vector<Item>& items) : items(items)
{
}

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
