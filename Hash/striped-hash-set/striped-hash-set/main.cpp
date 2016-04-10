//
//  main.cpp
//  striped-hash-set
//
//  Created by Екатерина Вишневская on 10.04.16.
//  Copyright © 2016 Екатерина Вишневская. All rights reserved.
//

#include<stdio.h>
#include<stdlib.h>
#include<cstdlib>
#include<string>
#include<vector>
#include<list>
#include<iostream>
#include<math.h>

using namespace std;

template <class T>
class hash_table{
private:
    vector<list<T>> table;
    int size;
    int m;
public:
    hash_table(int size_,int m_);
    hash_table(const hash_table& table);
    bool add(T el);
    bool remove(T el);
    bool contains(T el);
    int size1();
    friend ostream& operator <<(ostream& cout_, hash_table table1);
    ~hash_table();
};

template <class T>
hash_table<T>::hash_table(int size_,int m_){
    vector<list<string>> table_;
    size=size_;
    table_.reserve(size);
    table=table_;
    m=m_;
}

template <class T>
hash_table<T>::hash_table(const hash_table& table_)
{
    size=table_.size;
    table=table_.table;
}

template <class T>
bool hash_table<T>::add(T el){
    size_t h=std::hash(el);
    if(h>=size){
        table.resize(h+1);
        size= h+1;
        table[h].push_front(el);
        return true;
    }
    else{
        
        int i=0;
        if(table[h].empty()==true){
            table[h].push_front(el);
            return true;
        }
        else{
            list<string>::iterator str1;
            str1=table[h].begin();
            while(str1!=table[h].end()){
                if((*str1)==el){
                    return false;
                    break;
                }
                else{
                    str1++;
                }
            }
            table[h].push_front(el);
            return true;
        }
    }
}

template <class T>
bool hash_table<T>::remove(T el){
    int h=hash(el);
    list<string>::iterator str1;
    str1=table[h].begin();
    while(str1!=table[h].end()){
        if((*str1)==el){
            table[h].remove(el);
            return true;
            break;
        }
        else{
            str1++;
        }
    }
    return false;
}

template <class T>
bool hash_table<T>::contains(T el){
    int h=hash(el);
    cout << h << endl;
    auto el1=table[h].begin();
    while(el1!=table[h].end()){
        if((*el1)==el){
            return true;
            break;
        }
        else{
            el1++;
        }
    }
    return false;
    
}

template <class T>
int hash_table<T>::size1(){
    return table.capacity();
}
hash_table<T>::~hash_table(){
    for(size_t i=0; i<table.size(); i++){
        table[i].clear();
    }
    table.clear();
    size=0;
    m=0;
}



int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
