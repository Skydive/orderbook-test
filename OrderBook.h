#pragma once
#include <map>
#include <memory>
// #include <tuple>

#include "Order.h"

using namespace std;

typedef multimap<pair<short, int>, Order*> OrderGroup;

class OrderBook {
private:
    int time;
    // (Sort by (price - short, time - int) 
    OrderGroup buy_orders;
    OrderGroup sell_orders;
    
    // vector<TradeExecuted> transactions;
    void MatchBuyOrder(int id, short price, int& quantity);
    void MatchSellOrder(int id, short price, int& quantity);
public:
    OrderBook() : time(0) {};
    void InsertLimitBuyOrder(int id, short price, int quantity);
    void InsertLimitSellOrder(int id, short price, int quantity);
    void InsertIcebergBuyOrder(int id, short price, int quantity, int peak_size);
    void InsertIcebergSellOrder(int id, short price, int quantity, int peak_size);
    void RegisterTransaction(int buy_id, int sell_id, short price, int quantity);
    void AddBuyOrder(Order* order);
    void AddSellOrder(Order* order);
    OrderGroup& getBuyOrders();
    OrderGroup& getSellOrders();
    int getCurrentTime() const;
    void Print() const;
    friend ostream& operator<<(ostream& os, const Order& order);
};