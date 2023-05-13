
/* The following C code implements the room-assignment problem
 * Written By : Sai Suraj(21560) and Suvam Tamang(21561)
 * MTCS - 103(P) Project
 */

 #include<stdio.h>
 #include<time.h>
 #include<stdlib.h>
 #include<time.h>
 #include<math.h>
 //#include<omp.h>
 #include<mpi.h>

 float getRand(int min,int max);
 void getMatrix(int**,int);
 int check(int *,int );
 int contains(int * a, int p, int n);
 void genSolution(int * a, int n);
 int getCost(int **,int *,int );
 int* makeCopy(int *,int );
 int getCost(int **,int *,int);
 int room_assign(int **,int *,int *,int,int);
 void Free(int *,int *,int **,int );
 int getMin(int *a, int size);

 int main(int argc,char *argv[]){
	 int rank, np;
	 int cost = 0;
	 int n = 50;
	 int *aux,*a,**D;
	 double elapsed_time;
	 //int threads = atoi(argv[1]);

	 MPI_Init(&argc, &argv);
	 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	 MPI_Comm_size(MPI_COMM_WORLD, &np);
	 
	 int *results;
	 int newCost = 0;
	 //srand(rank + time(0));
	 a = (int *)calloc(n,sizeof(int));
	 D = (int **)calloc(n,sizeof(int *));
	 for(int i=0;i<n;i++){
	     D[i] = (int *)calloc(n,sizeof(int));
	 }

	    MPI_Barrier(MPI_COMM_WORLD);
	    elapsed_time = MPI_Wtime();
	    if ( rank == 0 ){
              getMatrix(D,n);
	      results = (int *)calloc(np,sizeof(int));
	    }
	    
            genSolution(a,n);
	    aux = makeCopy(a,n);
            
	    //Broadcasting the Compatibility matrix to other processes
            for (int i = 0; i < n; i++){
	         MPI_Bcast(D[i], n, MPI_INT, 0, MPI_COMM_WORLD);
	    }

	         //Every Process performs the room assignment and calculates the optimal cost it finds as newCost
                 newCost = room_assign(D,a,aux,n,rank);
	         Free(a,aux,D,n);

		 //Gathering the new cost calculated by each process
		 MPI_Gather(&newCost, 1, MPI_INT, results, 1, MPI_INT, 0, MPI_COMM_WORLD);

		 /*if(rank==0){
		 for(int i=0;i<np;i++){
			 printf("%d\n",results[i]);
		 }
		 }*/

		 elapsed_time += MPI_Wtime();
                 if(rank==0){
			printf("%d %d %lf\n", n, getMin(results, np) , elapsed_time);
		 }
										
	   // Free(a,aux,D,n);
	 return 0;
 }

 int getMin(int *a, int size){
      int min = a[0];
      for(int j = 0; j<size; j++)
       	 if(a[j] < min) 
		 min = a[j];
					
      return min;
 }

 void Free(int *a,int *aux,int **D,int n){
	 free(a);
	 free(aux);
	 for(int i=0;i<n;i++){
	    free(D[i]);}
	 free(D);
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
 

  int room_assign(int **D,int *a,int *aux,int n,int rank){
	  int cost = 0;
          //#pragma omp parallel num_threads(nthreads)
	  //{
	    int temp;
	    float u1,u2;
	    int c1,c2;
	    int t=1;
	    srand(rank+time(0));
	    cost = getCost(D,a,n);
            //#pragma omp for 
	    for(int i=0;i<1000;i++){
	         while(1){
	             u1 = getRand(0,1);
	             u2 = getRand(0,1);
	             c1 = floor(u1*n);
	             c2 = floor(u2*n);
		     if(a[c1]!=a[c2]) break;
	         }
		 temp = aux[c1];
	         aux[c1] = aux[c2];
	         aux[c2] = temp;

		 int newCost = getCost(D,aux,n);
	         int delta = newCost-cost;
	         double expo = exp( (double) (-delta)/t);
	         double u = ((double)rand()/(RAND_MAX));
	 
	         if (delta < 0 || expo >= u ){
		    int t = a[c1];
	            a[c1] = a[c2];
	            a[c2] = t;
		    cost = newCost;
		    i = 0;
	         }
	         else{
		      temp = aux[c2];
	              aux[c2] = aux[c1];
	              aux[c1] = temp;
		      i = i + 1;
	         }
	         t = 0.999*t;
	     }
	   //}
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
            if( !contains(a, p, n) )
            {
          	a[i] = p;
		a[i+(n/2)] = p;
            }
            else i--;
        }
     }
  }
 
 int* makeCopy(int *a,int n){
	 int *res = (int *)calloc(n,sizeof(int));
	 for(int i=0;i<n;i++){
		 res[i]=a[i];
	 }
	 return res;
 }
