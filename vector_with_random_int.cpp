#include <cstdlib>
#include <iostream>
#include <vector>
#include <time.h>
using namespace std;

int n = 10;
vector<int> myVector;

void init(){
    srand(time(0));
    for(int i = 0; i < n; i++){
        
        myVector.push_back((rand()%(n+n)) + (-n));

    }

}

void print_vector(){

    cout<<"[ ";
    for(int i = 0; i < n; i++){

         cout<<myVector[i];   
         if(i < n-1) cout <<" , ";

    }
    cout<<" ]"<<endl;
}


int main(int argc, char* argv[]){
    
    if(argc == 2){
        n = (int)strtol(argv[1],NULL,0);
    }
    init();
    print_vector();
    return 0;
}
