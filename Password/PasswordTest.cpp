#include "gtest/gtest.h"

using namespace std;

class BaseRule
{
public:
    virtual bool check( const std::string& password ) = 0;
};

class LengthRule : BaseRule
{
public:
    LengthRule( int length ) : m_length( length ) {}

    bool check( const std::string& password )
    {
        return password.size() > m_length;
    }

private:
    int m_length;
};

class AlphaRule : public BaseRule
{
public:
    bool check( const std::string& password )
    {
        bool hasAlpha = false;
        for ( auto c : password )
        {
            hasAlpha = hasAlpha || isalpha(c);
        }
        return hasAlpha;
    }
};

class DigitRule : public BaseRule
{
public:
    bool check( const std::string& password )
    {
        bool hasDigit = false;
        for ( auto c : password )
        {
            hasDigit = hasDigit || isdigit(c);
        }
        return hasDigit;
    }
};

class HasSpecialRule : public BaseRule
{
public:
    bool check( const std::string& password )
    {
        bool hasSpecial = false;
        for ( auto c : password )
        {
            hasSpecial = hasSpecial || !isalnum(c);
        }
        return hasSpecial;
    }
};

class LastSpecialOrDigit : public BaseRule
{
public:
    bool check(const std::string &password)
    {
        if ( password.size() > 0 )
        {
            return !isalpha(password.at(password.size()-1));
        }
        else
        {
            return false;
        }
    }
};

class PasswordChecker
{
public:
    int min_len;
    bool too_short = false;
    bool noAlpha = true;
    bool noDigit = true;

    virtual bool acceptable( const std::string& password )
    {
        too_short = !LengthRule( min_len - 1 ).check( password );
        bool hasAlpha = AlphaRule().check( password );
        bool hasDigit = DigitRule().check( password );
        noAlpha = !hasAlpha;
        noDigit = !hasDigit;
        return !too_short && hasAlpha && hasDigit;
    }
};

class UserPasswordChecker : public PasswordChecker
{
public:
    UserPasswordChecker() {min_len = 8;}
};

class AdminPasswordChecker : public PasswordChecker
{
public:
    bool noSpecial = true;
    bool lastIsAlpha = false;

    AdminPasswordChecker() {min_len = 11;}

    bool acceptable( const std::string& password ) override
    {
        bool initial = PasswordChecker::acceptable( password );

        bool hasSpecial = HasSpecialRule().check( password );
        noSpecial = !hasSpecial;
        lastIsAlpha = !LastSpecialOrDigit().check( password );
        return initial && hasSpecial && !lastIsAlpha;
    }
};

class PasswordCheckerFixture : public ::testing::Test
{
public:
    UserPasswordChecker p;
};

TEST_F(PasswordCheckerFixture, LengthGreaterThan7) {
    EXPECT_TRUE(p.acceptable("a2345678"));
}
TEST_F(PasswordCheckerFixture, LengthLessThan8) {
    EXPECT_FALSE(p.acceptable("1234567"));
}

TEST_F(PasswordCheckerFixture, NotContainsOneAlphaChar) {
    EXPECT_FALSE(p.acceptable("12345678"));
}
TEST_F(PasswordCheckerFixture, ContainsOneAlphaChar) {
    EXPECT_TRUE(p.acceptable("a2345678"));
}

TEST_F(PasswordCheckerFixture, NotContainsOneDigit) {
    EXPECT_FALSE(p.acceptable("aaaaaaaa"));
}
TEST_F(PasswordCheckerFixture, ContainsOneDigit) {
    EXPECT_TRUE(p.acceptable("1asdfASdf"));
}

TEST_F(PasswordCheckerFixture, LongNoAlphaNoDigit) {
    EXPECT_FALSE(p.acceptable("!!!!!!!!!!!!!!"));
}
TEST_F(PasswordCheckerFixture, ReasonShortLength) {
    p.acceptable("a1!");
    EXPECT_FALSE(p.noAlpha);
    EXPECT_TRUE(p.too_short);
}
TEST_F(PasswordCheckerFixture, ReasonNoAlpha) {
    p.acceptable("1234567890");
    EXPECT_TRUE(p.noAlpha);
    EXPECT_FALSE(p.too_short);
}
TEST_F(PasswordCheckerFixture, ReasonNoDigit) {
    p.acceptable("asdfqwerzxcv");
    EXPECT_TRUE(p.noDigit);
}
TEST_F(PasswordCheckerFixture, TooShortWithAlpha) {
    p.acceptable("abcde");
    EXPECT_FALSE(p.noAlpha);
    EXPECT_TRUE(p.too_short);
}




class AdminPasswordCheckerFixture : public ::testing::Test
{
public:
    AdminPasswordChecker p;
};

TEST_F(AdminPasswordCheckerFixture, EmptyPassword) {
    EXPECT_FALSE(p.acceptable(""));
}

TEST_F(AdminPasswordCheckerFixture, LengthNotGreaterThan10) {
    EXPECT_FALSE(p.acceptable("a23456790!"));
}

TEST_F(AdminPasswordCheckerFixture, LengthGreaterThan10) {
    EXPECT_TRUE(p.acceptable("a23456789012345!"));
}

TEST_F(AdminPasswordCheckerFixture, NotContainsOneAlphaChar) {
    EXPECT_FALSE(p.acceptable("123456789012345!"));
}

TEST_F(AdminPasswordCheckerFixture, ContainsOneAlphaChar) {
    EXPECT_TRUE(p.acceptable("a23456789012345!"));
}

TEST_F(AdminPasswordCheckerFixture, NotContainsOneDigit) {
    EXPECT_FALSE(p.acceptable("aaaaaaaaaaaaaaaaaa!"));
}
TEST_F(AdminPasswordCheckerFixture, ContainsOneDigit) {
    EXPECT_TRUE(p.acceptable("1asdfASdf012345!"));
}

TEST_F(AdminPasswordCheckerFixture, LongNoAlphaNoDigit) {
    EXPECT_FALSE(p.acceptable("!!!!!!!!!!!!!!!!!!!!!!!!"));
}

TEST_F(AdminPasswordCheckerFixture, NotContainsSpecialChar) {
    EXPECT_FALSE(p.acceptable("a23456789012"));
}
TEST_F(AdminPasswordCheckerFixture, ContainsSpecialChar) {
    EXPECT_TRUE(p.acceptable("a23456789012!"));
}
TEST_F(AdminPasswordCheckerFixture, NotContainsLastSpecialOrNumber) {
    EXPECT_FALSE(p.acceptable("a23456789012!a"));
}
TEST_F(AdminPasswordCheckerFixture, ContainsLastSpecialOrNumber) {
    EXPECT_TRUE(p.acceptable("a23456789012!"));
}
TEST_F(AdminPasswordCheckerFixture, ReasonNoAlpha) {
    p.acceptable("1234567890123");
    EXPECT_TRUE(p.noAlpha);
    EXPECT_FALSE(p.too_short);
    EXPECT_TRUE(p.noSpecial);
}
TEST_F(AdminPasswordCheckerFixture, ReasonHasSpecial) {
    p.acceptable("12345678901!3");
    EXPECT_TRUE(p.noAlpha);
    EXPECT_FALSE(p.too_short);
    EXPECT_FALSE(p.noSpecial);
}

TEST_F(AdminPasswordCheckerFixture, LastIsAlpha) {
    p.acceptable("abcde");
    EXPECT_FALSE(p.noAlpha);
    EXPECT_TRUE(p.lastIsAlpha);
}


int main(int argc, char** argv)
{
    // run all tests
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
