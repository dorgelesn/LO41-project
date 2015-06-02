
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "main.h"

#define NUM_INTERCHANGE	4
#define NUM_VEHICLES	10

//COMMON FUNCTIONS

void erreur(const char *msg)
{
    perror(msg);
    exit(1);
}

//fonction qui retourne un nombre entre 0 et max.
int get_random (int max){
	   double val;
      val = (double) max * rand ();
	 val = val / (RAND_MAX + 1.0);
	    return ((int) val);
}

//THREAD VEHICLES FUNCTIONS

void AfficheEtatV(vehicule* current)
{

    printf("\nJe suis la voiture  #%ld \nde TID : %ld \nde type %d \nje pars de %d et je vais à %d\n",current->num,(long) pthread_self(),current->type,current->position,current->itineraire);
    fflush(stdout);
}


void *creationVehicule(void *data)
{
    /* random pour le type de voiture 0->normal 1->prioritaire.
     * random pour la position 
     * random pour l'itinéraire
    */
    vehicule newVehicule;

    newVehicule.num = (long) data;
    newVehicule.type = get_random(2); 
    newVehicule.position = get_random(8);
    do{
	    newVehicule.itineraire = get_random(8);
    }while(newVehicule.position==newVehicule.itineraire);

    AfficheEtatV(&newVehicule);
    pthread_exit(NULL);

}

//INTERCHANGE FUNCTIONS

void AfficheEtatI(echangeur* current)
{

    printf("\nJe suis l'echangeur  #%ld \nde TID : %ld \n j'ai %d vehicules ici \n",current->num,(long) pthread_self(),current->nbV);
    fflush(stdout);
}


void *creationEchangeur(void *data)
{
    echangeur newEchangeur;

    newEchangeur.num = (long) data;
    newEchangeur.nbV = 0;

    AfficheEtatI(&newEchangeur);
    pthread_exit(NULL);

}


//MAIN FUNCTION

int main(void)
{
	//THREAD INIT
	pthread_attr_t thread_attr;

  if (pthread_attr_init (&thread_attr) != 0) {
    fprintf (stderr, "pthread_attr_init error");
    exit (1);
  }

  if (pthread_attr_setdetachstate (&thread_attr, PTHREAD_CREATE_DETACHED) != 0) {
    fprintf (stderr, "pthread_attr_setdetachstate error");

    exit (1);
  }
	//THREAD CREATION

    int i;
	// j'ai peut etre bossé pour rien... les échangeurs devaient ^etre des ressources critique
    	//INTERCHANGE
    pthread_t Ethread_id[NUM_INTERCHANGE];
	
    for (i = 0; i < NUM_INTERCHANGE; i++) {

	if (pthread_create (&Ethread_id[i], &thread_attr,creationEchangeur ,(void*)i) < 0) {
          fprintf (stderr, "pthread_create error for thread \n");
        exit (1);
  }
	 
    }
    	//VEHICLES
	pthread_t Vthread_id[NUM_VEHICLES];
	
    for (i = 0; i < NUM_VEHICLES; i++) {

	if (pthread_create (&Vthread_id[i], &thread_attr,creationVehicule ,(void*)i) < 0) {
          fprintf (stderr, "pthread_create error for thread \n");
        exit (1);
  }
	 
    }	    	 

	pthread_exit(NULL);
}
