#include "simulator.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "page.h"

// int opt_comp(const page_record_t* a)
// {
//   printf("opt_comp Not implemented\n");
//   exit(1);
// }
//
// int fifo_comp(const page_record_t* a)
// {
//   return -(int) a->arrival_time;
// }
//
// int lru_comp(const page_record_t* a){
//   printf("lru_comp Not implemented\n");
//   exit(1);
// }
//
// int clk_comp(const page_record_t* a){
//   printf("clk_comp Not implemented\n");
//   exit(1);
// }

simulator_stats_t main_runner(int argc, char *argv[]) {
  // Checking if the number of arguments is correct
  if (argc != 2) {
    fprintf(stderr, "Usage: %s (OPT,FIFO,LRU,CLK) < (inputfile)\n",
            argv[0]);
    exit(1);
  }

  // Finding which scheduling algorithm was chosen
  char *scheduling_algorithm = argv[1];
  char *scheduling_algorithms[] = {"OPT", "FIFO","LRU", "CLK"};
  int num_scheduling_algorithms =
      sizeof(scheduling_algorithms) / sizeof(char *);
  int algorithm_chosen = 0;

  while (algorithm_chosen < num_scheduling_algorithms) {
    if (!strcmp(scheduling_algorithm,
                scheduling_algorithms[algorithm_chosen])) {
      break;
    }
    algorithm_chosen++;
  }
  // erroring if none of the valid choices are used
  if (algorithm_chosen == num_scheduling_algorithms) {
    fprintf(stderr,
            "Invalid scheduling algorithm: %s, please choose one of the "
            "following: ",
            scheduling_algorithm);
    int i = 0;

    while (i < num_scheduling_algorithms - 1) {
      fprintf(stderr, "%s, ", scheduling_algorithms[i]);
      i++;
    }
    fprintf(stderr, "%s\n", scheduling_algorithms[i]);
    exit(1);
  }


  page_records_t page_records = create_page_records();

  simulator_t simulator;
  uint32_t cache_capacity = 32;
  uint32_t interrupt_interval = 30;
  if (algorithm_chosen == 0) {
    simulator = simulator_new(&page_records, 0, cache_capacity, interrupt_interval);
  } else if (algorithm_chosen == 1) {
    simulator = simulator_new(&page_records, 1, cache_capacity, interrupt_interval);
  } else if (algorithm_chosen == 2) {
    simulator = simulator_new(&page_records, 2, cache_capacity, interrupt_interval);
  } else if (algorithm_chosen == 3) {
    simulator = simulator_new(&page_records, 3, cache_capacity, interrupt_interval);
  } else {
    fprintf(stderr,"Simulator could not be created\n");
    exit(1);
  }

  simulator_stats_t stats = simulator_run(&simulator);
  return stats;
}

simulator_stats_t main_runner_no_stdin(int argc, char *argv[], char *input, uint32_t cache_capacity, uint32_t interrupt_interval) {
  // Checking if the number of arguments is correct
  if (argc != 2) {
    fprintf(stderr, "Usage: %s (OPT,FIFO,LRU,CLK) < (inputfile)\n",
            argv[0]);
    exit(1);
  }

  // Finding which scheduling algorithm was chosen
  char *scheduling_algorithm = argv[1];
  char *scheduling_algorithms[] = {"OPT", "FIFO","LRU", "CLK"};
  int num_scheduling_algorithms =
      sizeof(scheduling_algorithms) / sizeof(char *);
  int algorithm_chosen = 0;

  while (algorithm_chosen < num_scheduling_algorithms) {
    if (!strcmp(scheduling_algorithm,
                scheduling_algorithms[algorithm_chosen])) {
      break;
    }
    algorithm_chosen++;
  }
  // erroring if none of the valid choices are used
  if (algorithm_chosen == num_scheduling_algorithms) {
    fprintf(stderr,
            "Invalid scheduling algorithm: %s, please choose one of the "
            "following: ",
            scheduling_algorithm);
    int i = 0;

    while (i < num_scheduling_algorithms - 1) {
      fprintf(stderr, "%s, ", scheduling_algorithms[i]);
      i++;
    }
    fprintf(stderr, "%s\n", scheduling_algorithms[i]);
    exit(1);
  }


  page_records_t page_records = create_page_records_no_stdin(input);

  simulator_t simulator;
  if (algorithm_chosen == 0) {
    simulator = simulator_new(&page_records, 0, cache_capacity, interrupt_interval);
  } else if (algorithm_chosen == 1) {
    simulator = simulator_new(&page_records, 1, cache_capacity, interrupt_interval);
  } else if (algorithm_chosen == 2) {
    simulator = simulator_new(&page_records, 2, cache_capacity, interrupt_interval);
  } else if (algorithm_chosen == 3) {
    simulator = simulator_new(&page_records, 3, cache_capacity, interrupt_interval);
  } else {
    fprintf(stderr,"Simulator could not be created\n");
    exit(1);
  }

  simulator_stats_t stats = simulator_run(&simulator);
  return stats;

}
