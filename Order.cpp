#include "Order.h"

Order::Order(char type, int time, int id, short price, int quantity) 
    : type(type), time(time), id(id), price(price), quantity(quantity) {}

IcebergOrder::IcebergOrder(char type, int time, int id, short price, int quantity, int peak_size)
    : Order(type, time, id, price, quantity), peak_size(peak_size), original_quantity(quantity) {};

ostream& operator<<(ostream& os, const Order& order) {
    os << order.type << " " << order.time << " " << order.id << " " << order.price << " " << order.quantity;
    return os;
}