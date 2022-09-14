#include <iostream>

#include "OrderBook.h"

using namespace std;

inline void PrintIntro() {
    cout << "//==============================//" << endl
         << "//     Order Book Simulator     //" << endl
         << "//     12-09-22 --- K Aleem     //" << endl
         << "//==============================//" << endl;
}

int main(int argc, char **argv) {
    OrderBook book;

    PrintIntro();

    book.InsertLimitSellOrder(0, 11, 100, 0);
    book.InsertLimitSellOrder(1, 12,  50, 0);
    book.InsertLimitSellOrder(2,  9,  25, 0);
    book.Print();
    book.InsertLimitBuyOrder(0, 11, 100);
    book.Print();

    return 0;
}