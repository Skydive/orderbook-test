# Order Book Test


# Guide
To build (requires GCC/Clang with c++17 support) [due to use of structured bindings]:

    >make clean release
     RM -Rf obj
     CC main.cpp
     CC OrderBook.cpp
     CC OrderBook_Print.cpp
     CC Order.cpp
     LD bin/IcebergExecutable

To run unit tests:

    >chmod +x tests.sh
    >./tests.sh
    Running tests...
    Running test: BuySellLimitOrders... Success!
    Running test: BuySellPriority... Success!
    Running test: IcebergOrdersTest... Success!
    Running test: LimitBetweenIceberg... Success!
    Running test: NormalOrder... Success!
    Running test: OrderIdFormat... Success!
    Running test: OrderPriceFormat... Success!
    Running test: OrderVolumeFormat... Success!
    Running test: OrderWithComment... Success!
    Running test: SingleTrade... Success!
    All tests completed successfully!

To execute (pipe in .cin file):

    >./bin/IcebergExecutable < tests/SingleTrade.cin
    +-----------------------------------------------------------------+
    | BUY                            | SELL                           |
    | Id       | Volume      | Price | Price | Volume      | Id       |
    +----------+-------------+-------+-------+-------------+----------+
    |         1|            2|      1|       |             |          |
    +-----------------------------------------------------------------+
    +-----------------------------------------------------------------+
    | BUY                            | SELL                           |
    | Id       | Volume      | Price | Price | Volume      | Id       |
    +----------+-------------+-------+-------+-------------+----------+
    |         1|            2|      1|      2|            1|         2|
    +-----------------------------------------------------------------+
    1,3,1,1
    +-----------------------------------------------------------------+
    | BUY                            | SELL                           |
    | Id       | Volume      | Price | Price | Volume      | Id       |
    +----------+-------------+-------+-------+-------------+----------+
    |         1|            1|      1|      2|            1|         2|
    +-----------------------------------------------------------------+

To execute (input from console):

    >./bin/IcebergExecutable
    >B,1337,1337,1337
    +-----------------------------------------------------------------+
    | BUY                            | SELL                           |
    | Id       | Volume      | Price | Price | Volume      | Id       |
    +----------+-------------+-------+-------+-------------+----------+
    |      1337|        1,337|  1,337|       |             |          |
    +-----------------------------------------------------------------+
    >S,1338,1337,1336
    1337,1338,1337,1336
    +-----------------------------------------------------------------+
    | BUY                            | SELL                           |
    | Id       | Volume      | Price | Price | Volume      | Id       |
    +----------+-------------+-------+-------+-------------+----------+
    |      1337|            1|  1,337|       |             |          |
    +-----------------------------------------------------------------+

# What is the best way to design this?
If somebody want to buy a stock, they will transact with the lowest sell price. If somebody wants to sell a stock they will transact with the highest buy price. Orders entering our order book later will have a lower priority.

Thus, we can use two multimap containers for our buy and sell orders, each using an underlying red-black tree implementation. We exploit the already-implemented comparator for the C++ `tuple<>` type to order first by price ascending or descending for sell and buy orders respectively, and then by priority.

# Implementing Iceberg Orders
It's possible to use polymorphism to capture the behaviour of iceberg orders. However, in order to do this, we need to use dynamic memory and appropriately clone `virtual Order::Clone()` our `Order` objects. As such, we implement an `void Order::OnOrderResolved(OrderBook& book)` method. This will dynamically regenerate our Iceberg order when it is consumed by our matching algorithm so the matching loop can continue (and perhaps match orders with a greater priority).

In order to group together the Iceberg order transaction messages during matching, we have separate functions for matching our incoming orders `OrderBook::MatchSellOrder/OrderBook::MatchBuyOrder`. Here, we sort all of our grouped transaction messages by the last time we processed each order transaction.

# Questions
- When an order in the book is partially matched by an incoming order, do we keep the same priority or decrease the priority in favor of other orders(?) [ASSUMED THAT WE KEEP THE SAME PRIORITY]
- Is our output print order the first time we match the iceberg order or the last time? What if there is a limit order in the middle? [ASSUMED IT IS THE LAST TIME]

# Tested using:
Desktop (Linux/NixOS): (GCC v11.3.0 x86_64)

    >g++ -v
    Using built-in specs.
    COLLECT_GCC=/nix/store/d7q0qfm12hb59wj63wyjs0hrdhmmapfz-gcc-11.3.0/bin/g++
    COLLECT_LTO_WRAPPER=/nix/store/d7q0qfm12hb59wj63wyjs0hrdhmmapfz-gcc-11.3.0/libexec/gcc/x86_64-unknown-linux-gnu/11.3.0/lto-wrapper
    Target: x86_64-unknown-linux-gnu
    Configured with: 
    Thread model: posix
    Supported LTO compression algorithms: zlib
    gcc version 11.3.0 (GCC)

Desktop (Linux/NixOS): (Clang v11.1.0 x86_64)

    >clang++ -v
    clang version 11.1.0
    Target: x86_64-unknown-linux-gnu
    Thread model: posix
    InstalledDir: /nix/store/p6dr62inbahm1q40xf4hl7l5p1ivghfj-clang-11.1.0/bin

Mobile Phone (Android/Termux): (Clang v14.0.6 aarch64)

    >g++ -v 
    clang version 14.0.6
    Target: aarch64-unknown-linux-android24
    Thread model: posix
    InstalledDir: /data/data/com.teremux/files/usr/bin

Windows Subsystem for Linux: (Ubuntu g++)
    >g++ -v
    Using built-in specs.
    COLLECT_GCC=g++
    COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/9/lto-wrapper
    OFFLOAD_TARGET_NAMES=nvptx-none:hsa
    OFFLOAD_TARGET_DEFAULT=1
    Target: x86_64-linux-gnu
    Configured with: ../src/configure -v --with-pkgversion='Ubuntu 9.4.0-1ubuntu1~20.04.1' --with-bugurl=file:///usr/share/doc/gcc-9/README.Bugs --enable-languages=c,ada,c++,go,brig,d,fortran,objc,obj-c++,gm2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-9 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-9-Av3uEd/gcc-9-9.4.0/debian/tmp-nvptx/usr,hsa --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
    Thread model: posix
    gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.1)
    >uname -r
    5.10.16.3-microsoft-standard-WSL2