#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PHILOSOPHER_EATING_TIME_MS  2000
#define PHILOSOPHER_THINKING_TIME_MS  3000
#define MS_TO_US(x) (1000 * x)

#define THINKING    0
#define EATING      1

uint32_t philosopher_count;

pthread_mutex_t *forks;

pthread_t *threads;

typedef struct {
  uint32_t index;
  uint32_t ph_state;
} philosopher_ctx_t;

void *print_state(void *arg) {
  philosopher_ctx_t **phil_ctxs = (philosopher_ctx_t **)arg;

  for(int i=0; i<philosopher_count; i++) {
    printf("phil %2d |", i);
  }
  printf("\n");

  for(int i=0; i<philosopher_count; i++) {
    printf("=========");
  }
  printf("\n");

  while (1) {

    for(int i=0; i<philosopher_count; i++) {
      if ((*phil_ctxs)[i].ph_state == THINKING) {
        printf("THINKING|");
      } else if ((*phil_ctxs)[i].ph_state == EATING) {
        printf("EATING  |");
      }
    }
    printf("\n");

    for(int i=0; i<philosopher_count; i++) {
      printf("=========");
    }
    printf("\n");
    usleep(MS_TO_US(1000));
  }
}

void *do_philosophy(void *arg) {
  philosopher_ctx_t *ctx = (philosopher_ctx_t *)arg;

  const uint32_t left_fork = ctx->index;
  const uint32_t right_fork = (ctx->index + 1) % philosopher_count;

  const uint32_t first_fork = left_fork < right_fork ? left_fork : right_fork;
  const uint32_t sec_fork = first_fork == right_fork ? left_fork : right_fork;

  pthread_mutex_lock(&forks[first_fork]);
  ctx->ph_state = THINKING;

  while (1) {
    pthread_mutex_lock(&forks[sec_fork]);

    ctx->ph_state = EATING;
    usleep(MS_TO_US(PHILOSOPHER_EATING_TIME_MS));
    pthread_mutex_unlock(&forks[first_fork]);
    pthread_mutex_unlock(&forks[sec_fork]);
    ctx->ph_state = THINKING;

    usleep(MS_TO_US(PHILOSOPHER_THINKING_TIME_MS));

    pthread_mutex_lock(&forks[first_fork]);
  }
}

int main(int argc, char **argv) {
  if (argc != 3 || strcmp(argv[1], "-c")) {
    printf("./dinner_time -c [philosophers_count]\n");
    return -1;
  }

  sscanf(argv[2], "%u", &philosopher_count);

  forks = (pthread_mutex_t *)calloc(philosopher_count, sizeof(pthread_mutex_t));
  threads = (pthread_t *)calloc(philosopher_count, sizeof(pthread_t));
  philosopher_ctx_t *philosophers = (philosopher_ctx_t *)calloc(philosopher_count, sizeof(philosopher_ctx_t));

  pthread_t *print_thread = (pthread_t *)calloc(1, sizeof(pthread_t));

  if (!forks || !threads || !philosophers) {
    perror("Memory allocation failed\n");
    return -1;
  }

  for (int i = 0; i < philosopher_count; i++) {
    pthread_mutex_init(&forks[i], NULL);
    philosophers[i].index = i;
  }

  for (int i = 0; i < philosopher_count; i++) {
    pthread_create(&threads[i], NULL, do_philosophy, (void *)&philosophers[i]);
  }

  pthread_create(print_thread, NULL, print_state, (void *)&philosophers);

  usleep(1000000); // Wait for all threads to initialize

  for (int i = 0; i < philosopher_count; i++) {
    pthread_join(threads[i], NULL);
  }

  for (int i = 0; i < philosopher_count; i++) {
    pthread_mutex_destroy(&forks[i]);
  }

  free(forks);
  free(threads);
  free(philosophers);

  return 0;
}
