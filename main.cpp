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

    // PrintIntro();

    // TODO: Unit tests for priorities
    // TUDI: Unit test for limit order in between iceberg during matching 
    // TODO: Figure out how to print iceberg orders properly
    /*
A trade message should be sent to stdout for each matched trade. Note that in the 
technical specification it states that only a single trade message will be sent for each
iceberg order, even if the match occurs on more than one peak (e.g. a match size of
16,000 on an iceberg order with a peak size of 10,000 should trade once for a quantity
of 16,000, not one for 10,000 and another for 6,000).
    */

    // Process cin!
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