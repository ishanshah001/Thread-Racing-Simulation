
// Homework 8 : PT Cruisers: Working with POSIX Threads
//
// file: racer.c
//
// a thread controlling a small figure that races across a character window
//
// @author: Ishan Shah is4761


#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "racer.h"
#include "display.h"

// mutex for sync
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// wait time for pause
long wait_time;


void init_racers(long milliseconds) {	

	wait_time = milliseconds;
 
}


Racer * make_racer(char * name, int position) {
	
	Racer * racer = (Racer*)malloc(sizeof(Racer));
  
	racer->graphic = (char*)malloc(MAX_CAR_LEN + 1);
 
  // the drawable text of the racer
	strncpy(racer->graphic, "~O=-------o>", MAX_CAR_LEN + 1);
  
	// replace the hyphens with name
	if(strlen(name)<=6){
		memcpy(racer->graphic+3, name, strlen(name));
	} 
  else{
		// print error message and exit
		fprintf(stderr, "Name length is too long (>6)!\n");
		exit(EXIT_FAILURE);
	}
  
	racer->row = position;

	racer->dist = 0;

	return racer;
}


void destroy_racer(Racer * racer) {
	
	free(racer->graphic);
	free(racer);

}


void * run(void * racer) {
	
	Racer * racerr = (Racer*)racer;
 
	// locking
	pthread_mutex_lock(&mutex);
 
	set_cur_pos(racerr->row, 1);
	printf("%s", racerr->graphic);
 
	fflush(stdout);
 
	// unlocking
	pthread_mutex_unlock(&mutex);
  
	// Stop when the racer reaches the finish line.
	while(1) {
		
		long random_wait_time = (rand() % wait_time);
    
		usleep(random_wait_time * 1000);
   
		// locking
		pthread_mutex_lock(&mutex);
   
		// flat
		if(random_wait_time<=3){
			racerr->graphic[1] = 'X';
		} 
    else{
   
			set_cur_pos(racerr->row, ++racerr->dist);
			put(' ');
		}
    set_cur_pos(racerr->row, FINISH_LINE);
    puts("|");
		set_cur_pos(racerr->row, racerr->dist + 1);
    puts(racerr->graphic);
   
		fflush(stdout);
   
		// unlocking
		pthread_mutex_unlock(&mutex);
   
		if(racerr->dist >= FINISH_LINE-1 || random_wait_time <= 3) {
      
			break;
      
		}
	}
 
	return NULL;
}
