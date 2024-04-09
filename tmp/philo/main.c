#include "philo.h"

int	ft_strlen(const char *message)
{
	int	i;

	i = 0;
	while (message && message[i])
		i++;
	return (i);
}

void	error_exit(const char *message)
{
	write(2, message, ft_strlen(message));
	exit (EXIT_FAILURE);
}

unsigned int	ft_atou(char *str)
{
	int	i;
	unsigned int	result;
	unsigned int	temp;

	i = 0;
	if (!str || !str[i])
		error_exit("Error: argument not valid\n");
	while (str[i] == ' ' || (str[i] <= '\t' && str[i] >= '\r'))
		i++;
	if (str[i] == '-')
		error_exit("Error: argument not valid\n");
	if (str[i] == '+')
		i++;
	result = 0;
	if (str[i] <= '0' || str[i] >= '9')
		error_exit("Error: argument not valid\n");
	while (str[i] >= '0' && str[i] <= '9')
	{
		temp = result * 10 + str[i] - '0';
		if (temp < result)
			error_exit("Error: argument not valid\n");
		result = temp;
		i++;
	}
	return (result);
}

void	parse(t_process_data *process_data, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		error_exit("Error: wrong number of arguments\n");
	process_data->total_philos = ft_atou(argv[1]);
	process_data->time_before_die = ft_atou(argv[2]);
	process_data->meal_duration = ft_atou(argv[3]);
	process_data->sleep_duration = ft_atou(argv[4]);
	if (argc == 6)
		process_data->total_meals = ft_atou(argv[5]);
	//limit is 0 for all except for times to eat?
	//already some handling if there is only one routine
	printf("number of philos: %u\n", process_data->total_philos);
	printf("time to die: %u\n", process_data->time_before_die);
	printf("time to eat: %u\n", process_data->meal_duration);
	printf("time to sleep: %u\n", process_data->sleep_duration);
	if (argc == 6)
		printf("times to eat: %u\n", process_data->total_meals);
}

void	update_time(t_process_data *process_data)
{
	gettimeofday(&process_data->time, NULL);
	process_data->current_time = process_data->time.tv_sec * 1000L + process_data->time.tv_usec / 1000L;
}



void	*eat(t_thread_data *thread_data)
{
	pthread_mutex_lock(&thread_data->process_data->forks[thread_data->left_fork]);
	update_time(thread_data->process_data);
	printf("%ldms: %d has taken a fork\n", thread_data->process_data->current_time, thread_data->philo_number);

	pthread_mutex_lock(&thread_data->process_data->forks[thread_data->right_fork]);


	update_time(thread_data->process_data);
	printf("%ldms: %d is eating\n", thread_data->process_data->current_time, thread_data->philo_number);
	usleep(thread_data->process_data->meal_duration * 1000L);



	pthread_mutex_unlock(&thread_data->process_data->forks[thread_data->left_fork]);
	pthread_mutex_unlock(&thread_data->process_data->forks[thread_data->right_fork]);
	return ((void *)1);
}
/*
	wait for an eating request?
	die if current time - time of last meal >= time to die`
	at the same time allow it all the philos whos philo number is the same modulo % 2
	for an uneven amount of philos make that number of philos - 1 / 2
	and for even amount number of philos / 2
	*/
void	initialize_philo(t_thread_data *thread_data)
{
	thread_data->left_fork = thread_data->philo_number;
	if (thread_data->philo_number == 0)
		thread_data->right_fork = thread_data->process_data->total_philos - 1;
	else
		thread_data->right_fork = thread_data->philo_number - 1;
}

void	*routine(void	*thread_args)
{

	t_thread_data	*thread_data;

	thread_data = (t_thread_data *)thread_args;


	initialize_philo(thread_data);

	while (1)
	{
		update_time(thread_data->process_data);
		printf("%ldms: %d is thinking\n", thread_data->process_data->current_time, thread_data->philo_number);


		eat(thread_data);

		update_time(thread_data->process_data);
		thread_data->time_last_meal = thread_data->process_data->current_time;
		printf("%ldms: %d is sleeping\n", thread_data->process_data->current_time, thread_data->philo_number);
		usleep(thread_data->process_data->sleep_duration * 1000L);


	}
	//return value?
	return (NULL);

	//what is void pointer actually?
}
void	*manage(void *process_args)
{
	t_process_data *process_data;

	process_data = (t_process_data *)process_args;

	return (NULL);
}

void	free_and_exit(t_process_data *process_data, t_thread_data **threads, const char *message)
{
	if (process_data->philos)
		free(process_data->philos);
	if (process_data->forks)
		free(process_data->forks);
	if (*threads)
		free(*threads);
	error_exit(message);
}

void	initialize_arrays(t_process_data *process_data, t_thread_data **threads)
{
	process_data->philos = NULL;
	process_data->forks = NULL;
	*threads = NULL;

	process_data->philos = malloc(process_data->total_philos * sizeof(pthread_t));
	if (!process_data->philos)
		free_and_exit(process_data, threads, "Error: malloc fail\n");
	process_data->forks = malloc(process_data->total_philos * sizeof(pthread_mutex_t));
	if (!process_data->forks)
		free_and_exit(process_data, threads, "Error: malloc fail\n");
	*threads = malloc(process_data->total_philos * sizeof(t_thread_data));
	if (!threads)
		free_and_exit(process_data, threads, "Error: malloc fail\n");
}

int	main(int argc, char **argv)
{
	t_process_data process_data;
	t_thread_data	*threads;



	unsigned int	i;

	update_time(&process_data);
	process_data.time_start = process_data.current_time;
	printf("%ld ms: nothing is happening\n", process_data.current_time);
	parse(&process_data, argc, argv);
	initialize_arrays(&process_data, &threads);

	i = 0;
	while (i < process_data.total_philos)
	{
		pthread_mutex_init(&process_data.forks[i], NULL);
		//protect forks init?
		i++;
	}

	pthread_create(&process_data.manager, NULL, manage, (void *)&process_data);

	i = 0;
	while (i < process_data.total_philos)
	{
		threads[i].philo_number = i;
		threads[i].process_data = &process_data;
		threads[i].time_last_meal = -1L;
		threads[i].time_left_to_live = process_data.time_before_die;
		pthread_create(&process_data.philos[i], NULL, routine, (void *)&threads[i]);

		i++;
	}

	i = 0;
	while (i < process_data.total_philos)
	{
		pthread_join(process_data.philos[i], NULL);
		i++;
	}

	pthread_join(process_data.manager, NULL);

	i = 0;
	while (i < process_data.total_philos)
	{
		pthread_mutex_destroy(&process_data.forks[i]);
		i++;
	}

}