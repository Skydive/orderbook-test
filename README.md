# Order Book Test


# What is the best way to design this?
If somebody want to buy a stock, they will transact with the lowest sell price. If somebody wants to sell a stock they will transact with the lowest buy price. Orders entering our order book later will have a lower priority.

Thus, we can use two multimap containers for our buy and sell orders, each using an underlying red-black tree implementation. We exploit the already-implemented comparator for the C++ `tuple<>` type to order first by  price ascending or descending, and then by priority. 
