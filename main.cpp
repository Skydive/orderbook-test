#include <iostream>

#include "OrderBook.h"

using namespace std;

inline void PrintIntro() {
    cout << "//==============================//" << endl
         << "//     Order Book Simulator     //" << endl
         << "//     12-09-22 --- K Aleem     //" << endl
         << "//==============================//" << endl;
}

int main() {
    OrderBook book;

    PrintIntro();

    // TODO: Convert into unit tests
        // Test buy/sell limit order matching, prices and priorities
        // Test matching of iceberg orders and on resolved
    // TODO: Code cin order processing
    // TODO: Figure out how to print iceberg orders
    book.InsertLimitSellOrder(0, 11, 100'000);
    book.InsertLimitSellOrder(1, 12,  50'000);
    book.InsertLimitSellOrder(2,  9,  25'000);
    book.Print();
    book.InsertLimitBuyOrder(0, 11, 50'000);
    book.Print();
    book.InsertLimitBuyOrder(0, 12, 100'000);
    book.Print();
    book.InsertLimitBuyOrder(0, 12, 50'000);
    book.InsertLimitBuyOrder(3, 12, 50'000);
    book.Print();

    book.InsertLimitSellOrder(4, 10, 50'000);
    book.Print();
    book.InsertLimitSellOrder(5, 10, 50'000);
    book.Print();

    book.InsertIcebergBuyOrder(1337, 12, 50'000, 20'000);
    book.Print();

    book.InsertLimitSellOrder(5, 10, 30'000);
    book.Print();

    book.InsertLimitBuyOrder(5, 10, 10'000);
    book.Print();

    book.InsertIcebergSellOrder(1338, 11, 60'000, 10'000);
    book.InsertIcebergSellOrder(1337,  9,  5'001,    500);
    book.Print();

    book.InsertLimitBuyOrder(0, 10,  5'500);
    book.Print();

    return 0;
}