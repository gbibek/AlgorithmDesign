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
#include <sstream>
#include <iomanip>

using namespace std;

typedef vector<int> Row;
typedef vector<Row> table;


double cal_simi_avg(const vector<int> compute_this_table,const vector<int> compare_this_table);
double cal_avg(const vector<int> compute_this_table);

void paper_predict_all(const table my_table, int active_user,
                          int active_users_item_to_predict_vote );
void paper_predict_simi(const table my_table, int active_user,
                         int active_users_item_to_predict_vote );
void class_predict_all(const table my_table, int active_user,
                         int active_users_item_to_predict_vote);
void class_predict_simi(const table my_table, int active_user,
                          int active_users_item_to_predict_vote );
void top_rating_item(int active_user, int compare_with_this_user);

double simi_avg_simi(const vector<int> active_user,const vector<int> compare_to_user);
//double simi_avg_all(const vector<int> active_user,const vector<int> compare_to_user);

vector<int> parsing(string);
void print(const table);
int count_votes(vector<int>);
vector<string> lines;

int main(int argc, const char * argv[])
{
    string line;
    table my_table;
    ifstream myfile ("/Users/lsu/Documents/workspace/collaborative/collaborative/data.txt");
    int active_user = 3;
    int item_number = 0;
    cout<<"Begin .. "<<endl;
    if (myfile.is_open()){
        while ( getline (myfile,line) )
        {
            lines.push_back(line);
            if(parsing(line).size() > 0)
            {
                my_table.push_back(parsing(line));
        
            }
        }
        myfile.close();
    }else{
        cout << "Unable to open file";
    }
    print(my_table);
    cout<<"Prosessing .. paper prediction_All .. "<<endl;
    paper_predict_all(my_table, active_user, item_number);
    cout<<endl<<endl;
    cout<<"Prosessing .. paper prediction_Simi .."<<endl;
    paper_predict_simi(my_table, active_user, item_number);
    cout<<endl<<endl;
    cout<<"Prosessing .. class prediction_ALL .."<<endl;
    class_predict_all(my_table, active_user, item_number);
    cout<<endl<<endl;
    cout<<"Prosessing .. class prediction_Simi .."<<endl;
    class_predict_simi(my_table, active_user, item_number);
    cout<<endl<<endl;
    
    return 0;
}
vector<int> parsing(string each_line){
    
    string dummy;
    dummy.clear();
    Row my_row;
    
    for(int i = 0; i < each_line.length(); i++){
        
        if(each_line[i] != ' '){
            
            dummy += each_line[i];
            if(dummy[0] != 'i' && dummy[0] != 'u'){
            
                
                if (dummy[0] == '-') {
                    my_row.push_back(-1);
                }
                else{
                    my_row.push_back(atoi(dummy.c_str()));
                }
                
            }
            
               
        }else{
            dummy.clear();
        }
    }
    return my_row;
    
}

void print(table my_table){
    //cout<<" the size of my_table = "<<my_table.size()<<endl;
   /*
    for (int i = 0 ; i < my_table.size(); i++) {
        for(int j = 0; j < my_table[i].size(); j++){
            if(my_table[i][j] > 0){
                cout<<my_table[i][j]<<"  ";
            }
            else{
                cout<<"-"<<"  ";
            }
        }
        cout<<endl;
    }*/
    for (int k = 0; k < lines.size(); k++) {
        cout<<lines[k]<<endl;
    }
}

void paper_predict_all(const table my_table, int active_user,
                     int active_users_item_to_predict_vote ){
    
    double avg_of_each_i = 0.0;
    double k = 0.0;
    double simi = 0.0;
    double numerator = 0.0;
    for (int j = 0; j < my_table.size(); j++) {
        
        if((my_table[j][active_users_item_to_predict_vote] > 0) && active_user != j ){
            
            simi = simi_avg_simi(my_table[active_user], my_table[j]);
            k = k + abs(simi);
            avg_of_each_i = cal_avg(my_table[j]);
            cout<<" with user "<<j <<setw(10)<<"sim= " <<simi<<endl;
            numerator = numerator + simi*(my_table[j][active_users_item_to_predict_vote]-avg_of_each_i);
        
        }
        
    }
    cout<<"Predicted value for user "<< active_user<< " and item "<<active_users_item_to_predict_vote<<" = "
    <<( cal_avg(my_table[active_user])+ (numerator/k));
    
}

void paper_predict_simi(const table my_table, int active_user,
                         int active_users_item_to_predict_vote ){
    
    double avg_of_each_i = 0.0;
    double k = 0.0;
    double simi = 0.0;
    double numerator = 0.0;
    
    for (int j = 0; j < my_table.size(); j++) {
        
        if(my_table[j][active_users_item_to_predict_vote]>0 && active_user != j ){
            
            simi = simi_avg_simi(my_table[active_user], my_table[j]);
            k = k + abs(simi);
            avg_of_each_i = cal_simi_avg(my_table[j],my_table[active_user]);
            cout<<" with user "<<j <<setw(10)<<"sim= " <<simi<<endl;
            numerator = numerator + simi*(my_table[j][active_users_item_to_predict_vote]-avg_of_each_i);
        }
    }
    cout<<"Predicted value for user "<< active_user<< " and item "<<active_users_item_to_predict_vote<<" = "
    <<( cal_avg(my_table[active_user])+ (numerator/k));
}

