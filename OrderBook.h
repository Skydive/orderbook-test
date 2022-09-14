#include <map>
// #include <tuple>

using namespace std;

// (from, to, price, quantity)
struct TradeExecuted {
    int from_id;
    int to_id;
    short price;
    int quantity;
};

struct Order {
    char type;
    int time;

    int id;
    short price;
    int quantity;
    int peak_id;

    void Print();
};

class OrderBook {
private:
    // (Sort by (price - short, time - int) 
    // 
    int time;
    map<pair<short, int>, Order> buy_orders;
    map<pair<short, int>, Order> sell_orders;
    
    // vector<TradeExecuted> transactions;
public:
    OrderBook() : time(0) {};
    void InsertLimitBuyOrder(int id, short price, int quantity);
    void InsertLimitSellOrder(int id, short price, int quantity, int peak_size);
    void Print();
};