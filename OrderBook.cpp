#include "OrderBook.h"

#include <climits>
#include <iostream>

void OrderBook::InsertLimitBuyOrder(int id, short price, int quantity) {
    // Check if a transaction can occur!
    // Find lowest someone is willing to sell below my bid price
    cout << "COMMAND: BuyLimitOrder (" << id << ", " << price << ", " << quantity << ") " << endl;
    auto it = sell_orders.begin();
    Print();
    while(it != sell_orders.end() && (*it).second.price <= price && quantity > 0) {
        cout << "Current Iterator: " << endl;
        cout << (*it).second << endl;

        cout << "Quantity:" << quantity << endl;

        auto [k, order] = *it;
        if(order.quantity > quantity) {
            // Remove quantity from order in OrderBook
            sell_orders.erase(it);
            order.quantity -= quantity;

            RegisterTransaction(id, order.id, order.price, quantity);
            // TODO: order.time or time(??)
            sell_orders.insert({{order.price, order.time}, order});
            quantity = 0;
            break;
            
        } else { // order.quantity <= quantity
            sell_orders.erase(it);
            quantity -= order.quantity;
            RegisterTransaction(id, order.id, order.price, order.quantity);
        }
        // Print();
        it = sell_orders.begin();
        // it = sell_orders.lower_bound({price+1, INT_MIN});
    }

    if(quantity > 0) { 
        // Insert a buy order
        buy_orders.insert({{-price, time}, Order('B', time, id, price, quantity)});
    }
    time++;
}

void OrderBook::InsertLimitSellOrder(int id, short price, int quantity) {
    // Find highest someone is willing to buy at above my ask price
    // auto it = buy_orders.lower_bound({price+1, INT_MIN});
    cout << "COMMAND: SellLimitOrder (" << id << ", " << price << ", " << quantity << ") " << endl;;
    auto it = buy_orders.begin();
    // Print();
    while(it != buy_orders.end() && (*it).second.price >= price && quantity > 0) {
        // cout << "Current Iterator: " << endl;
        // (*it).second.Print();
        // cout << endl;

        // cout << "Quantity:" << quantity << endl;

        auto [k, order] = *it;
        if(order.quantity > quantity) {
            // Remove quantity from order in OrderBook
            buy_orders.erase(it);
            order.quantity -= quantity;

            RegisterTransaction(order.id, id, order.price, quantity);
            // TODO: order.time or time(??)
            buy_orders.insert({{-order.price, order.time},  order});
            quantity = 0;
            break;
        } else { // order.quantity <= quantity
            buy_orders.erase(it);
            quantity -= order.quantity;
            RegisterTransaction(order.id, id, order.price, order.quantity);

        }
        // Print();
        it = buy_orders.begin();
    }

    if(quantity > 0) { 
        // Insert a buy order
        sell_orders.insert({{price, time}, Order('S', time, id, price, quantity)});
    }
    time++;
}

void OrderBook::RegisterTransaction(int buy_id, int sell_id, short price, int quantity) {
    // Be careful of Iceberg Orders! --> Only register the transaction when completed!
    cout << buy_id << "," << sell_id << "," << price << "," << quantity << endl;
}