#include <stdio.h>

//static int *a_arr[4]; 
union ex {
 struct {
 	union Ex *ptr;
	int q;
	int s;
}ex1;
long x;
};	

typedef union ex Ex;

static __thread Ex *ar[5];

int change_arr(Ex *a){
	// int *p = a;
	// *p = 5;
	//ex *p = *a;
	a->ex1.s = 5;

	a->ex1.q = 3;
}

int print_arr(){
	Ex* a = &ar[0];
	printf("%d - %d" , a->ex1.s , a->ex1.q);
}

int main(int argc, char **argv)
{
   print_arr();
   //printf("%d", ar);
   change_arr(&ar[0]);
   print_arr();
   return 0;
}

