#include <pthread.h>

struct station {
	    int empty_seats_in_train;
		int passengers_arrived_to_station;
		int passengers_in_train;
		pthread_mutex_t mutex;
	    pthread_cond_t train_arrival;
		pthread_cond_t train_move;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
