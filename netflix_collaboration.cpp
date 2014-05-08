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
#include <unistd.h>

using namespace std;

struct ItemRating{
    int user;
    int item;
    int rating;
};


typedef vector<ItemRating> Row;
typedef vector<Row> table;

vector<int> parsing(string);
void print(const table);



double PredictionEq1(const table MyTable, int ActiveUser,
                       int PredictVoteOfItem );
double SimiCorrelation(const vector<ItemRating> ActiveUser,const vector<ItemRating> CompareToUser);
double CalAvg(const vector<ItemRating>);
double CalSimiAvg(const vector<ItemRating>,const vector<ItemRating>);
int HasItemAt(const table MyTable, int User,
                   int Item);


int main(int argc, const char * argv[])
{
    string line;
    table MyTable;
    ifstream myfile ("/Users/lsu/Documents/workspace/netflix_collaboration/netflix_collaboration/u1base.txt");
    ItemRating obj;
    int ActiveUser , PredictVoteOfItem = 0;
    vector<int> Dummy;
    double PredictedVote = 0.0;
    int CurrentUser = 1; // since in the file user starts from 1
    Row MyRow;
    clock_t start;
    
    cout<<"Enter user and item number sperating them by space : "<<endl;
    cin>>ActiveUser>>PredictVoteOfItem;
    ActiveUser = ActiveUser-1;
    cout<<"Begin .. "<<endl;
    
    if (myfile.is_open()){
        while ( getline (myfile,line))
        {
            if(parsing(line).size() > 0)
            {
                
                Dummy = parsing(line);
                if (CurrentUser == Dummy[0]) {
                    obj.user = Dummy[0];
                    obj.item = Dummy[1];
                    obj.rating = Dummy[2];
                    MyRow.push_back(obj);
                }
                else{
                    MyTable.push_back(MyRow);
                    CurrentUser = Dummy[0];
                    MyRow.clear();
                    obj.user = Dummy[0];
                    obj.item = Dummy[1];
                    obj.rating = Dummy[2];
                    MyRow.push_back(obj);
                    // cout<< " "<<dummy[0]<<endl;
                }
            }
        }
        MyTable.push_back(MyRow);
        myfile.close();
    }else{
        cout << "Unable to open file";
    }
    //print(my_table);
    start = std::clock();
    PredictedVote = PredictionEq1(MyTable, ActiveUser, PredictVoteOfItem );
    cout << "Prediction of vote for user " << MyTable[ActiveUser][0].user << " of item " <<PredictVoteOfItem << " = "<<PredictedVote <<endl;
    cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC ) << "  sec" << std::endl;
    return 0;
}
vector<int> parsing(string each_line){
    
    string dummy;
    dummy.clear();
    vector<int> my_row;
    int count = 0;
    
    for(int i = 0; i <= each_line.length(); i++){
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
        for(int j = 0; j < my_table[i].size(); j++){
            cout<<"user = "<<my_table[i][j].user<<", item = " <<my_table[i][j].item<< " , raing = " << my_table[i][j].rating<< "  "<<endl;
        }
        cout<<" next user "<<endl;
    }
}

double PredictionEq1(const table MyTable, int ActiveUser,
                       int PredictVoteOfItem ){
    
    double k = 0.0;
    double simi = 0.0;
    double numerator = 0.0;
    double avg_of_each_comparing_user = 0.0;
    int similar_item;
    for (int i = 0; i < MyTable.size(); i++) {
        
        similar_item = HasItemAt(MyTable,i,PredictVoteOfItem);
        if(i != (MyTable[ActiveUser][0].user-1) && (similar_item >= 0)){
            
            simi = SimiCorrelation(MyTable[ActiveUser], MyTable[i]);
            k = k + abs(simi);
            avg_of_each_comparing_user = CalSimiAvg(MyTable[i],MyTable[ActiveUser]);
            numerator = numerator + simi*(MyTable[i][similar_item].rating -
                                   avg_of_each_comparing_user);

        }
    }
    
    return (CalAvg(MyTable[ActiveUser])+ numerator/k);
    
}
/*
 returns if the index where the specificific item resides
 else return -1 to indicate that there is no such item 
 in the array
 */

int HasItemAt(const table MyTable,int User,
                   int Item){
    int i = 0;
    bool has_it = false;
    
    while ((Item) >= MyTable[User][i].item && MyTable[User].size()>i) {
        if((Item) == MyTable[User][i].item ){
            has_it = true;
            i++;
        }
        else i++;
    }
    if(has_it == true){
        return i-1;
    }
    else return -1;
}

double SimiCorrelation(const vector<ItemRating> ActiveUser,const vector<ItemRating> CompareToUser){
    
    double AvgOfActiveUser  = CalSimiAvg(ActiveUser, CompareToUser);
    double AvgOfCompareUser = CalSimiAvg(CompareToUser, ActiveUser);
   
    double numerator    = 0.0;
    double denominator1 = 0.0;
    double denominator2 = 0.0;
    int j = 0;
    int i = 0;
    int tmp;
   
    while( (i < ActiveUser.size()) && (j < CompareToUser.size())){
        if(ActiveUser[i].item == CompareToUser[j].item ){
            
            numerator = numerator+((ActiveUser[i].rating-AvgOfActiveUser)*(CompareToUser[j].rating-AvgOfCompareUser));
            denominator1  = denominator1 + ((ActiveUser[i].rating-AvgOfActiveUser)*(ActiveUser[i].rating-AvgOfActiveUser));
            denominator2  = denominator2 + ((CompareToUser[j].rating-AvgOfCompareUser)*(CompareToUser[j].rating-AvgOfCompareUser));
            }
        tmp = j;
        if(ActiveUser[i].item >= CompareToUser[tmp].item){
            j++;
        }
        if(ActiveUser[i].item <= CompareToUser[tmp].item){
            i++;
        }
        
    }
    if(denominator1 == 0 || denominator2 == 0){
        return  0;
    }
    else return (numerator/sqrt(denominator1*denominator2));
}


double CalAvg(const vector<ItemRating> row_of_item){
    
    double sum = 0.0;
    for (int i = 0; i < row_of_item.size(); i++) {
        sum = sum + row_of_item[i].rating;
    }
    return sum/row_of_item.size();
}

double CalSimiAvg(const vector<ItemRating> active_user, const vector<ItemRating> compare_user){
    
    int i = 0;
    int j = 0;
    double sum = 0.0;
    double count = 0;
    int tmp;
    
    while ((active_user.size() > i) && (compare_user.size() > j) ) {
        if(active_user[i].item == compare_user[j].item){
            count++;
            sum = sum + active_user[i].rating;
        }
        tmp = j;
        if(active_user[i].item >= compare_user[tmp].item){
            j++;
        }
        if(active_user[i].item <= compare_user[tmp].item){
            i++;
        }
    }
    if(count == 0){
        return 0;
    }
    else{
        return sum/count;
    }
}
