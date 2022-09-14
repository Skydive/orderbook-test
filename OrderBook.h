#include <map>
// #include <tuple>

#include "Order.h"

using namespace std;
// (from, to, price, quantity)

class OrderBook {
private:
    // (Sort by (price - short, time - int) 
    // 
    int time;
    multimap<pair<short, int>, Order> buy_orders;
    multimap<pair<short, int>, Order> sell_orders;
    
    // vector<TradeExecuted> transactions;
public:
    OrderBook() : time(0) {};
    void InsertLimitBuyOrder(int id, short price, int quantity);
    void InsertLimitSellOrder(int id, short price, int quantity);
    void InsertIceBergBuyOrder(int id, short price, int quantity, int peak_size);
    void RegisterTransaction(int buy_id, int sell_id, short price, int quantity);
    void Print();
};