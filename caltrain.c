#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
	station->passengers_arrived_to_station = 0;
	station->passengers_in_train = 0 ;
	pthread_mutex_init(&station->mutex, NULL);
	pthread_cond_init(&station->train_arrival, NULL);
	pthread_cond_init(&station->train_move, NULL);
}

void
station_load_train(struct station *station, int count)
{
	pthread_mutex_lock(&station->mutex);

	station->empty_seats_in_train = count ;

	while(station->empty_seats_in_train > 0 && station->passengers_arrived_to_station > 0)
	{
		pthread_cond_broadcast(&station->train_arrival);
		pthread_cond_wait(&station->train_move,&station->mutex);
	}
	station->empty_seats_in_train = 0 ;
	pthread_mutex_unlock(&station->mutex);



}

void
station_wait_for_train(struct station *station)
{
		pthread_mutex_lock(&station->mutex);
	    station->passengers_arrived_to_station ++;
	    while(station->passengers_in_train == station->empty_seats_in_train)
	        pthread_cond_wait(&station->train_arrival, &station->mutex);
	    station->passengers_arrived_to_station --;
	    station->passengers_in_train ++;
		pthread_mutex_unlock(&station->mutex);
}

void
station_on_board(struct station *station)
{
	pthread_mutex_lock(&station->mutex);

	station->empty_seats_in_train-- ;
	station->passengers_in_train-- ;

	if( (station->passengers_arrived_to_station==0 && station->passengers_in_train==0) || station->empty_seats_in_train==0 ){
		station->passengers_in_train = 0 ;
		pthread_cond_signal(&station->train_move);
	}

	pthread_mutex_unlock(&station->mutex);
}
