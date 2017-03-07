
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../src/StockQuote.h"
#include "../src/LunExServiceUnavailableException.h"

using ::testing::Return;
using ::testing::Throw;

class MockLunExServices : public ITC::SecurityExchangeTransmissionInterface {
public:
    MOCK_METHOD1(currentPrice, double(const char* symbol));
     //MockLunExServices(double price) : m_price( price), m_calls(0) {}
    /*double currentPrice(const char *symbol) override
    {
        ++m_calls;
        return m_price;
    }

    unsigned int m_calls;
    double m_price;*/

};

TEST(StockQuotes, getFullQuotePriceWhereSymbolExists) {
    MockLunExServices services;
    EXPECT_CALL(services, currentPrice(::testing::StrEq("ABC"))).WillOnce(Return(42.0));
    StockQuote quote("ABC", 1, &services);
    EXPECT_EQ(52.84, quote.getTotal());
}

TEST(StockQuotes, getFullQuotePriceWhereDiffSymbolExists) {
    MockLunExServices services;
    EXPECT_CALL(services, currentPrice(::testing::StrEq("EFC"))).WillOnce(Return(55.0));
    StockQuote quote("EFC", 1, &services);
    EXPECT_EQ(66.1, quote.getTotal());
}

TEST(StockQuotes, MultipleSharesABC) {
    MockLunExServices services;
    EXPECT_CALL(services, currentPrice(::testing::StrEq("ABC"))).WillOnce(Return(42.0));
    StockQuote quote("ABC", 2, &services);
    EXPECT_EQ(95.68, quote.getTotal());
}

TEST(StockQuotes, zeroCostResultsInCommissionTotal)
{
    MockLunExServices services;
    EXPECT_CALL(services, currentPrice(::testing::StrEq("zeroCost"))).WillOnce(Return(0.0));
    StockQuote quote("zeroCost", 1, &services);
    EXPECT_EQ(10.0, quote.getTotal());
}

TEST(StockQuotes, costOfOneGives2PercentAndCommission)
{
    MockLunExServices services;
    EXPECT_CALL(services, currentPrice(::testing::StrEq("oneCost"))).WillOnce(Return(1.0));
    StockQuote quote("oneCost", 1, &services);
    EXPECT_EQ(11.02, quote.getTotal());
}

TEST(StockQuotes, getTotalOnlyPingsServerOnce)
{
    MockLunExServices services;
    EXPECT_CALL(services, currentPrice(::testing::StrEq("oneCost"))).WillOnce(Return(1.0));
    StockQuote quote("oneCost", 1, &services);
    quote.getTotal();
    quote.getTotal();
    EXPECT_EQ(11.02, quote.getTotal());
}

TEST(StockQuotes, exceptionsArePassedUp)
{
    MockLunExServices services;
    EXPECT_CALL(services, currentPrice(::testing::StrEq("oneCost"))).WillOnce(Throw(LunExServiceUnavailableException()));
    EXPECT_THROW(StockQuote quote("oneCost", 1, &services), std::exception);
}

int main(int argc, char** argv)
{
    // run all tests
    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}
