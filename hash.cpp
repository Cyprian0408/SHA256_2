#include "hash.h"
#include <vector>
#include <array>
#include <string>
#include <bits/stdc++.h>
#include "boost/multi_array.hpp"
#include <bit>
#include <numeric>
#include <algorithm>
#include <bitset>

//funkcje pomocnicze konieczne do zrobienia XOR między dwoma stringami 
std::bitset<32> to_bitset(std::string s)
{
    auto binary = [](char c) { return c == '0' || c == '1'; };
    auto not_binary = [binary](char c) { return !binary(c); };

    s.erase(std::remove_if(begin(s), end(s), not_binary), end(s));

    return std::bitset<32>(s);
}
std::string to_string(std::bitset<32> bs)
{
    return bs.to_string();
}



void hash::print_hash(){
    for (int i=0; i<_hash.size();i++){
        std::cout<<_hash[i];
    }
    std::cout<<std::endl;
    std::cout<<_hash.size()<<std::endl;
}
void leftrotate(std::string &s, int d){
    reverse(s.begin(), s.begin()+d);
    reverse(s.begin()+d, s.end());
    reverse(s.begin(), s.end());
}
void rightrotate(std::string &s, int d)
{
   leftrotate(s, s.length()-d);
}

std:: string stringShift(std::string s, std::vector<std::vector<int> >& shift)
{
 
    int val = 0;
 
    for (int i = 0; i < shift.size(); ++i)
 
        // If shift[i][0] = 0, then left shift
        // Otherwise, right shift
        val += shift[i][0] == 0
                   ? -shift[i][1]
                   : shift[i][1];
 
    // Stores length of the string
    int len = s.length();
 
    // Effective shift calculation
    val = val % len;
 
    // Stores modified string
    std::string result = "";
 
    // Right rotation
    if (val > 0)
        result = s.substr(len - val, val)
                 + s.substr(0, len - val);
 
    // Left rotation
    else
        result
            = s.substr(-val, len + val)
              + s.substr(0, -val);
    return result;
}

