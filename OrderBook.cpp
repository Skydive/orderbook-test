#include "OrderBook.h"

#include <climits>
#include <iostream>
#include <vector>
#include <algorithm> // std::sort

#include <unordered_map>

#include "util.h"

void OrderBook::MatchBuyOrder(int id, short price, int& quantity) {
    // TODO: Register a special transaction when we match against an iceberg
    // Maybe think about how to move away from the 'OnOrderResolved' methodology.
    // Look at PDF for how matching is done specifically

    // Group transactions
    // TODO: Account for order of grouping
    unordered_map<tuple<u64, int, short>, int, util::key_hash, util::key_equal> transaction_quantities, last_time_accessed;

    int i=0;
    auto it = sell_orders.begin();
    while(it != sell_orders.end() && (*it).second->price <= price && quantity > 0) {
        auto& [k, order] = *it;
        if(order->quantity > quantity) {
            // Buy order consumed entirely
            // RegisterTransaction(id, order->id, order->price, quantity);
            transaction_quantities[{order->uuid, order->id, order->price}] += quantity;
            last_time_accessed[{order->uuid, order->id, order->price}] = i;
            // Copy Iceberg if it's an Iceberg
            Order* new_order = order->Clone();
            // TODO: order.time or time(??)
            // TODO: Do we alter the priority or not? (ASK!?)
            new_order->time = order->time; 
            new_order->quantity -= quantity;
            AddSellOrder(new_order);
            quantity = 0;
            delete order;
            sell_orders.erase(it);
            break;
            
        } else { // order.quantity <= quantity
            // RegisterTransaction(id, order->id, order->price, order->quantity);
            transaction_quantities[{order->uuid, order->id, order->price}] += order->quantity;
            last_time_accessed[{order->uuid, order->id, order->price}] = i;

            quantity -= order->quantity;
            order->OnOrderResolved(*this);
            delete order;
            sell_orders.erase(it);
        }
        it = sell_orders.begin();
        i++;
    }

    vector<tuple<int, u64, int, short, int>> ordered_transactions;
    for(auto& [k, v] : last_time_accessed) {
        auto& [uuid, order_id, order_price] = k;
        ordered_transactions.push_back({
            v, uuid, order_id, order_price, transaction_quantities[k]
        });
    }
    sort(ordered_transactions.begin(), ordered_transactions.end());
    for(auto& x : ordered_transactions) {
        auto& [time, uuid, order_id, order_price, q] = x;
        RegisterTransaction(id, order_id, order_price, q);
    }
}

void OrderBook::MatchSellOrder(int id, short price, int& quantity) {
    map<tuple<u64, int, short>, int> transaction_quantities, last_time_accessed;

    int i=0;
    auto it = buy_orders.begin();
    while(it != buy_orders.end() && (*it).second->price >= price && quantity > 0) {
        auto& [k, order] = *it;
        if(order->quantity > quantity) {
            transaction_quantities[{order->uuid, order->id, order->price}] += quantity;
            last_time_accessed[{order->uuid, order->id, order->price}] = i;

            Order* new_order = order->Clone();
            // TODO: order.time or time(??)
            // TODO: Do we alter the priority or not? (ASK!?)
            new_order->time = order->time;
            new_order->quantity -= quantity;
            AddBuyOrder(new_order);
            quantity = 0;
            delete order;
            buy_orders.erase(it);
            break;
        } else {
            transaction_quantities[{order->uuid, order->id, order->price}] += order->quantity;
            last_time_accessed[{order->uuid, order->id, order->price}] = i;

            quantity -= order->quantity;
            order->OnOrderResolved(*this);
            delete order;
            buy_orders.erase(it);
        }
        it = buy_orders.begin();
        i++;
    }

    vector<tuple<int, u64, int, short, int>> ordered_transactions;
    for(auto& [k, v] : last_time_accessed) {
        auto& [uuid, order_id, order_price] = k;
        ordered_transactions.push_back({
            v, uuid, order_id, order_price, transaction_quantities[k]
        });
    }
    sort(ordered_transactions.begin(), ordered_transactions.end());
    for(auto& x : ordered_transactions) {
        auto& [time, uuid, order_id, order_price, q] = x;
        RegisterTransaction(order_id, id, order_price, q);
    }
}

void OrderBook::InsertLimitBuyOrder(int id, short price, int quantity) {
    // Check if a transaction can occur!
    // Find lowest someone is willing to sell below my bid price
    #ifdef DEBUG
        cerr << "COMMAND: BuyLimitOrder (" << id << ", " << price << ", " << quantity << ") " << endl;
    #endif
    MatchBuyOrder(id, price, quantity);

    if(quantity > 0) { 
        AddBuyOrder(new Order('B', time, id, price, quantity));
    }
    time++;
}

void OrderBook::InsertLimitSellOrder(int id, short price, int quantity) {
    // Find highest someone is willing to buy at above my ask price
    #ifdef DEBUG
        cerr << "COMMAND: SellLimitOrder (" << id << ", " << price << ", " << quantity << ") " << endl;;
    #endif
    MatchSellOrder(id, price, quantity);

    if(quantity > 0) { 
        AddSellOrder(new Order('S', time, id, price, quantity));
    }
    time++;
}

void OrderBook::InsertIcebergBuyOrder(int id, short price, int quantity, int peak_size) {
    #ifdef DEBUG
        cerr << "COMMAND: BuyIcebergOrder (" << id << ", " << price << ", " << quantity << ", " << peak_size << ") " << endl;
    #endif
    MatchBuyOrder(id, price, quantity);

    if(quantity > 0) { 
        auto* berg = new IcebergOrder('B', time, id, price, quantity, peak_size);
        #ifdef DEBUG
            cerr << "Inserting Iceberg: " << *berg << endl;
        #endif
        AddBuyOrder(berg);
    }
    time++;
}

void OrderBook::InsertIcebergSellOrder(int id, short price, int quantity, int peak_size) {
    #ifdef DEBUG
        cerr << "COMMAND: SellIcebergOrder (" << id << ", " << price << ", " << quantity << ", " << peak_size << ") " << endl;
    #endif
    MatchSellOrder(id, price, quantity);

    if(quantity > 0) { 
        auto* berg = new IcebergOrder('S', time, id, price, quantity, peak_size);
        #ifdef DEBUG
            cerr << "Inserting Iceberg: " << *berg << endl;
        #endif
        AddSellOrder(berg);
    }
    time++;
}

void OrderBook::RegisterTransaction(int buy_id, int sell_id, short price, int quantity) {
    // Be careful of Iceberg Orders! --> Only register the transaction when completed!
    cout << buy_id << "," << sell_id << "," << price << "," << quantity << endl;
}

void OrderBook::AddBuyOrder(Order* order) {
    buy_orders.insert({{-order->price, order->time}, (order)});
}

void OrderBook::AddSellOrder(Order* order) {
    sell_orders.insert({{order->price, order->time}, (order)});
}

OrderGroup& OrderBook::getBuyOrders() {
    return buy_orders;
}

OrderGroup& OrderBook::getSellOrders() {
    return sell_orders;
}

int OrderBook::getCurrentTime() const {
    return time;
}