
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long wordcount(char *);
void *progress_monitor(void *PROGRESS_STATUS);

/*
*		Progress_status struct
*/

typedef struct {
 long CurrentStatus; /* Current status of computation */
 long TerminationValue; /* Done when CurrentStatus >= TerminationValue */
} PROGRESS_STATUS;


/************************
*
*	Main Method
*
*************************/

int main(int argc,char *argv[]){

	char * file_name;
	long count;

	/*	Arguments and File name check  */
	
	if(argc < 2){
		printf("File not provided.\n");
		exit(1);
	}else if(argc > 2){
		printf("Please provide only one file.\n");
		exit(1);
	}else{
		file_name = argv[1];
		count = wordcount(file_name);
	}

	printf("There are %lu words in this file.\n",count);

	return 0;
}

/************************
*
*	wordcount()
*
*************************/

long wordcount(char *file_name){

	int ch,space_count = 0;
	long size;
	long number_of_words = 0;
	FILE *fp;
	fp = fopen(file_name,"r");
	
	if(!fp) {
		printf("Could not open file!\n");	
		exit(1);
	}
	// Calculating file size
	fseek(fp,0L,SEEK_END);
	size = ftell(fp);
	fseek(fp,0L,SEEK_SET);

	// declaring and initializing the progress status struct
	PROGRESS_STATUS *progress_stat = malloc(sizeof(PROGRESS_STATUS));
	progress_stat->TerminationValue = size;
	progress_stat->CurrentStatus = 0;	

	// creating the thread to print the progress bar
	pthread_t progress_thread;
	pthread_create(&progress_thread,NULL,progress_monitor,(void *)progress_stat);


	if(fp){
		// calculating the file size in bytes

		
		//moving the stream back to the beginning of file
		ch = getc(fp);
		while(ch != EOF){
			char previous;

			// incrementing the status that is passed to the progress thread
 			progress_stat->CurrentStatus++;
			
			// counting the words  
			if(isspace(ch) && !isspace(previous)){
				number_of_words++;
				}
			previous = ch;
			ch = getc(fp);
		}
		
		
		fclose(fp);
	
	}else{
		printf("Could not open file!\n");
		exit(1);
	}
	
	// house cleaning part - thread join and free the allocated memory
	pthread_join(progress_thread,NULL);	
	free(progress_stat);
	return number_of_words;
}

/************************
*
*	progress_monitor()
*
*************************/

void *progress_monitor(void *progress_stat){
	
	int bar = 1;
	int i = 1;
	float progress_so_far;
	
	// casting the parameter back to the struct from (void *)
	PROGRESS_STATUS *status = (PROGRESS_STATUS *)progress_stat;
	
	
	while(i <= 50){
	
	//calculating the status of the bar with needed casting
	progress_so_far = ((float)(status->CurrentStatus / (float)status->TerminationValue) * 50);
	bar = (int) progress_so_far;
		
		for(i;i<=bar;i++){
			if(i % 10 == 0)
				printf("+"); // + sign for every ten
			else
				printf("-");
			
			fflush(stdout);		// flushing the output for smooth bar
		} 
	 }
	 printf("\n");
}
