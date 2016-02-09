//
//  main.cpp
//  QuickSort
//
//  Created by Bibek Ghimire on 2/3/16.
//  Copyright © 2016 Bibek Ghimire. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <time.h>
#include "Algorithms.hpp"

class Sort{
private:
    Algorithm * obj;
public:
    
    
    Sort(Algorithm * _obj){
        obj = _obj;
    }
    void sort(){
        obj->sort();
    }
};


int main(int argc, const char * argv[]) {
    int number;
    size_t n=20;
    std::cout << "Size of array = " << n <<std::endl;
    std::vector<int> my_array;
    srand((unsigned)time(0));
    
    for(int i = 0; i < n; i++){
        number =(rand()%(n)) + 1;
        my_array.push_back(number);
    }
    
    Sort qobj(new QuickSort(my_array));
    
    qobj.sort();
    for (int i = 0; i < n; i++) {
        std::cout << "my_array[" << i << "] = " << my_array[i] << std::endl;
    }
    
    return 0;
}
