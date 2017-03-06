// SetTests.cpp : Defines the entry point for the console application.
//

#include "gtest/gtest.h"
#include "Set.h"

class SetTestFixture : public ::testing::Test 
{
public:
    virtual void SetUp()
    {
        // Setup stuff here per test
    }

    Set set;
};

TEST_F(SetTestFixture, InitializesToEmpty)
{
    EXPECT_TRUE(set.Empty());
}

TEST_F(SetTestFixture, DoesNotContain)
{
    EXPECT_FALSE(set.Contains("a"));
}

TEST_F(SetTestFixture, SetNotEmptyAfterAddingAnItem)
{
    set.Add("Foodbye");

    EXPECT_FALSE(set.Empty());
}

TEST_F(SetTestFixture, AddOneItem)
{
    set.Add("Foodbye");

    EXPECT_TRUE(set.Contains("Foodbye"));
}

TEST_F(SetTestFixture, AddDuplicateItems)
{
    set.Add("Foodbye");
    set.Add("Foodbye");

    EXPECT_EQ(1, set.Size());
}

TEST_F(SetTestFixture, NoEmptyStrings)
{
    EXPECT_THROW(set.Add(""), InvalidItemException);
}

TEST_F(SetTestFixture, Size)
{
    set.Add("Foodbye");
    EXPECT_EQ(1, set.Size());
}

TEST_F(SetTestFixture, SetUnion)
{
    Set set1;
    Set set2;

    set1.Add("A");
    set2.Add("B");

    Set set3 = set1.Union( set2 );

    EXPECT_EQ(2, set3.Size());
    EXPECT_TRUE(set3.Contains("A"));
    EXPECT_TRUE(set3.Contains("B"));
}

TEST_F(SetTestFixture, SetUnionOnEmptySet)
{
    Set set1;
    Set set2;

    set2.Add("B");

    Set set3 = set1.Union( set2 );

    EXPECT_EQ(1, set3.Size());
    EXPECT_TRUE(set3.Contains("B"));
}

TEST_F(SetTestFixture, SetIntersection)
{
    Set set1;
    Set set2;

    set1.Add("A");
    set1.Add("B");
    set2.Add("B");
    set2.Add("C");

    Set set3 = set1.Intersection( set2 );

    EXPECT_EQ(1, set3.Size());
    EXPECT_TRUE(set3.Contains("B"));
}

TEST_F(SetTestFixture, SetIntersectionOnEmptySet)
{
    Set set1;
    Set set2;

    set2.Add("B");
    set2.Add("C");

    Set set3 = set1.Intersection( set2 );

    EXPECT_TRUE(set3.Empty());
}

TEST_F(SetTestFixture, SetIntersectionWithNoProperIntersection)
{
    Set set1;
    Set set2;

    set1.Add("A");
    set2.Add("B");
    set2.Add("C");

    Set set3 = set1.Intersection( set2 );

    EXPECT_TRUE(set3.Empty());
}

TEST_F(SetTestFixture, SetsAreEqual)
{
    Set set1;
    Set set2;

    set1.Add("A");
    set2.Add("A");

    EXPECT_TRUE(set1.Equals(set2));
    EXPECT_TRUE(set2.Equals(set1));
}

TEST_F(SetTestFixture, SetsDifferentSizeNotEqual)
{
    Set set1;
    Set set2;

    set1.Add("A");
    set2.Add("A");
    set2.Add("B");

    EXPECT_FALSE(set1.Equals(set2));
    EXPECT_FALSE(set2.Equals(set1));
}

int main(int argc, char** argv)
{
	// run all tests
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
