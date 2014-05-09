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
    int totalsum;

};


typedef vector<ItemRating> Row;
typedef vector<Row> table;
vector<vector<double>> HoldSimiAvg;
vector<int> parsing(string);
void print(const table);
double PredictionEq1(const table MyTable, int ActiveUser,
                       int PredictVoteOfItem);
double SimiCorrelation(const vector<ItemRating> ActiveUser,const vector<ItemRating> CompareToUser, double, double);
double CalSimiAvg(const vector<ItemRating>,const vector<ItemRating>);
int HasItemAt(const table MyTable, int User,
                   int Item);
table Filling_Table(table,string);
double root_mean_square(table BaseTable, table TestTable);
void CalTotalItemRatingOfAll(table& BaseTable);
void CalTotalSimiRatingOfAll(table BaseTable,vector<vector<double>>&);
int getSizeOfItemsUserRated(vector<ItemRating> ItemsOfUser);


int main(int argc, const char * argv[])
{
    string line;
    table BaseTable;
    table TestTable;
    Row MyRow;
    clock_t start;
    double MSSE = 0.0;
    
    BaseTable = Filling_Table(BaseTable,"/Users/bghimire/workspace/neflix_prediction_algorithm/neflix_prediction_algorithm/u1base.txt");
    TestTable = Filling_Table(TestTable,"/Users/bghimire/workspace/neflix_prediction_algorithm/neflix_prediction_algorithm/u1test.txt");
    CalTotalItemRatingOfAll(BaseTable );
    CalTotalSimiRatingOfAll(BaseTable, HoldSimiAvg);
    cout<<"Begin .. "<<endl;
    //print(BaseTable);
    
    start = std::clock();
    MSSE  = root_mean_square(BaseTable, TestTable);
    cout << " Total Mean sum of square Error = " << MSSE << endl;
    cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC ) << "  sec" << std::endl;
    
    return 0;
}


/*
 * This function parse the data from the file the first colum 
 * is the user column, the second colum is the item column and the
 * third colum is the rating column given by the user
 */
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

/*
 * This function reads the file and fills the Table
 *
 */
table Filling_Table(table BaseTable,string filepath){
    
    ifstream myfile (filepath);
    vector<int> Dummy;
    int CurrentUser = 1; // since in the file user starts from 1
    Row MyRow;
    string line;
    ItemRating obj;
    
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
                    BaseTable.push_back(MyRow);
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
        BaseTable.push_back(MyRow);
        myfile.close();
    }else{
        cout << "Unable to open file";
    }

    return BaseTable;
}

/* 
 * This function prints the file out
 */
 
void print(table my_table){
    cout<<" the size of my_table = "<<my_table.size()<<endl;
    
    for (int i = 0 ; i < my_table.size(); i++) {
        for(int j = 0; j < my_table[i].size(); j++){
            cout<<"user = "<<my_table[i][j].user<<", item = " <<my_table[i][j].item<< " , raing = " << my_table[i][j].rating<< "  "<<endl;
        }
        cout<<" next user "<<endl;
    }
}

double root_mean_square(table BaseTable, table TestTable){
    
    double actual_value    = 0.0;
    double predicted_value = 0.0;
    double MSSE            = 0.0;
    int count              = 0;
    for (int i = 0; i < TestTable.size(); i++){
        for (int j = 0; j<TestTable[i].size(); j++) {
            predicted_value = PredictionEq1(BaseTable, TestTable[i][j].user, TestTable[i][j].item);
            cout<<" predicted vote for user "<< TestTable[i][j].user << " and item "<<TestTable[i][j].item<<"  = "<<predicted_value<<endl;
            actual_value    = TestTable[i][j].rating;
            MSSE            = (pow((actual_value - predicted_value), 2));
            count++;
        }
    }
    
    return (MSSE/count);
}

/* This function predicts the rating of the user using 
 * eq1 of the paper
 */
