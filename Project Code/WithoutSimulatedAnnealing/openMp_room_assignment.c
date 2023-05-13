
/* The following C code implements the room-assignment problem
 * Written By : Sai Suraj(21560) and Suvam Tamang(21561)
 * MTCS - 103(P) Project
 */

 #include<stdio.h>
 #include<time.h>
 #include<stdlib.h>
 #include<time.h>
 #include<math.h>
 #include<omp.h>

 float getRand(int min,int max);
 void getMatrix(int**,int);
 int check(int *,int );
 int contains(int * a, int p, int n);
 void genSolution(int * a, int n);
 int getCost(int **,int *,int );
 int* makeCopy(int *,int );
 int getCost(int **,int *,int);
 int room_assign(int **,int *,int *,int,int,int);
 void Free(int *,int *,int **,int );

 int main(int argc,char *argv[]){
	 
	 int cost = 0;
	 int n;
	 int *aux,*a,**D;
	 int newCost = 0;
         FILE *fp = fopen(argv[1],"w");
	 if(fp==NULL){
		 printf("Error!\n");
		 exit(-1);
	 }
         int t = atoi(argv[2]);
	 //n = 50;
	 for(n=10;n<=100;n+=2){
	    a = (int *)calloc(n,sizeof(int));
	    D = (int **)calloc(n,sizeof(int *));
	    for(int i=0;i<n;i++){
		 D[i] = (int *)calloc(n,sizeof(int));
	    }
            
            getMatrix(D,n);
            genSolution(a,n);
	    aux = makeCopy(a,n);
	    cost = getCost(D,a,n);
            
	    //Only two students can't arrange them
	    if ( n == 2 ){  
		    newCost = cost;
	            fprintf(fp,"%d %d %d\n",n,cost,newCost);
		    //continue;
		    return 0;
	    }
            else{
                   newCost = room_assign(D,a,aux,n,cost,t);
	           fprintf(fp,"%d %d %d\n",n,cost,newCost);
	    }
	    Free(a,aux,D,n);
	 }
	 fclose(fp);
	 return 0;
 }

 void Free(int *a,int *aux,int **D,int n){
	 free(a);
	 free(aux);
	 for(int i=0;i<n;i++){
	    free(D[i]);}
	 free(D);
 }

  int room_assign(int **D,int *a,int *aux,int n,int cost,int nt){
          double start = omp_get_wtime();
          #pragma omp parallel num_threads(nt)
	  {
	    //int temp;
	    float u1,u2;
	    int c1,c2;
	    //int t=1;
            #pragma omp for 
	    for(int i=0;i<1000;i++){
	         while(1){
	             u1 = getRand(0,1);
	             u2 = getRand(0,1);
	             c1 = floor(u1*n);
	             c2 = floor(u2*n);
		     if(a[c1]!=a[c2]) break;
	         }
		 int temp = aux[c1];
	         aux[c1] = aux[c2];
	         aux[c2] = temp;

		 int newCost = getCost(D,aux,n);
	         int delta = newCost-cost;
	         //double expo = exp( (double) (-delta)/t);
	         //double u = ((double)rand()/(RAND_MAX));
	 
	         if (delta < 0){ // || expo >= u ){
		    int flag = a[c1];
	            a[c1] = a[c2];
	            a[c2] = flag;
		    cost = newCost;
		    i = 0;
	         }
	         else{
		      int fl = aux[c2];
	              aux[c2] = aux[c1];
	              aux[c1] = fl;
		      i = i + 1;
	         }
	         //t = 0.999*t;
	     }
	   }
           double end = omp_get_wtime();
	   printf("Size : %d Time Taken : %lf\n",n,end-start);
	   return cost;
  }

 // returns an int between [min, max[
 float getRand(int min, int max)
 {
   return (int) min + (rand() / (((double)RAND_MAX + 1)/ max));
 }

 void getMatrix(int **D,int size){
	 int num;
	 for(int i=0;i<size;i++){
		 for(int j=i;j<size;j++){
		     num = rand()%11;
		     D[i][j]=num;
		     D[j][i]=num;
                 }
	 }
 }
 
 int contains(int * a, int p, int n)
 {
	 for(int i = 0; i<n; i++){
		if (a[i] == p) return 1;
         }
	 return 0;
 } 

 // generates a random solution
 void genSolution(int * a, int n)
 {
    for(int i=0; i<n; i++){
	if(a[i] == 0)
	{
	    int p = rand() % (n/2) + 1;
	    //int p = rand() % (n/2) ;
            if( !contains(a, p, n) )
            {
          	a[i] = p;
		a[i+(n/2)] = p;
            }
            else i--;
        }
     }
  }

 int getCost(int **D,int *a,int n){
	 int cost = 0;
	 for(int i=0;i<n;i++){
		 for(int j=0;j<n;j++){
			 if(a[i] == a[j]){
				 cost += D[i][j];
			 }
		 }
	 }
	 return cost;
 }
 
 int* makeCopy(int *a,int n){
	 int *res = (int *)calloc(n,sizeof(int));
	 for(int i=0;i<n;i++){
		 res[i]=a[i];
	 }
	 return res;
 }
