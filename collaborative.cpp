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

using namespace std;


int main(int argc, const char * argv[])
{
    typedef vector<int> Row;
    typedef vector<Row> table;
    table my_table(4,Row(4)) ;
    string line;
    ifstream myfile ("/Users/lsu/Documents/workspace/collaborative/collaborative/data.txt");
    int i = 0;
    int j = 0;
    int k = 0;
    
    
    if (myfile.is_open())
    {
        
        while ( getline (myfile,line) )
        {
            
            
            if(line[j] != '#'){
                
                while(line[j] != '\0' ){
            
                    
                    if(line[j] != ' '){
                       // cout<<i<<","<<j<< "   ";
                    
                      //  cout<<line[j]-'0'<<"  ";
                    
                        my_table[i][k] = line[j] - '0';
                        k++;
                    }
                    j++;
                    
                }
                j=0;
                k=0;
                i++;
                cout<<endl;
                
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
    
    

   
    
    return 0;
}

