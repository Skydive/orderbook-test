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


    return 0;
}