#include "hash.h"

int main(void){
    const std::string data ="Zaliczenie przedmiotu pd1";
    hash hash;
    hash.sha(data);
    hash.print_hash();
    return 0;
}