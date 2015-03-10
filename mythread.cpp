#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mt = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void * th3_impl(void *)
{
	pthread_mutex_lock(&mt);
	printf("Inside third thread\n");
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mt);
}

void * th2_impl(void *)
{
	pthread_mutex_lock(&mt);
	printf("Inside second thread\n");
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mt);
}

void * th1_impl(void *)
{
	pthread_mutex_lock(&mt);
	printf("Inside first thread\n");
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mt);
}

int main()
{
	pthread_t th1, th2, th3;
	pthread_mutex_lock(&mt);
	pthread_create(&th1, NULL, &th1_impl, NULL);
	pthread_cond_wait(&cond, &mt);
	pthread_mutex_unlock(&mt);

	pthread_mutex_lock(&mt);
	pthread_create(&th2, NULL, &th2_impl, NULL);
	pthread_cond_wait(&cond, &mt);
	pthread_mutex_unlock(&mt);

	pthread_mutex_lock(&mt);
	pthread_create(&th3, NULL, &th3_impl, NULL);
	pthread_cond_wait(&cond, &mt);
	pthread_mutex_unlock(&mt);
}