double PredictionEq1(const table BaseTable, int ActiveUser,
                       int PredictVoteOfItem){
    
    double k = 0.0;
    double simi = 0.0;
    double numerator = 0.0;
    double AvgOfEachComparingUser = 0.0;
    int similar_item = -1;
    int TrackSimi = 0;
    double AvgOfActiveUser = 0.0;
    //cout<<"BaseTable Size  = "<<BaseTable.size()<<endl;
    AvgOfActiveUser = (double)BaseTable[ActiveUser-1][0].totalsum/(double)BaseTable[ActiveUser-1].size();
    //cout<<"AvgOfActiveUser = "<<AvgOfActiveUser<<endl;
    for (int i = 0; i < BaseTable.size(); i++) {
        
        similar_item = HasItemAt(BaseTable,i,PredictVoteOfItem);
        //cout<<"similar_item = "<<similar_item<<"   "<<endl;
        if(i != (ActiveUser-1) && (similar_item < 0)) { TrackSimi++;}
        if(i != (ActiveUser-1) && (similar_item >= 0)){ // ActiveUser-1 because i start from 0
            //cout<<"("<<(ActiveUser-1)<<","<<TrackSimi<<")"<<endl;
            simi = HoldSimiAvg[ActiveUser-1][TrackSimi];//= SimiCorrelation(BaseTable[ActiveUser-1], BaseTable[i]);
            k = k + abs(simi);
            //cout<<"For user                   =  "<<BaseTable[i][similar_item].user<<endl;
            AvgOfEachComparingUser = (double)(BaseTable[i][0].totalsum - BaseTable[i][similar_item].rating)/(double)(getSizeOfItemsUserRated(BaseTable[i])-1);
            //cout<<"First term and second term =  "<<BaseTable[i][similar_item].rating<<" - "<<AvgOfEachComparingUser<<endl;
            //cout<<"simi                       =  "<<simi<<endl;
            numerator = numerator + simi*(BaseTable[i][similar_item].rating -
                                          AvgOfEachComparingUser);
            
            TrackSimi++;
        }
        
        
    }
    
    return (AvgOfActiveUser+ numerator/k);
    
}

void CalTotalSimiRatingOfAll(table BaseTable, vector<vector<double>>& HoldSimiAvg){
    
    vector<double> simi;
    double value = 0.0;
    for (int i = 0; i < BaseTable.size(); i++) {
        for (int j  = 0; j < BaseTable.size(); j++) {
            if(i != j){
                double AvgOfActiveUser  = CalSimiAvg(BaseTable[i], BaseTable[j]);
                double AvgOfCompareUser = CalSimiAvg(BaseTable[j], BaseTable[i]);
                value  = SimiCorrelation(BaseTable[i], BaseTable[j],AvgOfActiveUser,AvgOfCompareUser);
                //cout<<"simi between activer user "<<i <<" and compare to user "<< j << " = " << value <<endl;
                simi.push_back(value);
                
            }
        }
        HoldSimiAvg.push_back(simi);
        simi.clear();
    }

}

/*
 returns if the index-1 where the specificific item resides
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
/*
 * This function calculates the correlation between the 
 * active user and the other user that is compared with
  */
double SimiCorrelation(const vector<ItemRating> ActiveUser,const vector<ItemRating> CompareToUser, double AvgOfActiveUser, double AvgOfCompareUser){

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

/* calculates the average rating of the similar items
 * present between the active user the compared user
 */



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

/* 
 * calculates the total sum of all the rating rated by the
 * user
 */


void CalTotalItemRatingOfAll(table& BaseTable){
    
    double sum = 0.0;
    for (int i = 0; i < BaseTable.size(); i++) {
        for (int j = 0; j < BaseTable[i].size(); j++) {
            //cout<<"Inside CaltotalItemRatingOfAll of user "<<i<<" and item "<<j<<" rating  = "<<BaseTable[i][j].rating<<endl;
            sum = sum + BaseTable[i][j].rating;
        }
        BaseTable[i][0].totalsum = sum;
        //cout << "Total sum = "<<sum<<endl;
        sum = 0;
    }
    //return sum;
}





int getSizeOfItemsUserRated(vector<ItemRating> ItemsOfUser){
    
    return (int)ItemsOfUser.size();
}
