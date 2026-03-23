#include <gtest/gtest.h>

#include <algorithm>

#ifdef QUNITCONV_QT_TESTS
#include <QString>
#endif

#include "qaliasdictionary.h"

namespace {

template<class Container, class T>
bool contains(const Container &c, const T &value)
{
    return std::find(c.begin(), c.end(), value) != c.end();
}

}

template<typename T>
class QAliasDictionaryTest : public ::testing::Test {};

using StringTypes = ::testing::Types<
    std::string
#ifdef QUNITCONV_QT_TESTS
    , QString
#endif
>;
TYPED_TEST_SUITE(QAliasDictionaryTest, StringTypes);

TYPED_TEST(QAliasDictionaryTest, Basic)
{
    QAliasDictionary<TypeParam> dictionary;
    ASSERT_TRUE(dictionary.isEmpty());
    ASSERT_FALSE(dictionary.contains("name"));
    ASSERT_TRUE(dictionary.aliases("name").empty());

    dictionary.addAlias("name", "alias");
    dictionary.addAlias("name", "alias2");
    dictionary.addAlias("name", "alias3");
    dictionary.addAlias("name2", "alias_for_name2");
    dictionary.addAlias("name2", "alias2_for_name2");

    ASSERT_FALSE(dictionary.isEmpty());

    // name() resolves both names and aliases; returns default for unknown
    ASSERT_EQ(dictionary.name("name"), "name");
    ASSERT_EQ(dictionary.name("alias"), "name");
    ASSERT_EQ(dictionary.name("alias2"), "name");
    ASSERT_EQ(dictionary.name("alias3"), "name");
    ASSERT_EQ(dictionary.name("alias_for_name2"), "name2");
    ASSERT_EQ(dictionary.name("alias2_for_name2"), "name2");
    ASSERT_EQ(dictionary.name("unknown"), TypeParam{});

    // contains() matches names and aliases, but not unknown strings
    ASSERT_TRUE(dictionary.contains("name"));
    ASSERT_TRUE(dictionary.contains("alias"));
    ASSERT_FALSE(dictionary.contains("unknown"));

    // aliases() lists only direct aliases; returns empty for unknown or alias inputs
    auto aliases = dictionary.aliases("name");
    ASSERT_TRUE(contains(aliases, TypeParam{"alias"}));
    ASSERT_TRUE(contains(aliases, TypeParam{"alias2"}));
    ASSERT_TRUE(contains(aliases, TypeParam{"alias3"}));
    ASSERT_TRUE(dictionary.aliases("unknown").empty());
    ASSERT_TRUE(dictionary.aliases("alias").empty());

    // name() also returns the name itself when passed a name directly
    ASSERT_EQ(dictionary.name("name2"), "name2");

    // reassigning an alias to a different name overwrites the mapping
    dictionary.addAlias("name2", "alias");
    ASSERT_EQ(dictionary.name("alias"), "name2");

    // aliases() reflects the reassignment: removed from old name, added to new name
    ASSERT_FALSE(contains(dictionary.aliases("name"), TypeParam{"alias"}));
    ASSERT_TRUE(contains(dictionary.aliases("name2"), TypeParam{"alias"}));

    // clear() resets the dictionary entirely
    dictionary.clear();
    ASSERT_TRUE(dictionary.isEmpty());
    ASSERT_EQ(dictionary.name("name"), TypeParam{});
    ASSERT_EQ(dictionary.name("alias2"), TypeParam{});
    ASSERT_FALSE(dictionary.contains("name"));
    ASSERT_FALSE(dictionary.contains("alias2"));
}
