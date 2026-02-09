#include <iostream>
#include <string>
#include "config/config.hpp"

using std::cout;
using std::endl;

using std::string;

int main() {
    Config& config = Config::getInstance();

    config.New();
    config.Load();

    int v = config.Get<int>("asdf", 1);

    cout << v << endl;

    return 0;
}