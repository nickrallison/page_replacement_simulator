#include "simulator.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "page.h"


int main_runner(int argc, char *argv[]) {
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
  if (algorithm_chosen == 0) {
    simulator = simulator_new(&page_records, &fcfs_comp_2, 0, 0);
  } else if (algorithm_chosen == 1) {
    simulator = simulator_new(&page_records, &sjn_comp_2, 0, 0);
  } else if (algorithm_chosen == 2) {
    simulator = simulator_new(&page_records, &rr_comp_2, time_quantum, 0);
  } else if (algorithm_chosen == 3) {
    simulator = simulator_new(&page_records, &priority_comp_2, 0, 0);
  } else if (algorithm_chosen == 4) {
    simulator = simulator_new(&page_records, &srt_comp_2, 1, exp_weight);
  } else {
    fprintf(stderr,"Simulator could not be created\n");
    exit(1);
  }

  page_records_t *page_completion_records = simulator_run(&simulator);
  return 0;
}

int main_runner_no_stdin(int argc, char *argv[], char *input) {
  // Checking if the number of arguments is correct
  if (argc != 2 && argc != 3) {
    fprintf(stderr, "Usage: %s (FCFS,SJF,RR,Priority,SRT) [exp_weight | rr time quantum] < (inputfile)\n",
            argv[0]);
    exit(1);
  }

  // Finding which scheduling algorithm was chosen
  char *scheduling_algorithm = argv[1];
  char *scheduling_algorithms[] = {"FCFS", "SJF","RR", "Priority", "SRT"};
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
    simulator = simulator_new(&page_records, &fcfs_comp_2, 0, 0);
  } else if (algorithm_chosen == 1) {
    simulator = simulator_new(&page_records, &sjn_comp_2, 0, 0);
  } else if (algorithm_chosen == 2) {
    simulator = simulator_new(&page_records, &rr_comp_2, time_quantum, 0);
  } else if (algorithm_chosen == 3) {
    simulator = simulator_new(&page_records, &priority_comp_2, 0, 0);
  } else if (algorithm_chosen == 4) {
    simulator = simulator_new(&page_records, &srt_comp_2, 1, exp_weight);
  } else {
    fprintf(stderr,"Simulator could not be created\n");
    exit(1);
  }

  page_records_t *page_completion_records = simulator_run(&simulator);
  return 0;
}
