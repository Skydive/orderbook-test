#include <iostream>
#include <sstream>
#include <array>

#include "OrderBook.h"
#include "util.h"

using namespace std;

inline void PrintIntro() {
    cout << "//==============================//" << endl
         << "//     Order Book Simulator     //" << endl
         << "//     12-09-22 --- K Aleem     //" << endl
         << "//==============================//" << endl;
}

int main() {
    OrderBook book;

    string s;
    int line = 0;
    while(getline(cin, s)) {
        line++;
        // Remove whitespace
        util::trim(s);
        if(s.size() == 0)continue; // Whitespace line
        if(s[0] == '#') continue; // Comment line
        // ASCII Line: Split by commas
        vector<string> command;

        // Split string by commas
        istringstream ss(s);
        string token;
        while(getline(ss, token, ',')) {
            command.push_back(token);
        }
    
        if(command.size() >= 4) {
            char type = (char)command[0][0];
            int id = stoi(command[1]);
            short price = (short)stoi(command[2]);
            int quantity = stoi(command[3]);
            if(command.size() == 4) { // Limit Order
                (type == 'B')
                  ? book.InsertLimitBuyOrder(id, price, quantity)
                  : book.InsertLimitSellOrder(id, price, quantity);
            } else if(command.size() == 5) { // Iceberg Order
                int peak_size = stoi(command[4]);
                (type == 'B')
                  ? book.InsertIcebergBuyOrder(id, price, quantity, peak_size)
                  : book.InsertIcebergSellOrder(id, price, quantity, peak_size);
            } else {
                cerr << "ERROR: Malformed command! Line: " << line << endl;
                cerr << "Problem: Too many arguments: " << command.size() << endl;
                cerr << "Data: " << s << endl;
                return 1;
            }
        } else {
            cerr << "ERROR: Malformed command! Line: " << line << endl;
            cerr << "Problem: Too few arguments: " << command.size() << endl;
            cerr << "Data: " << s << endl;
            return 1;
        }
        book.Print();
    }
    

    return 0;
}