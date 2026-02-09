#include <iostream>
#include <string>
#include "config/config.hpp"

using std::cout;
using std::endl;

using std::string;

int main() {
    Config config;

    config.New();
    config.Load();

    int v = config.Get<int>("asdf", 1);

    cout << v << endl;

    return 0;
}