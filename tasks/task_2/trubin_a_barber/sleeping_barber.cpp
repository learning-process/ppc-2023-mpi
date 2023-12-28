// Copyright 2023 Trubin Artem
#include <mpi.h>
#include <random>
#include <stdexcept>
#include "task_2/trubin_a_barber/sleeping_barber.h"

enum signal_type {stop_execute, check_waiting_room, customer_wait, no_customer,
                  wake_up, check_barber, sit_on_chair, no_chair};
enum barber_status {barber_sleeping, barber_cutting};

int sleeping_barber(int number_of_chairs, int need_to_serve, int* not_enough_chair) {
    int rank; int size;
    int served = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (number_of_chairs < 0 || need_to_serve < 0)
        throw std::runtime_error("Invalid negative value");
    if (size < 3)
        throw std::runtime_error("Insufficient number of process");
    if (rank == 1) {
        int flag = 0;
        int mes;
        int send_mes;
        MPI_Status status;
        bool stop_execution = false;
        std::mt19937 gen;
        gen.seed(static_cast<unsigned int>(rank));
        double cutting;
        while (!stop_execution) {
            send_mes = check_waiting_room;
            MPI_Send(&send_mes, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
            MPI_Recv(&mes, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (mes == stop_execute) {
                stop_execution = true;
                break;
            } else if (mes == customer_wait) {  // start cutting
                cutting = (static_cast<double>(gen()%100)/10000);
                sleep(cutting);
            } else {
                int new_mes;
                MPI_Recv(&new_mes, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);  // waiting for wake up
                if (new_mes == stop_execute) {
                    stop_execution = true;
                    break;
                }
            }
            MPI_Iprobe(MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &flag, &status);
            if (flag)
                break;
        }
    } else if (rank > 1) {
        int flag = 0;
        int mes = 0;
        int send_mes;
        MPI_Status status;
        std::mt19937 gen;
        gen.seed(static_cast<unsigned int>(rank));
        bool stop_execution = false;
        double new_customer_arrive;
        while (!stop_execution) {
            new_customer_arrive = (static_cast<double>(gen()%100)/10000);
            sleep(new_customer_arrive);
            MPI_Iprobe(MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &flag, &status);
            if (flag)
                break;
            send_mes = check_barber;
            MPI_Send(&send_mes, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
            // Wake up barber / Sit on chair / No chairs
            MPI_Recv(&mes, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (mes == stop_execute) {
                stop_execution = true;
                break;
            }
        }
    } else if (rank == 0) {
        int used_chairs = 0;
        int mes;  // recieved message
        int send_mes;  // send message
        int flag = 0;  // check incoming messages
        int barber_status = -1;
        MPI_Status status;
        int proc_source;
        while (served < need_to_serve) {
            if (!flag) {
                MPI_Iprobe(MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &flag, &status);
            }
            if (flag) {
                flag = 0;
                proc_source = status.MPI_SOURCE;
                MPI_Recv(&mes, 1, MPI_INT, proc_source, 1, MPI_COMM_WORLD, &status);
                if (mes == check_waiting_room) {  // barber message
                    if (used_chairs == 0) {  // Go to sleep
                        send_mes = no_customer;
                        MPI_Send(&send_mes, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
                        barber_status = barber_sleeping;
                    } else {  // Start cutting
                        used_chairs--;
                        send_mes = customer_wait;
                        MPI_Send(&send_mes, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
                        served++;
                        barber_status = barber_cutting;
                    }
                } else if (mes == check_barber) {  // customer message
                    if (barber_status == barber_sleeping) {
                        send_mes = wake_up;
                        MPI_Send(&send_mes, 1, MPI_INT, proc_source, 1, MPI_COMM_WORLD);
                        MPI_Send(&send_mes, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
                        used_chairs++;
                        barber_status = barber_cutting;
                    } else {
                        if (used_chairs < number_of_chairs) {
                            used_chairs++;
                            send_mes = sit_on_chair;
                            MPI_Send(&send_mes, 1, MPI_INT, proc_source, 1, MPI_COMM_WORLD);
                        } else {
                            *not_enough_chair = *not_enough_chair + 1;
                            send_mes = no_chair;
                            MPI_Send(&send_mes, 1, MPI_INT, proc_source, 1, MPI_COMM_WORLD);
                        }
                    }
                }
            }
        }
        MPI_Iprobe(MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &flag, &status);
        while (flag) {
            MPI_Recv(&mes, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Iprobe(MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &flag, &status);
        }
        send_mes = stop_execute;
        for (int i = 1; i < size; i++)
            MPI_Send(&send_mes, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
    }
    return served;
}

int sleeping_barber_sequential(int number_of_chairs, int need_to_serve, int* not_enough_chair) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(MPI_Wtime()));
    int served = 0;
    int used_chair = 0;
    int barber_status = barber_sleeping;
    bool wake_up_barber = false;
    bool check_hall_and_start = false;
    double prev_time_customer = MPI_Wtime();
    double prev_time_cutting = 1000000;
    double curr_time = 0;
    double cutting = 0;
    double new_customer_arrive = (static_cast<double>(gen()%100)/10000);

    while (served < need_to_serve) {
        curr_time = MPI_Wtime();
        if (curr_time - prev_time_customer > new_customer_arrive) {
            if (barber_status == barber_sleeping)
                wake_up_barber = true;
            if (number_of_chairs >= used_chair)
                used_chair++;
            else
                *not_enough_chair = *not_enough_chair + 1;
            prev_time_customer = MPI_Wtime();
            new_customer_arrive = (static_cast<double>(gen()%100)/10000);
        }
        if (curr_time - prev_time_cutting > cutting) {
            served++;
            check_hall_and_start = true;
        }
        if (wake_up_barber) {
            wake_up_barber = false;
            check_hall_and_start = true;
        }
        if (check_hall_and_start) {
            check_hall_and_start = false;
            if (used_chair > 0) {
                barber_status = barber_cutting;
                prev_time_cutting = MPI_Wtime();
                cutting = (static_cast<double>(gen()%100)/10000);
                used_chair--;
            } else {
                barber_status = barber_sleeping;
            }
        }
    }
    return served;
}

void sleep(double t) {
    double prev_time = MPI_Wtime();
    double curr_time = MPI_Wtime() - prev_time;
    while (curr_time < t) {
        curr_time = MPI_Wtime() - prev_time;
    }
}
