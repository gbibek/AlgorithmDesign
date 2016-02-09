//
//  Algorithms.hpp
//  QuickSort
//
//  Created by Bibek Ghimire on 2/3/16.
//  Copyright © 2016 Bibek Ghimire. All rights reserved.
//

#ifndef Algorithms_hpp
#define Algorithms_hpp

#include <iostream>
#include <vector>
class Algorithm {
public:
    virtual void sort()=0;
};

class QuickSort: public Algorithm{
    std::vector<int> &my_array;
    int length;
    
public:

    QuickSort(std::vector<int> &array):my_array(array){
        length = (int)my_array.size();
    }
    
    virtual void sort(){
        std::cout <<"from sort()" <<std::endl;
        q_sort(0,1,length-1);
    }
    
    void swap(int a, int b){
        int tmp = my_array[a];
        my_array[a] = my_array[b];
        my_array[b]  = tmp;
    }
    
    void q_sort(int pivot, int left, int right){
        
        int splitpoint =  0;
        int length     =  right-pivot;
        
        
        if (left > right) {
            return;
        }
        while (left <= right) {
            while ((my_array[left] <= my_array[pivot]) && (left < (right+1)) ){
                left++;
            }
            while (my_array[right] > my_array[pivot]) {
                right--;
            }
            if(left < right){
                swap(left, right);
            }
            if ((left > right) && (pivot != (left-1))){
                splitpoint = right;
                swap(left-1, pivot);
            }
        }
        
        q_sort(pivot, (pivot+1), right-1);
        q_sort(left, left+1, ((left+(length - (right-pivot)))-1));
    }

};


class InsertionSort: public Algorithm{
public:
    virtual void sort(){
        std::cout << "Doing Insertion sort now " << std::endl;
    }
};

#endif /* Algorithms_hpp */
