
// Homework 8 : PT Cruisers: Working with POSIX Threads
//
// file: pt-crusiers.c
//
// Is in charge of processing the command line and creating separate threads that each run a racer instance.
//
// @author: Ishan Shah is4761




#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "display.h"
#include "racer.h"


// check if given string number or not
// @param str string to check
// return 0 if int else return 1
static int is_number(char *str){
  
  int i=0;
  
  if(str[0] == '-'){
    i=1;
  }
  
  for(;str[i]!=0;i++){
    
    if(!isdigit(str[i])){
      return 0;
    }
    
  }
  return 1;
}

int main(int argc, char * argv[]){

	if(argc<4) {
	
		fprintf(stderr, "Usage: pt-cruisers [max-speed-delay] name1 name2 [name3...]\n");
		return EXIT_FAILURE;
   
	}
	
  srand(time(NULL));
 
  int wait_time = DEFAULT_DELAY;
	
  // racer starting index
	int racer_index = 1;
	
  // check first argument
  // flag to check if first argument is an int or list of cars
  int check = is_number(argv[1]);
	
  // first argument not a number
  if(!check) {
		for(int i=racer_index;i<argc; i++){
			if(strlen(argv[i])>MAX_NAME_LEN){
				fprintf(stderr, "Error: racer names must not exceed length 6.\n");
				return EXIT_FAILURE;
			}
		}
	} 
  else{
  
    // convert the first argument to an int
		int time = (int)strtol(argv[1], NULL, 10);
    
    if(time>0){
      wait_time = time;
    }
    else{
      wait_time = DEFAULT_DELAY;
    }
	
		// set the starting index to 2
		racer_index++;
    
		for(int i = racer_index; i < argc; i++) {
			// check if the names are too long
			if(strlen(argv[i]) > MAX_NAME_LEN) {
				fprintf(stderr, "Error: racer names must not exceed length 6.\n");
				return EXIT_FAILURE;
			}
		}	
	}
 
	// total number of racers
	int total = argc - racer_index;
  
  
  // declare threads
	pthread_t threads[total];
 
	// declare racers
	Racer* racer[total];
 
	int row = 0;
	
	clear();
 
  // initialise the racers
	for(int i=0; i<total; i++){
		racer[i] = make_racer(argv[i+racer_index],++row);
  }
  
	init_racers(wait_time);
 
	// initialise the threads
	for(int i = 0; i<total; i++){
 
		if(pthread_create(&threads[i],NULL,run,(void*)racer[i])){
			perror("pthread_create");
			return EXIT_FAILURE;
		}
   
	}
 
	// join the threads
	for(int i = 0; i<total; i++) {
		if(pthread_join(threads[i], NULL)) {
			perror("pthread_join");
			return EXIT_FAILURE;
		}
	}

	for(int i = 0; i < total; i++){
   
		destroy_racer(racer[i]);
  
  }

	set_cur_pos(total + 1, 1);
 
	return EXIT_SUCCESS;
}