void class_predict_simi(const table my_table, int active_user,
                          int active_users_item_to_predict_vote ){

    double avg_of_each_i = 0.0;
    int count = 0;
    double k = 0.0;
    double simi = 0.0;
    double numerator = 0.0;
    count = count_votes(my_table[active_user]);

    for (int j = 0; j < my_table.size(); j++) {
        if(my_table[j][active_users_item_to_predict_vote ]>0 && active_user != j ){
            simi = simi_avg_simi(my_table[active_user], my_table[j]);
            k = k + abs(simi);
            avg_of_each_i = cal_simi_avg(my_table[j],my_table[active_user]);
            cout<<" with user "<<j <<setw(10)<<"sim= " <<simi<<endl;
            numerator = numerator + simi*(my_table[j][active_users_item_to_predict_vote]-avg_of_each_i);
            
        }
    }
    cout<<"Predicted value for user "<< active_user<< " and item "<<active_users_item_to_predict_vote<<" = "
    <<( ((cal_avg(my_table[active_user])*count) + ((numerator/k)*(count+1)))/count);
}

void class_predict_all(const table my_table, int active_user,
                         int active_users_item_to_predict_vote){
    
    double avg_of_each_i = 0.0;
    int count = 0;
    double k = 0.0;
    double simi = 0.0;
    double numerator = 0.0;
    count = count_votes(my_table[active_user]);

    for (int j = 0; j < my_table.size(); j++) {
        if(my_table[j][active_users_item_to_predict_vote]>0 && active_user != j ){
            simi = simi_avg_simi(my_table[active_user], my_table[j]);
            k = k + abs(simi);
            avg_of_each_i = cal_avg(my_table[j]);
            cout<<" with user "<<j <<setw(10)<<"sim= " <<simi<<endl;
            numerator = numerator + simi*(my_table[j][active_users_item_to_predict_vote]-avg_of_each_i);
            
        }
    }
    cout<<"Predicted value for user "<< active_user<< " and item "<<active_users_item_to_predict_vote<<" = "
    << (((cal_avg(my_table[active_user])*count) + ((numerator/k)*(count+1)))/count);
}

double simi_avg_simi(const vector<int> active_user,const vector<int> compare_to_user){

    double avg_of_active_user  = cal_simi_avg(active_user,compare_to_user);
    double avg_of_compare_user = cal_simi_avg(compare_to_user,active_user);
    double numerator    = 0.0;
    double denominator1 = 0.0;
    double denominator2 = 0.0;
    
    for (int i =0; i<active_user.size();i++) {
        if(active_user[i] > 0 && compare_to_user[i]>0){
            numerator = numerator+((active_user[i]-avg_of_active_user)*(compare_to_user[i]-avg_of_compare_user));
            denominator1  = denominator1 + ((active_user[i]-avg_of_active_user)*(active_user[i]-avg_of_active_user));
            denominator2  = denominator2 + ((compare_to_user[i]-avg_of_compare_user)*(compare_to_user[i]-avg_of_compare_user));
        }
    }

    return (numerator/sqrt(denominator1*denominator2));
}
/*
double simi_avg_all(const vector<int> active_user,const vector<int> compare_to_user){
    
    double avg_of_active_user  = cal_simi_avg(active_user,compare_to_user);
    double avg_of_compare_user = cal_simi_avg(compare_to_user,active_user);
    double numerator    = 0.0;
    double denominator1 = 0.0;
    double denominator2 = 0.0;
    
    for (int i =0; i<active_user.size();i++) {
        if(active_user[i] > 0 && compare_to_user[i]>0){
            numerator = numerator+((active_user[i]-avg_of_active_user)*(compare_to_user[i]-avg_of_compare_user));
            denominator1  = denominator1 + ((active_user[i]-avg_of_active_user)*(active_user[i]-avg_of_active_user));
            denominator2  = denominator2 + ((compare_to_user[i]-avg_of_compare_user)*(compare_to_user[i]-avg_of_compare_user));
        }
    }
    return (numerator/sqrt(denominator1*denominator2));
}
 */
void top_rating_item(int active_user, int compare_with_this_user){
    
    
    cout<<"Printing top three item for user "<<active_user<< "is = ";
    
    
    
}

double cal_simi_avg(const vector<int> compute_this_table,const vector<int> compare_this_table){
    
    double sum = 0.0;
    int count=0;
    for (int i=0; i<compute_this_table.size(); i++) {
        if(compute_this_table[i] > 0 && compare_this_table[i] > 0)
        {
            sum = compute_this_table[i]+sum;
            count++;
        }
    }
    return sum/count;
}

double cal_avg(const vector<int> compute_this_table){
    
    double sum = 0.0;
    int count = 0;
    
    for (int i =0; i < compute_this_table.size(); i++) {
        if (compute_this_table[i] > 0) {
            sum = compute_this_table[i] + sum;
            count++;
        }
    }
    return sum/count;
}

int count_votes(vector<int> count_this_vector){
    
    int count= 0;
    
    for (int i = 0; i < count_this_vector.size(); i++) {
        if(count_this_vector[i] > 0){
            count++;
        }
    }
    return count;
}

