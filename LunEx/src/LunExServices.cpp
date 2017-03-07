#include <stdlib.h>
#ifdef _WIN32
#  include <Windows.h>
#else
#  include <unistd.h>
#endif

#include "LunExServices.h"
#include "LunExServiceUnavailableException.h"


LunExServices::LunExServices() {
    // srand(0);
}

double LunExServices::currentPrice(const char* symbol) {
    (void)symbol;
    pauseToEmulateSendReceive();
    if (rand() % 100 > 80) {
        throw LunExServiceUnavailableException();
    }

    double currentPrice = 42.0 + rand() * 2.1;

    return truncate(currentPrice);
}

void LunExServices::pauseToEmulateSendReceive() {
    try {
#ifdef WIN32
        Sleep(5000);
#else
        sleep(5);
#endif
    } catch (std::exception& e) {
        // lucky client!
    }
}

double LunExServices::truncate(double original) {
    long y = (long) (original * 10000);
    return (double) y / 10000;
}
