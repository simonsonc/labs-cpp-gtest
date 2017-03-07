#ifndef STOCK_QUOTE_HEADER
#define STOCK_QUOTE_HEADER

#include <string>
#include "LunExServices.h"

class StockQuote
{
public:
    StockQuote(std::string symbol, unsigned numberOfShares,
        ITC::SecurityExchangeTransmissionInterface* exchanged_p);
    ~StockQuote();

    double getTotal();
private:
    double calculateTotal(double price, unsigned int shares);

    double m_total = 0.0;
};

#endif // STOCK_QUOTE_HEADER
