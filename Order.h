#pragma once
#include <sstream>

class OrderBook;

using namespace std;

#include <cstdint>
typedef uint64_t u64;

// TODO: Code getters/setters for Order object parameters
class Order {
private:
    static u64 GenerateUniqueU64();
public:
    u64 uuid;
    char type;
    int time;
    int id;
    short price;
    int quantity;
    Order(char type, int time, int id, short price, int quantity);
    virtual ~Order();
    virtual Order* Clone();
    virtual void OnOrderResolved(OrderBook& book);
    friend ostream& operator<<(ostream& os, const Order& order);
};

class IcebergOrder : public Order {
public:
    int peak_size;
    int original_quantity;
    int iceberg_quantity;
    IcebergOrder(char type, int time, int id, short price, int quantity, int peak_size);
    virtual ~IcebergOrder();
    virtual Order* Clone() override;
    virtual void OnOrderResolved(OrderBook& book) override;
    friend ostream& operator<<(ostream& os, const IcebergOrder& order);
};