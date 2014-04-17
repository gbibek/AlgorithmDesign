//
//  main.cpp
//  collaborative
//
//  Created by LSU on 4/15/14.
//  Copyright (c) 2014 LSU. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <cmath>

using namespace std;

typedef vector<int> Row;
typedef vector<Row> table;
table my_table(4,Row(4)) ;

double similarity(const vector<int> a_table,const vector<int> i_table,int size);
double cal_avg(const vector<int> compute_this_table,const vector<int> compare_this_table);
double paper_predict(const table,const vector<int>,int size,int location);
double class_predict();

int main(int argc, const char * argv[])
{
    string line;
    ifstream myfile ("/Users/lsu/Documents/workspace/collaborative/collaborative/data.txt");
    int i= 0,j=0,k=0;
    int location_of_vote_to_figure_out = 0;
    
    if (myfile.is_open()){
        while ( getline (myfile,line) )
        {
            if(line[j] != '#'){
                while(line[j] != '\0' ){
                    if(line[j] != ' '){
                        my_table[i][k] = line[j] - '0';
                        k++;
                    }j++;
                }j=0;k=0;i++;
            }
        }
        myfile.close();
    }else{
        cout << "Unable to open file";
    }
    for(int a =0;a <4;a++){
        for (int b =0; b <4;b++) {
            cout<<" "<<my_table[a][b]<<" ";
        }
        cout<<endl;
    }
    cout<<" Actually -3 means user have not voted for this item "<<endl;
    cout<<"Begin the prediction..."<<endl;
    cout<<paper_predict(my_table,my_table[3],(int)my_table[3].size(),location_of_vote_to_figure_out);
    cout<<endl;
    return 0;
}

double paper_predict(const table my_table,const vector<int> predict_this_user,int size,int location_of_vote_to_figure_out){
    
    double my_avg = 0.0;
    double avg_of_each_i = 0.0;
    int count = 0;
    double k = 0.0;
    double simi = 0.0;
    double numerator = 0.0;
    for (int i = 0; i < size; i++) {
        if(predict_this_user[i] > 0){
            my_avg = my_avg + predict_this_user[i];
            count++;
        }
    }
    my_avg = my_avg/count;
    cout<<"my_avg = "<<my_avg<<endl;
    
    for (int j = 0; j < my_table.size(); j++) {
        if(my_table[j][location_of_vote_to_figure_out]>0 && predict_this_user != my_table[j] ){
            simi = similarity(predict_this_user, my_table[j], size);
            k = k + abs(simi);
            avg_of_each_i = cal_avg(my_table[j],predict_this_user);
            cout<<"sim= " <<simi <<",average= "<<avg_of_each_i<<",";
            numerator = numerator + simi*(my_table[j][location_of_vote_to_figure_out]-avg_of_each_i);
            cout<< "numerator= "<<numerator<<",";
            cout<<"j= "<<my_table[j][location_of_vote_to_figure_out]<<endl;
        }
        
    }
    cout<<"numerator = "<<numerator<<endl;
    return (my_avg + (numerator/k));
}

double similarity(const vector<int> a_table,const vector<int> i_table,int size){
    
    int size_of_a = size ;
    double avg_of_a = cal_avg(a_table,i_table);
    double avg_of_i = cal_avg(i_table,a_table);
    double numerator = 0.0;
    double denominator1 = 0.0;
    double denominator2 = 0.0;
    for (int i =0; i<size_of_a;i++) {
        if(a_table[i] > 0 && i_table[i]>0){
            numerator = numerator+((a_table[i]-avg_of_a)*(i_table[i]-avg_of_i));
            denominator1  = denominator1 + ((a_table[i]-avg_of_a)*(a_table[i]-avg_of_a));
            denominator2  = denominator2 + ((i_table[i]-avg_of_i)*(i_table[i]-avg_of_i));
        }
    }
    return (numerator/sqrt(denominator1*denominator2));
}

double cal_avg(const vector<int> compute_this_table,const vector<int> compare_this_table){
    
    int size =(int) compute_this_table.size();
    double sum = 0.0;
    int count=0;
    for (int i=0; i<size; i++) {
        if(compute_this_table[i] > 0 && compare_this_table[i] > 0)
        {
            sum = compute_this_table[i]+sum;
            count++;
        }
    }
    return sum/count;
}


