// Copyright 2023 Mamaeva Olga
#include "../tasks/task_1/mamaeva_o_sum_of_vec_elem/sum_of_vec_elem.h"

std::vector<double> getRandomVector(int n) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<double> vec(n);
    for (int i = 0; i < n; i++) {
        vec[i] = gen();
    }
    return vec;
}


double getSequentialOperations(std::vector<double> vec, int n, const std::string& ops) {
    double sum = 0;
    if (ops == "+") {
        for (int i = 0; i < n; i++) {
            sum += vec[i];
        }
    } else {
        if (ops == "-") {
            for (int i = 0; i < n; i++) {
                sum -= vec[i];
            }
        } else {
            throw "Input error: invalide value of operation";
        }
    }
    return sum;
}

double getParallelOperations(std::vector<double> vec, int n, const std::string& ops) {
    // double start, finish;
    int ProcNum, ProcRank;
    int* dig = nullptr;
    double sum = 0, sum_all = 0;
    // srand(time(NULL));


    // 2 module ------------------------------------------------------------------------------


    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);


    // 3 module ------------------------------------------------------------------------------


    /*if (ProcNum < 2) {
        if (ProcRank == 0) {
            printf("Error: more processes required ");
            fflush(stdout);
        }
        MPI_Abort(MPI_COMM_WORLD, MPI_ERR_OTHER);
    }*/

    // if (ProcRank == 0) {
    //    //scanf_s("%d", &n);
    //    //vec.resize(n);
    //    //vec = getRandomVector(n);
    //    for (int i = 0; i < n; i++) {
    //        //vec[i] = rand() / (double)RAND_MAX;
    //        /*printf("            [%lf]\n", vec[i]);
    //        fflush(stdout);*/
    //    }
    //    start = MPI_Wtime();
    //}
    // MPI_Bcast(&n, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);


    // 4 module ------------------------------------------------------------------------------


    dig = new int[ProcNum];
    for (int i = 0; i < ProcNum; i++) {
        if (i < (n % ProcNum))
            dig[i] = n / ProcNum + 1;
        else
            dig[i] = n / ProcNum;
    }


    //****************************************************************************************

     std::vector<double> vecc(dig[ProcRank]);

    //  MPI_Scatter(vec.data(), dig[ProcRank], MPI_DOUBLE, vecc.data(), dig[ProcRank], MPI_DOUBLE, 0, MPI_COMM_WORLD);


     int tmp = 0;
     int* stepv = new int[ProcNum];
     for (int i = 0; i < ProcNum; i++) {
        stepv[i] = tmp;
        tmp += dig[i];
    }
     MPI_Scatterv(vec.data(), &dig[ProcRank], stepv, MPI_DOUBLE,
        vecc.data(), dig[ProcRank], MPI_DOUBLE, 0, MPI_COMM_WORLD);


    /* for (int in = 0; in < dig[ProcRank]; in++) {
        printf("%d -------------------------- %lf\n", ProcRank, vecc[in]);
    }
     printf("my length: %d\n", dig[ProcRank]);

     printf("             start to work %d\n", ProcRank);
     for (int in = 0; in < dig[ProcRank]; in++) {
        sum += vecc[in];
    }*/
     sum = getSequentialOperations(vecc, dig[ProcRank], ops);


    // ****************************************************************************************


    // top or bottom


    // ****************************************************************************************

    // if (ProcRank != 0) {
    //    vec.resize(dig[ProcRank]);
    //}

    // if (ProcRank == 0) {
    //    int step = 0;
    //    for (int ind = 1; ind < ProcNum; ind++) {
    //        step += dig[ind - 1];
    //        MPI_Send(vec.data() + step, dig[ind], MPI_DOUBLE, ind, 0, MPI_COMM_WORLD);
    //    }
    //} else {
    //    MPI_Status status;
    //    MPI_Recv(vec.data(), dig[ProcRank], MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    //}

    ///* for (int in = 0; in < dig[ProcRank]; in++) {
    //     printf("%d -------------------------- %lf\n", ProcRank, vec[in]);
    // }
    // printf("my length: %d\n", dig[ProcRank]);
    // printf("             start to work %d\n", ProcRank);*/
    // /*for (int in = 0; in < dig[ProcRank]; in++) {
    //     sum += vec[in];
    // }*/
    // sum = getSequentialOperations(vec, dig[ProcRank], ops);


    // 5 module ------------------------------------------------------------------------------


    // ****************************************************************************************


     MPI_Reduce(&sum, &sum_all, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
     /*printf("%d --- sum       ((%lf))\n\n", ProcRank, sum);
     fflush(stdout);*/


     // ****************************************************************************************


     // top or bottom


     // ****************************************************************************************


    // if (ProcRank == 0) {
    //    sum_all = sum;
    //    /*printf("%d --- sum       ((%lf))\n\n", ProcRank, sum);
    //    fflush(stdout);*/
    //    for (int ind = 1; ind < ProcNum; ind++) {
    //        MPI_Status status;
    //        MPI_Recv(&sum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    //        sum_all += sum;
    //    }
    //} else {
    //    MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    //    /* printf("%d --- sum       ((%lf))\n\n", ProcRank, sum);
    //     fflush(stdout);*/
    //}

    // ****************************************************************************************


    // 6 module ------------------------------------------------------------------------------

    // MPI_Barrier(MPI_COMM_WORLD);
    /*if (ProcRank == 0) {
        finish = MPI_Wtime();
        printf("sum_all = %lf\n", sum_all);
        printf("time: %lf\n", finish - start);

        printf("dig: ");
        for (int i = 0; i < ProcNum; i++) {
            printf("%d ", dig[i]);
        }
        printf("\n");
    }*/
    // MPI_Finalize();

    return sum_all;
}
