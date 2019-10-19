#include<bits/stdc++.h>
int main(){
 
	 int numbers[100], dim=0, top, j;
	printf("Enter the quantity of numbers.\n");
	scanf("%d", &dim);
	while(dim >100) {printf("This number is no supported please choose other number\n"); scanf("%d", &dim);
	}
	printf("Enter the numbers.\n");
		
	for(top=0; top<dim; top++){
		scanf("%d", &numbers[top]);
	}
	for(int i=0; i<dim; i++){
		for(j=1; j<dim; j++){
			if(numbers[j]<numbers[j-1]){
				top=numbers[j];
				numbers[j]=numbers[j-1];
				numbers[j-1]=top;
			}
		} 
	}
	printf("number ordened:\n");
	for(top=0; top<dim; top++){
		printf( " %d ", numbers[top]);
 
	}
	puts("");
}