void hash::sha( const std::string &data){
    std::vector<std::string> bits_string;
    std::string bits;
    //binary representation of string data
    for (int i=0; i<data.size(); i++){
        bits=std::bitset<8>(data[i]).to_string();
        _hash.push_back(bits);
    }
    int size_64=_hash.size();
    //appending one at the end of binary string
    _hash.push_back("1");
    //filling with zeros until vector size is a multiple of 512 less 64
    int counter=1;
    while (true){
        if (_hash.size()<counter*512){
            break;
        }
        counter++;
    }
    int size=_hash.size();
    for (int i=0; i<counter*512-size-64;i++){
        _hash.push_back("0");
    }

    //appending the last 64 bits, which is size of _hash in binary
    int append_index=_hash.size();
    std::cout<<_hash.size()<<std::endl;
    bits=std::bitset<64>(size_64).to_string();
    std::cout<<bits<<std::endl;
    _hash.resize(counter*512-1);
    _hash.push_back(bits);

    //adding 48 32-bit words initialized to zero
    int current_size=_hash.size();
    std::cout<<"Obecny rozmiar to\t"<<current_size<<std::endl;
    int counter_1=48*32;
    for (int i=0; i<counter_1;i++){
        _hash.push_back("0");
    }
    std::cout<<"Obecny rozmiar1 to:\t"<<current_size+48*32<<std::endl;

    //performing operations on 32-bit words made from _hash vector
    const int size_of_array=_hash.size()/32;
    std::cout<<size_of_array<<std::endl;
    const int cols=32;
    std::cout<<"Creating multidimensional array"<<std::endl;
    boost::multi_array<std::string,2> hash_array{boost::extents[size_of_array][4]};
    std::cout<<"Entering loop"<<std::endl;
    for (int j=0; j<size_of_array;j++){
        for(int i=0; i<4;i++){
            std:: string temp=_hash[i];
            hash_array[j][i]=temp;
        }
    }
    for(int i=0; i<4;i++){
        std::cout<<hash_array[0][i]<<" ";
    }
    std::cout<<std::endl;
    //modifying rows from 16 to 63
    //creating a vector of values from the array
    std::vector<std::string>vector_64;
    for(int i=0; i<64;i++){
        std:: string temp=hash_array[i][0];
        std:: string temp1=hash_array[i][1];
        std:: string temp2=hash_array[i][2];
        std:: string temp3=hash_array[i][3];
        std:: string row=temp+temp1+temp2+temp3;
        vector_64.push_back(row);
    }
    std::cout<<vector_64[0]<<std::endl;
    std::vector<std::vector<int> > shift = {
        { 0, 0 },
        { 1, 3}
    };
    std::vector<std::vector<int> > shift2 = {
        { 0, 0 },
        { 1, 10 }
    };
    for (int i=16;i<64;i++){
        std::string temp=vector_64[i-15];
        std::string temp1=vector_64[i-15];
        std::string temp2=vector_64[i-15];
        std::string temp3=vector_64[i-2];
        std::string temp4=vector_64[i-2];
        std::string temp5=vector_64[i-2];
        std::string s0;
        std::string s1;
        rightrotate(temp,7);
        rightrotate(temp1,18);
        temp2=stringShift(temp2, shift);
        rightrotate(temp3,17);
        rightrotate(temp4,19);
        temp5=stringShift(temp5,shift2);
        s0=to_string(to_bitset(temp)^to_bitset(temp1)^to_bitset(temp2));
        s1=to_string(to_bitset(temp3)^to_bitset(temp4)^to_bitset(temp5));
        vector_64[i].clear();
        //vector_64[i]=vector_64[i-16]+s0+vector_64[i-7]+s1;
    }
    //performing compression
    std::string a=hash::hash_array[0];
    std::string b=hash::hash_array[1];
    std::string c=hash::hash_array[2];
    std::string d=hash::hash_array[3];
    std::string e=hash::hash_array[4];
    std::string f=hash::hash_array[5];
    std::string g=hash::hash_array[6];
    std::string h=hash::hash_array[7];
    for (int i=0; i<64; i++){
        std::string temp=e;
        std::string temp1=e;
        std::string temp2=e;
        std::string temp3=a;
        std::string temp4=a;
        std::string temp5=a;
        std::string S1;
        std::string S0;
        rightrotate(temp,6);
        rightrotate(temp1,11);
        rightrotate(temp2,25);
        S1=to_string(to_bitset(temp)^to_bitset(temp1)^to_bitset(temp2));
        rightrotate(temp3,2);
        rightrotate(temp4,13);
        rightrotate(temp5,22);
        std::string ch=to_string((to_bitset(e)&to_bitset(f))^(~(to_bitset(e))&to_bitset(g)));
        //std::string help1=h+S1+ch+hash::hash_constants[i]+vector_64[i];
        S0=to_string(to_bitset(temp3)^to_bitset(temp4)^to_bitset(temp5));
        std::string maj=to_string((to_bitset(a)&to_bitset(b))^(to_bitset(a)&to_bitset(c))^(to_bitset(b)&to_bitset(c)));
        //std::string help2=S0+maj;
        h=g;
        g=f;
        f=e;
        //e=d+help1;
        d=c;
        c=b;
        b=a;
        //a=help1+help2;
    }
    //hash::hash_array[0]=hash::hash_array[0]+a;
    //hash::hash_array[1]=hash::hash_array[1]+b;
    //hash::hash_array[2]=hash::hash_array[2]+c;
    //hash::hash_array[3]=hash::hash_array[3]+d;
    //hash::hash_array[4]=hash::hash_array[4]+e;
    //hash::hash_array[5]=hash::hash_array[5]+f;
    //hash::hash_array[6]=hash::hash_array[6]+g;
    //hash::hash_array[7]=hash::hash_array[7]+h;
    std::cout<<"Ostatecznie"<<std::endl;
    std::cout<<hash::hash_array[1]<<std::endl;

}



