#include "OrderBook.h"

#include <climits>
#include <iostream>

void OrderBook::InsertLimitBuyOrder(int id, short price, int quantity) {
    // Check if a transaction can occur!
    // Find lowest someone is willing to sell below my bid price
    cout << "COMMAND: BuyLimitOrder (" << id << ", " << price << ", " << quantity << ") " << endl;;
    auto it = sell_orders.begin();
    Print();
    // string dummy; cin >> dummy;
    while(it != sell_orders.end() && (*it).second.price <= price && quantity > 0) {
        cout << "Current Iterator: " << endl;
        (*it).second.Print();
        cout << endl;

        cout << "Quantity:" << quantity << endl;

        auto [k, order] = *it;
        if(order.quantity > quantity) {
            
            sell_orders.erase(it);
            order.quantity -= quantity;
            // TODO: order.time or time(??)
            sell_orders[{order.price, order.time}] = order;
            
        } else { // order.quantity <= quantity
            sell_orders.erase(it);
            quantity -= order.quantity;
        }
        Print();
        // string dummy; cin >> dummy;
        it = sell_orders.begin();
        // it = sell_orders.lower_bound({price+1, INT_MIN});
    }

    if(quantity > 0) { 
        // Insert a buy order
        buy_orders[{-price, time}] = Order{'B', time, id, price, quantity, 0};
    }

    time++;
}

void OrderBook::InsertLimitSellOrder(int id, short price, int quantity, int peak_size) {
    // Is someone buying at or above my ask price
    // auto it = buy_orders.lower_bound({price+1, INT_MIN});
    cout << "COMMAND: SellLimitOrder (" << id << ", " << price << ", " << quantity << ") " << endl;;

    // (Sort by price, then by priority)
    sell_orders[{price, time}] = Order{'S', time, id, price, quantity, peak_size};
    time++;
}


#include <iomanip>
#include <vector>
/*

+-----------------------------------------------------------------+
|      BUY | SELL |
|       Id |      Volume | Price | Price | Volume | Id |
+----------+-------------+-------+-------+-------------+----------+
|1234567890|1,234,567,890| 32,503| 32,504|1,234,567,890|1234567891|
| 1138| 7,500| 31,502| 32,505| 7,777| 6808|
| | | | 32,507| 3,000| 42100|
+-----------------------------------------------------------------+
*/

void OrderBook::Print() {
    // Top Border
    cout << "+" << string(65, '-') << "+" << endl;

    cout << "|" << left << setw(32) << " BUY" << "|" << left << setw(32) << " SELL" << "|" << endl;
    cout << "|" << left << setw(10) <<  " Id" << "|" << left << setw(13) << " Volume" << "|" << left << setw(7) << " Price" << "|"
                << left << setw(7) << " Price" << "|"<< left << setw(13) << " Volume" << "|" << left << setw(10) << " Id" << "|" << endl;
    cout << "+" << string(10, '-') << "+" << string(13, '-') << "+" << string(7, '-') << "+"
                << string(7, '-')  << "+" << string(13, '-') << "+" << string(10, '-') << "+" << endl;

    vector<tuple<int, int, int>> sorted_buy_orders;
    for(auto& [k, order] : buy_orders)
        sorted_buy_orders.push_back({order.id, order.quantity, order.price});
    vector<tuple<int, int, int>> sorted_sell_orders;
    for(auto& [k, order] : sell_orders)
        sorted_sell_orders.push_back({order.id, order.quantity, order.price});
    for(int i=0; i<max(sorted_buy_orders.size(),sorted_sell_orders.size()); i++) {
        cout << "|";
        if(i < sorted_buy_orders.size()) {
            auto& [id, volume, price] = sorted_buy_orders[i];
            cout << right << setw(10) << id << "|";
            cout << right << setw(13) << volume << "|";
            cout << right << setw(7) << price;
        } else {
            cout << string(10, ' ') << "|" << string(13, ' ') << "|" << string(7, ' ');
        }
        cout  << "|";
        if(i < sorted_sell_orders.size()) {
            auto& [id, volume, price] = sorted_sell_orders[i];
            cout << right << setw(7) << price << "|";
            cout << right << setw(13) << volume << "|";
            cout << right << setw(10) << id;
        } else {
            cout << string(10, ' ') << "|" << string(13, ' ') << "|" << string(7, ' ');
        }
        cout << "|";
        cout << endl;
    }


    // Bottom Border
    cout << "+" << string(65, '-') << "+" << endl;
    
    // cout << "Time: " << time << endl;
    // cout << "Type\tPrice\tQty\tID" << endl;
    // for(auto it = sell_orders.rbegin(); it != sell_orders.rend(); it++) {
    //     auto [k, order] = *it;
    //     order.Print();
    // }

    // for(auto it = buy_orders.begin(); it != buy_orders.end(); it++) {
    //     auto [k, order] = *it;
    //     order.Print();
    // }
    
    // vector<tuple<int, int, int>> sell_orders;
}

void Order::Print() {
    cout << type << "\t" << price << "\t" << quantity << "\t" << id << endl;//"\t" << time << "\t" << peak_id << endl;
}