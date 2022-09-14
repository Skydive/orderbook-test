#include <sstream>

using namespace std;

class Order {
public:
    char type;
    int time;
    int id;
    short price;
    int quantity;
    Order(char type, int time, int id, short price, int quantity);
    friend ostream& operator<<(ostream& os, const Order& order);
};

class IcebergOrder : public Order {
public:
    int peak_size;
    int original_quantity;
    IcebergOrder(char type, int time, int id, short price, int quantity, int peak_size);
};