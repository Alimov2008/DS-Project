#include <nlohmann/json.hpp>
#include <iostream>
using namespace std;
using json = nlohmann::json;

int main()
{
    json user={
        {"name","ali"},
        {"database","mysql"},
        {"skills",{"cpp","python","java"}}
    };

    cout<<user.dump(4);
}

