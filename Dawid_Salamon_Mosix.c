#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>


int main (int argc, char *argv[]){

	if(argc !=3){
		printf("zle parametry");
		return 1;
	}
	int n= atoi(argv[1]);
	int k  = atoi(argv[2]);
	if (n%2 !=0 || (n/2)&k != 0){
		printf("zle numerki");
		return 1;
	}
   	//printf("n: %d k: %d",n,k);
   
   int pipefd[2];
   int i;
   int chunk_size = n/(2*k);
   pipe(pipefd);
   for ( i=0 ; i<k ;i++){
   	pid_t pid = fork();
   	if (pid == 0){
   		
		//printf("chunksize a: %i b: %i \n",i*chunk_size, (i+1)*chunk_size);
		double sum = 0.0;
		double term;
		int start = i*chunk_size;
		int end = (i+1)*chunk_size;
		int j; 
		for ( j=start;j<end; j++){
			term =1.0/(2*j+1);
			if (j%2!=0){
			term = -term;
			}
			sum = sum +term;
			//printf("sum %f\n term %f \n", sum, term);
		}
		close(pipefd[0]);
		write(pipefd[1],&sum, sizeof(sum));
		close(pipefd[1]);
		exit(0);
   	}
	}
   close(pipefd[1]);
   double total_sum = 0.0;
   for ( i=0; i<k; i++){
   	double partial_sum;
	read(pipefd[0], &partial_sum, sizeof(partial_sum));
	//printf("Partial sum: %f\n", partial_sum);
	total_sum = total_sum + partial_sum;
   }
   close(pipefd[0]);
   for (i=0 ; i<k; i++){
   	wait(NULL);
   }
   double pi = 4 * total_sum;
   printf("wynik: %.15f\n",pi);
   return 0;
   
   
}
