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

GildedRose::GildedRose(vector<Item>& items) : items(items)
{
}

void GildedRose::updateQuality()
{
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i].name != AGED_BRIE && items[i].name != BACKSTAGE_PASS)
        {
            if (items[i].name != SULFURAS)
            {
                decreaseQuality(items[i]);
            }
        }
        else
        {
            increaseQuality(items[i]);

            if (items[i].name == BACKSTAGE_PASS)
            {
                if (items[i].sellIn < 11)
                {
                    increaseQuality(items[i]);
                }

                if (items[i].sellIn < 6)
                {
                    increaseQuality(items[i]);
                }
            }
        }

        if (items[i].name != SULFURAS)
        {
            items[i].sellIn = items[i].sellIn - 1;
        }

        if (items[i].sellIn < 0)
        {
            if (items[i].name != AGED_BRIE)
            {
                if (items[i].name != BACKSTAGE_PASS)
                {
                    if (items[i].name != SULFURAS)
                    {
                        decreaseQuality(items[i]);
                    }
                }
                else
                {
                    items[i].quality = 0;
                }
            }
            else
            {
                increaseQuality(items[i]);
            }
        }
    }
}
