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
    void sort(Algorithm * _obj){
        obj = _obj;
        obj->sort();
    }
    void printa(const std::vector<int>& array){
        for (int i = 0; i < array.size(); i++) {
            std::cout << "my_array[" << i << "] = " << array[i] << std::endl;
        }
    }
};

int main(int argc, const char * argv[]) {
    int number;
    size_t n=20;
    std::vector<int> unsorted_array, my_array;
    Sort qobj;
    srand((unsigned)time(0));
   
    std::cout << "Size of array = " << n <<std::endl;
    for(int i = 0; i < n; i++){
        number =(rand()%(n)) + 1;
        unsorted_array.push_back(number);
    }

    my_array = unsorted_array;
        
    std::cout << "**** Doing QuickSort ****"  << std::endl; 
    qobj.sort(new QuickSort(my_array));
    qobj.printa(my_array); 

    my_array = unsorted_array;

    std::cout<<"**** Merge Sort ****" <<std::endl;
    qobj.sort(new MergeSort(my_array));
    qobj.printa(my_array);

    my_array = unsorted_array;

    std::cout << "**** InsertionSort ****" << std::endl; 
    qobj.sort(new InsertionSort(my_array)); 
    qobj.printa(my_array);
     
    return 0;
}
