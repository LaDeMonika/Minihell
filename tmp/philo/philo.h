#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>

typedef struct s_process_data
{
	unsigned int	total_philos;
	unsigned int	time_before_die;
	unsigned int	meal_duration;
	unsigned int	sleep_duration;
	unsigned int	total_meals;
	long	time_start;
	pthread_t *philos;
	pthread_t manager;
	pthread_mutex_t	*forks;
	struct timeval	time;
	long	current_time;
} t_process_data;

typedef	struct s_thread_data
{
	unsigned int	philo_number;
	long	time_left_to_live;
	long	time_last_meal;
	int	left_fork;
	int	right_fork;
	t_process_data	*process_data;

}	t_thread_data;



#endif