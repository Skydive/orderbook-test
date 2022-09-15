#include "OrderBook.h"

#include <climits>
#include <iostream>

void OrderBook::InsertLimitBuyOrder(int id, short price, int quantity) {
    // Check if a transaction can occur!
    // Find lowest someone is willing to sell below my bid price
    cout << "COMMAND: BuyLimitOrder (" << id << ", " << price << ", " << quantity << ") " << endl;
    auto it = sell_orders.begin();
    // Print();

    // TODO: Split matching parts into seperate functions
    // Register a special transaction when we match against an iceberg
    // Think about how to move away from the 'OnOrderResolved' methodology.
    // Look at PDF for how matching is done specifically
    while(it != sell_orders.end() && (*it).second->price <= price && quantity > 0) {
        // cout << "Current Iterator: " << endl;
        // cout << (*it).second << endl;

        // cout << "Quantity:" << quantity << endl;

        auto& [k, order] = *it;
        if(order->quantity > quantity) {
            // Buy order consumed entirely
            RegisterTransaction(id, order->id, order->price, quantity);

            // TODO: order.time or time(??)
            Order* new_order = order->Clone(); // Copy Iceberg if it's an Iceberg
            new_order->time = time; // Alter priority
            new_order->quantity -= quantity;
            AddSellOrder(new_order);
            quantity = 0;
            delete order;
            sell_orders.erase(it);
            break;
            
        } else { // order.quantity <= quantity
            RegisterTransaction(id, order->id, order->price, order->quantity);

            quantity -= order->quantity;
            order->OnOrderResolved(*this);
            delete order;
            sell_orders.erase(it);
        }
        // Print();
        it = sell_orders.begin();
        // it = sell_orders.lower_bound({price+1, INT_MIN});
    }

    if(quantity > 0) { 
        AddBuyOrder(new Order('B', time, id, price, quantity));
    }
    time++;
}

void OrderBook::InsertLimitSellOrder(int id, short price, int quantity) {
    // Find highest someone is willing to buy at above my ask price
    cout << "COMMAND: SellLimitOrder (" << id << ", " << price << ", " << quantity << ") " << endl;;
    auto it = buy_orders.begin();
    // Print();
    while(it != buy_orders.end() && (*it).second->price >= price && quantity > 0) {
        // cout << "Current Iterator: " << endl;
        // (*it).second.Print();
        // cout << endl;

        // cout << "Quantity:" << quantity << endl;

        auto& [k, order] = *it;
        if(order->quantity > quantity) {
            // Sell order consumed entirely
            // Remove quantity from order in OrderBook
            RegisterTransaction(order->id, id, order->price, quantity);
            // TODO: order.time or time(??)

            Order* new_order = order->Clone();
            new_order->time = time;
            new_order->quantity -= quantity;
            AddBuyOrder(new_order);
            quantity = 0;
            delete order;
            buy_orders.erase(it);
            break;
        } else { // order.quantity <= quantity
            RegisterTransaction(order->id, id, order->price, order->quantity);

            quantity -= order->quantity;
            order->OnOrderResolved(*this);
            delete order;
            buy_orders.erase(it);
        }
        // Print();
        it = buy_orders.begin();
    }

    if(quantity > 0) { 
        AddSellOrder(new Order('S', time, id, price, quantity));
    }
    time++;
}

void OrderBook::InsertIcebergBuyOrder(int id, short price, int quantity, int peak_size) {
    cout << "COMMAND: BuyIcebergOrder (" << id << ", " << price << ", " << quantity << ", " << peak_size << ") " << endl;
    auto it = sell_orders.begin();
    while(it != sell_orders.end() && (*it).second->price <= price && quantity > 0) {
        auto& [k, order] = *it;
        if(order->quantity > quantity) {
            // Iceberg order consumed entirely
            RegisterTransaction(id, order->id, order->price, quantity);

            Order* new_order = order->Clone(); // Copy Iceberg if it's an Iceberg
            new_order->time = time; // Alter priority
            new_order->quantity -= quantity;
            AddSellOrder(new_order);
            quantity = 0;
            delete order;
            sell_orders.erase(it);
            break;
            
        } else { // order.quantity <= quantity
            // TODO: Is this valid - how do we record an Iceberg...
            RegisterTransaction(id, order->id, order->price, order->quantity);

            quantity -= order->quantity;
            order->OnOrderResolved(*this);
            delete order;
            sell_orders.erase(it);
        }
        it = sell_orders.begin();
    }

    if(quantity > 0) { 
        auto* berg = new IcebergOrder('B', time, id, price, quantity, peak_size);
        cout << "Inserting Iceberg: " << *berg << endl;
        AddBuyOrder(berg);
    }
    time++;
}

void OrderBook::InsertIcebergSellOrder(int id, short price, int quantity, int peak_size) {
    cout << "COMMAND: SellIcebergOrder (" << id << ", " << price << ", " << quantity << ", " << peak_size << ") " << endl;
    auto it = buy_orders.begin();
    while(it != buy_orders.end() && (*it).second->price <= price && quantity > 0) {
        auto& [k, order] = *it;
        if(order->quantity > quantity) {
            // Iceberg order consumed entirely
            RegisterTransaction(order->id, id, order->price, quantity);

            Order* new_order = order->Clone(); // Copy Iceberg if it's an Iceberg
            new_order->time = time; // Alter priority
            new_order->quantity -= quantity;
            AddBuyOrder(new_order);
            quantity = 0;
            delete order;
            buy_orders.erase(it);
            break;
            
        } else { // order.quantity <= quantity
            // TODO: Is this valid - how do we record an Iceberg...
            RegisterTransaction(order->id, id, order->price, order->quantity);

            quantity -= order->quantity;
            order->OnOrderResolved(*this);
            delete order;
            buy_orders.erase(it);
        }
        it = buy_orders.begin();
    }

    if(quantity > 0) { 
        auto* berg = new IcebergOrder('S', time, id, price, quantity, peak_size);
        cout << "Inserting Iceberg: " << *berg << endl;
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