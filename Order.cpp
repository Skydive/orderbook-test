#include "Order.h"
#include "OrderBook.h"

#include <iostream>
#include <random>

Order::Order(char type, int time, int id, short price, int quantity) 
    : type(type), time(time), id(id), price(price), quantity(quantity) {
    uuid = Order::GenerateUniqueU64();
}

Order::~Order() {}

IcebergOrder::IcebergOrder(char type, int time, int id, short price, int quantity, int peak_size)
    : Order(type, time, id, price, quantity),
      peak_size(peak_size), original_quantity(peak_size), iceberg_quantity(quantity) {
    this->original_quantity = min(quantity, peak_size);
    this->quantity = original_quantity;
};

IcebergOrder::~IcebergOrder() {}

ostream& operator<<(ostream& os, const Order& order) {
    os << order.type << " " << order.time << " " << order.id << " " << order.price << " " << order.quantity;
    return os;
}
ostream& operator<<(ostream& os, const IcebergOrder& order) {
    os << order.type << " " << order.time << " " << order.id << " " << order.price << " " << order.quantity << " ("
       << order.original_quantity << " " << order.iceberg_quantity << ")";
    return os;
}

u64 Order::GenerateUniqueU64() {
    static random_device rd;
    static mt19937_64 gen(rd());
    static uniform_int_distribution<u64> dis;
    return dis(gen);
}

Order* Order::Clone() {
    return new Order(*this);
}

void Order::OnOrderResolved(OrderBook& book) {
    (void)book; // for compiler...
}

Order* IcebergOrder::Clone() {
    return new IcebergOrder(*this);
}

void IcebergOrder::OnOrderResolved(OrderBook& book) {
    // Fragment order -- insert again into order book
    // cerr << "Iceberg Resolved: " << *this << endl;
    int new_iceberg_quantity = iceberg_quantity - original_quantity;
    // cerr << "New Quantity: " << new_iceberg_quantity << endl;
    if(new_iceberg_quantity > 0) { // Regenerate in peak_size...
        IcebergOrder* iceberg = dynamic_cast<IcebergOrder*>(this->Clone());
        iceberg->iceberg_quantity = new_iceberg_quantity;
        iceberg->original_quantity = (new_iceberg_quantity > peak_size) ? peak_size : new_iceberg_quantity;
        iceberg->quantity = iceberg->original_quantity;
        iceberg->time = book.getCurrentTime(); // Change priority of new iceberg order
        // cerr << "New Iceberg: " << *iceberg << endl;
        (this->type == 'B')
          ? book.AddBuyOrder(iceberg)
          : book.AddSellOrder(iceberg);
    }
}