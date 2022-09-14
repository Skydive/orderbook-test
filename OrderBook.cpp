#include "OrderBook.h"

#include <climits>
#include <iostream>

void OrderBook::InsertLimitBuyOrder(int id, short price, int quantity) {
    // Check if a transaction can occur!
    // Find lowest someone is willing to sell below my bid price
    cout << "COMMAND: BuyLimitOrder (" << id << ", " << price << ", " << quantity << ") " << endl;;
    auto it = sell_orders.begin();
    Print();
    while(it != sell_orders.end() && (*it).second.price <= price && quantity > 0) {
        cout << "Current Iterator: " << endl;
        (*it).second.Print();
        cout << endl;

        cout << "Quantity:" << quantity << endl;

        auto [k, order] = *it;
        if(order.quantity > quantity) {
            // Remove quantity from order in OrderBook
            sell_orders.erase(it);
            order.quantity -= quantity;
            // TODO: order.time or time(??)
            sell_orders[{order.price, order.time}] = order;
            
        } else { // order.quantity <= quantity
            sell_orders.erase(it);
            quantity -= order.quantity;
        }
        Print();
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