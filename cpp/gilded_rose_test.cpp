#include "gmock/gmock.h"
#include "gilded_rose.h"

using std::vector;
using std::string;

TEST(GildedRoseTest, Foo) {
    vector<Item> items;
    items.emplace_back("Foo", 0, 0);
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ("Foo", app.items[0].name);
}

TEST(GildedRoseTest, NormalItemQualityDecreases) {
    vector<Item> items;
    items.emplace_back("Normal Item", 10, 20);
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(9, app.items[0].sellIn);
    EXPECT_EQ(19, app.items[0].quality);
}

TEST(GildedRoseTest, NormalItemQualityDegradesTwiceAfterSellIn) {
    vector<Item> items;
    items.emplace_back("Normal Item", 0, 10);
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(8, app.items[0].quality);
}

TEST(GildedRoseTest, NormalItemQualityNeverNegative) {
    vector<Item> items;
    items.emplace_back("Normal Item", 0, 0);
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(0, app.items[0].quality);
}

TEST(GildedRoseTest, AgedBrieQualityIncreases) {
    vector<Item> items;
    items.emplace_back("Aged Brie", 5, 10);
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(11, app.items[0].quality);
}

TEST(GildedRoseTest, AgedBrieQualityIncreasesTwiceAfterSellIn) {
    vector<Item> items;
    items.emplace_back("Aged Brie", 0, 10);
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(12, app.items[0].quality);
}

TEST(GildedRoseTest, QualityNeverExceeds50) {
    vector<Item> items;
    items.emplace_back("Aged Brie", 5, 50);
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(50, app.items[0].quality);
}

TEST(GildedRoseTest, SulfurasNeverChanges) {
    vector<Item> items;
    items.emplace_back("Sulfuras, Hand of Ragnaros", 10, 80);
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(10, app.items[0].sellIn);
    EXPECT_EQ(80, app.items[0].quality);
}

TEST(GildedRoseTest, BackstagePassesQualityIncreasesByOne) {
    vector<Item> items;
    items.emplace_back("Backstage passes to a TAFKAL80ETC concert", 15, 20);
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(21, app.items[0].quality);
}

TEST(GildedRoseTest, BackstagePassesQualityIncreasesByTwoWithin10Days) {
    vector<Item> items;
    items.emplace_back("Backstage passes to a TAFKAL80ETC concert", 10, 20);
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(22, app.items[0].quality);
}

TEST(GildedRoseTest, BackstagePassesQualityIncreasesByThreeWithin5Days) {
    vector<Item> items;
    items.emplace_back("Backstage passes to a TAFKAL80ETC concert", 5, 20);
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(23, app.items[0].quality);
}

TEST(GildedRoseTest, BackstagePassesQualityDropsToZeroAfterConcert) {
    vector<Item> items;
    items.emplace_back("Backstage passes to a TAFKAL80ETC concert", 0, 20);
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(0, app.items[0].quality);
}

TEST(GildedRoseTest, BackstagePassesBoundaryAt6IsPlus2) {
    vector<Item> items;
    items.emplace_back("Backstage passes to a TAFKAL80ETC concert", 6, 20);
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(22, app.items[0].quality);
}

TEST(GildedRoseTest, BackstagePassesBoundaryAt11IsPlus1) {
    vector<Item> items;
    items.emplace_back("Backstage passes to a TAFKAL80ETC concert", 11, 20);
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(21, app.items[0].quality);
}

TEST(GildedRoseTest, BackstagePassesQualityCapAt50) {
    vector<Item> items;
    items.emplace_back("Backstage passes to a TAFKAL80ETC concert", 5, 49);
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(50, app.items[0].quality);
}
