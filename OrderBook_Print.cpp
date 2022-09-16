#include "OrderBook.h"

#include <iostream>
#include <sstream>
// For thousands comma separation
#include <locale>
// For pretty table printing
#include <iomanip>
#include <vector>

// Printing with commas in C++
// https://stackoverflow.com/questions/7276826/format-number-with-commas-in-c
class comma_numpunct : public std::numpunct<char> {
protected:
    virtual char do_thousands_sep() const {
        return ',';
    }
    virtual std::string do_grouping() const {
        return "\03";
    }
};

/*
    We seek to pretty print in the form below as follows:
+-----------------------------------------------------------------+
| BUY                            | SELL                           |
| Id       | Volume      | Price | Price | Volume      | Id       |
+----------+-------------+-------+-------+-------------+----------+
|1234567890|1,234,567,890| 32,503| 32,504|1,234,567,890|1234567891|
|      1138|        7,500| 31,502| 32,505|        7,777|      6808|
|          |             |       | 32,507|        3,000|     42100|
+-----------------------------------------------------------------+
*/
void OrderBook::Print() const {
    // Global Variable for comma locale
    static locale comma_locale(locale(), new comma_numpunct());

    // Top Border
    cout << "+" << string(65, '-') << "+" << endl;

    cout << "|" << left << setw(32) << " BUY" << "|" << left << setw(32) << " SELL" << "|" << endl;
    cout << "|" << left << setw(10) <<  " Id" << "|" << left << setw(13) << " Volume" << "|" << left << setw(7) << " Price" << "|"
                << left << setw(7) << " Price" << "|"<< left << setw(13) << " Volume" << "|" << left << setw(10) << " Id" << "|" << endl;
    cout << "+" << string(10, '-') << "+" << string(13, '-') << "+" << string(7, '-') << "+"
                << string(7, '-')  << "+" << string(13, '-') << "+" << string(10, '-') << "+" << endl;

    vector<tuple<int, int, int>> sorted_buy_orders;
    for(auto& [k, order] : buy_orders)
        sorted_buy_orders.push_back({order->id, order->quantity, order->price});
    vector<tuple<int, int, int>> sorted_sell_orders;
    for(auto& [k, order] : sell_orders)
        sorted_sell_orders.push_back({order->id, order->quantity, order->price});

    stringstream ss;
    ss.imbue(comma_locale);
    for(size_t i=0; i<max(sorted_buy_orders.size(),sorted_sell_orders.size()); i++) {
        cout << "|";
        if(i < sorted_buy_orders.size()) {
            auto& [id, volume, price] = sorted_buy_orders[i];
            cout << right << setw(10) << id << "|";
            ss.str("");
            ss << right << setw(13) << volume << "|";
            ss << right << setw(7) << price;
            cout << ss.str();
        } else {
            cout << string(10, ' ') << "|" << string(13, ' ') << "|" << string(7, ' ');
        }
        cout << "|";
        if(i < sorted_sell_orders.size()) {
            auto& [id, volume, price] = sorted_sell_orders[i];
            ss.str("");
            ss << right << setw(7) << price << "|";
            ss << right << setw(13) << volume << "|";
            cout << ss.str();
            cout << right << setw(10) << id;
        } else {
            cout << string(7, ' ') << "|" << string(13, ' ') << "|" << string(10, ' ');
        }
        cout << "|";
        cout << endl;
    }
    // Bottom Border
    cout << "+" << string(65, '-') << "+" << endl; 
}

ostream& operator<<(ostream& os, OrderBook& book) {
    cout << "Time: " << book.getCurrentTime() << endl;
    cout << "Type\tPrice\tQty\tID" << endl;
    auto sell_orders = book.getSellOrders();
    for(auto it = sell_orders.rbegin(); it != sell_orders.rend(); it++) {
        auto& [k, order] = *it;
        cout << *order;
    }
    auto& buy_orders = book.getBuyOrders();
    for(auto it = buy_orders.begin(); it != buy_orders.end(); it++) {
        auto& [k, order] = *it;
        cout << *order;
    }
    return os;
}