//
//  main.cpp
//  netflix_collaboration
//
//  Created by LSU on 4/23/14.
//  Copyright (c) 2014 LSU. All rights reserved.
//


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <cmath>
#include <sstream>
#include <iomanip>

using namespace std;

struct item_rating{
    
    int item;
    int rating;
};


typedef vector<item_rating> Row;
typedef vector<Row> table;

vector<int> parsing(string);
void print(const table);
int count_votes(vector<int>);


void paper_predict_all(const table my_table, int active_user,
                       int active_users_item_to_predict_vote );
double simi_avg_simi(const vector<int> active_user,const vector<int> compare_to_user);

double cal_simi_avg(const vector<item_rating> compute_this_table,const vector<item_rating> compare_this_table);
double cal_avg(const vector<item_rating>);

int main(int argc, const char * argv[])
{
    string line;
    table my_table;
    ifstream myfile ("/Users/lsu/Documents/workspace/netflix_collaboration/netflix_collaboration/u1base.txt");
    item_rating obj;
    vector<int> dummy;
    int current_user = 1;
    Row my_row;
    cout<<"Begin .. "<<endl;
    
    if (myfile.is_open()){
        while ( getline (myfile,line))
        {
            if(parsing(line).size() > 0)
            {
                
                dummy = parsing(line);
                if (current_user == dummy[0]) {
                    obj.item = dummy[1];
                    obj.rating = dummy[2];
                    my_row.push_back(obj);
                }
                else{
                    my_table.push_back(my_row);
                    current_user = dummy[0];
                    my_row.clear();
                    obj.item = dummy[1];
                    obj.rating = dummy[2];
                    my_row.push_back(obj);
                    // cout<< " "<<dummy[0]<<endl;
                }
                
            }
            
        }
        my_table.push_back(my_row);
        myfile.close();
    }else{
        cout << "Unable to open file";
    }
    //print(my_table);
    
    return 0;
}
vector<int> parsing(string each_line){
    
    string dummy;
    dummy.clear();
    vector<int> my_row;
    int count = 0;
    
    for(int i = 0; i < each_line.length(); i++){
        if(isdigit(each_line[i]) && count <= 3){
            dummy += each_line[i];
        }else{
            
            if(isdigit(dummy[0]) && count <= 3){
                my_row.push_back(atoi(dummy.c_str()));
                count++;
            }
            dummy.clear();
        }
    }
    return my_row;
}

void print(table my_table){
    cout<<" the size of my_table = "<<my_table.size()<<endl;
    
    for (int i = 0 ; i < my_table.size(); i++) {
        cout<<"user "<<i+1<<"  = ";
        for(int j = 0; j < my_table[i].size(); j++){
            cout<<"(" <<my_table[i][j].item<< "),(" << my_table[i][j].rating<< ")   ";
        }
        cout<<endl;
    }
}
double simi_avg_simi(const vector<item_rating> active_user,const vector<item_rating> compare_to_user){
    
    double avg_of_active_user  = cal_avg(active_user);
    double avg_of_compare_user = cal_avg(compare_to_user);
    double numerator    = 0.0;
    double denominator1 = 0.0;
    double denominator2 = 0.0;
    int j = 0;
    for (int i = 0; i < active_user.size() || i < compare_to_user.size() ; i++) {
        
        if(active_user[i].item == compare_to_user[j].item){
            j++;
            numerator = numerator+((active_user[i].rating-avg_of_active_user)*(compare_to_user[j].rating-avg_of_compare_user));
            denominator1  = denominator1 + ((active_user[i].rating-avg_of_active_user)*(active_user[i].rating-avg_of_active_user));
            denominator2  = denominator2 + ((compare_to_user[j].rating-avg_of_compare_user)*(compare_to_user[j].rating-avg_of_compare_user));
        }
    }
    
    return (numerator/sqrt(denominator1*denominator2));
}


double cal_avg(const vector<item_rating> row_of_item){
    
    double sum = 0.0;
    for (int i = 0; i < row_of_item.size(); i++) {
        sum = sum + row_of_item[i].rating;
    }
    return sum/row_of_item.size();
}