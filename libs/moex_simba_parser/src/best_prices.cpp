#include "moex/orders/best_prices.h"


BestPriceisOrderPayload::BestPriceisOrderPayload(BufferedReader &reader) {

}

void BestPriceisOrderPayload::parse(BufferedReader &reader) {

}

std::ostream &operator<<(std::ostream &os, const BestPriceisOrderPayload &header) {
    return os;
}

BestPriceisOrder::BestPriceisOrder(BufferedReader &reader) {

}

void BestPriceisOrder::parse(BufferedReader &reader) {

}

std::ostream &operator<<(std::ostream &os, const BestPriceisOrder &header) {
    // TODO to string
    return os;
}
