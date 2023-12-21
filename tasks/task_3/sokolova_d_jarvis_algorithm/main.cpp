// Copyright 2023 Sokolova Daria
#include <gtest/gtest.h>
#include <vector>
#include "./jarvis_algorithm.h"

TEST(Jarvis_Algorithm, Empty_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<Point> global_vector;
    const vector<int>::size_type size = 0;

    if (rank == 0) {
        global_vector = randomVector(size);
    }

    vector<Point> global_curve = parallelJarvisMarch(global_vector, size);

    if (rank == 0) {
        vector<Point> reference_curve = consistentJarvisMarch(global_vector);

        ASSERT_EQ(global_curve.size(), reference_curve.size());
        for (vector<int>::size_type i = 0; i < global_curve.size(); ++i) {
            ASSERT_EQ(global_curve[i].x, reference_curve[i].x);
            ASSERT_EQ(global_curve[i].y, reference_curve[i].y);
        }
    }
}

TEST(Jarvis_Algorithm, Single_Point) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<Point> global_vector;
    const vector<int>::size_type size = 1;

    if (rank == 0) {
        global_vector = randomVector(size);
    }

    vector<Point> global_curve = parallelJarvisMarch(global_vector, size);

    if (rank == 0) {
        vector<Point> reference_curve = consistentJarvisMarch(global_vector);

        ASSERT_EQ(global_curve.size(), reference_curve.size());
        for (vector<int>::size_type i = 0; i < global_curve.size(); ++i) {
            ASSERT_EQ(global_curve[i].x, reference_curve[i].x);
            ASSERT_EQ(global_curve[i].y, reference_curve[i].y);
        }
    }
}

TEST(Jarvis_Algorithm, VectorSize50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<Point> global_vector;
    const vector<int>::size_type size = 50;

    if (rank == 0) {
        global_vector = randomVector(size);
    }

    vector<Point> global_curve = parallelJarvisMarch(global_vector, size);

    if (rank == 0) {
        vector<Point> reference_curve = consistentJarvisMarch(global_vector);

        ASSERT_EQ(global_curve.size(), reference_curve.size());
        for (vector<int>::size_type i = 0; i < global_curve.size(); ++i) {
            ASSERT_EQ(global_curve[i].x, reference_curve[i].x);
            ASSERT_EQ(global_curve[i].y, reference_curve[i].y);
        }
    }
}

TEST(Jarvis_Algorithm, VectorSize100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<Point> global_vector;
    const vector<int>::size_type size = 100;

    if (rank == 0) {
        global_vector = randomVector(size);
    }

    vector<Point> global_curve = parallelJarvisMarch(global_vector, size);

    if (rank == 0) {
        vector<Point> reference_curve = consistentJarvisMarch(global_vector);

        ASSERT_EQ(global_curve.size(), reference_curve.size());
        for (vector<int>::size_type i = 0; i < global_curve.size(); ++i) {
            ASSERT_EQ(global_curve[i].x, reference_curve[i].x);
            ASSERT_EQ(global_curve[i].y, reference_curve[i].y);
        }
    }
}

TEST(Jarvis_Algorithm, LargeVector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<Point> global_vector;
    const vector<int>::size_type size = 100000;

    if (rank == 0) {
        global_vector = randomVector(size);
    }

    vector<Point> global_curve = parallelJarvisMarch(global_vector, size);

    if (rank == 0) {
        vector<Point> reference_curve = consistentJarvisMarch(global_vector);

        ASSERT_EQ(global_curve.size(), reference_curve.size());
        for (vector<int>::size_type i = 0; i < global_curve.size(); ++i) {
            ASSERT_EQ(global_curve[i].x, reference_curve[i].x);
            ASSERT_EQ(global_curve[i].y, reference_curve[i].y);
        }
    }
}

int main(int argc, char** argv) {
    int resultCode = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    resultCode = RUN_ALL_TESTS();
    MPI_Finalize();

    return resultCode;
}
