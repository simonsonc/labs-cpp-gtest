#include "StockQuote.h"

const double COMMISSION = 10.0;
const double SURCHARGE = 1.02;

StockQuote::StockQuote(std::string symbol, unsigned numberOfShares,
    ITC::SecurityExchangeTransmissionInterface* exchanged_p)
{
    double price = 0.0;
    try
    {
        price = exchanged_p->currentPrice( symbol.c_str() );
    }
    catch( const std::exception& e )
    {
        throw e;
    }

    m_total = calculateTotal( price, numberOfShares );
}

StockQuote::~StockQuote()
{
}

double StockQuote::calculateTotal(double price, unsigned int shares)
{
    return COMMISSION + SURCHARGE * price * shares;
}

double StockQuote::getTotal()
{
    return m_total;
}
