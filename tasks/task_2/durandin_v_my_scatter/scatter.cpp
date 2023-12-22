// Copyright 2023 Durandin Vladimir
#include "task_2/durandin_v_my_scatter/scatter.h"

int my_scatter::MPI_Scatter(const void* sendbuf, int sendcount,
                            MPI_Datatype sendtype, void* recvbuf, int recvcount,
                            MPI_Datatype recvtype, int root, MPI_Comm comm) {
  int rank, proc_count;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

  if (rank == root)
    if (sendbuf == nullptr) return MPI_ERR_BUFFER;
  if (sendcount <= 0 || recvcount <= 0 || (sendcount > recvcount))
    return MPI_ERR_COUNT;
  if (sendtype == MPI_DATATYPE_NULL || recvtype == MPI_DATATYPE_NULL ||
      (sendtype != recvtype))
    return MPI_ERR_TYPE;
  if (root < 0 || root >= proc_count) return MPI_ERR_ROOT;

  if (rank == root) {
    int mpi_type_size = 0;
    MPI_Type_size(sendtype, &mpi_type_size);
    for (int i = 0; i < proc_count; ++i) {
      if (i != rank)
        MPI_Send(reinterpret_cast<const char*>(sendbuf) +
                     i * sendcount * mpi_type_size,
                 sendcount, sendtype, i, i, comm);
      else
        std::memcpy(recvbuf,
                    reinterpret_cast<const char*>(sendbuf) +
                        i * sendcount * mpi_type_size,
                    static_cast<size_t>(sendcount) * mpi_type_size);
    }
  } else {
    MPI_Status status;
    status.MPI_ERROR = MPI_SUCCESS;
    MPI_Recv(recvbuf, recvcount, recvtype, root, rank, comm, &status);
    if (status.MPI_ERROR != MPI_SUCCESS) {
      std::cout << "An error has occurred in the function MPI_Recv";
      return MPI_ERR_IN_STATUS;
    }
  }

  return MPI_SUCCESS;
}

int my_scatter::Binary_MPI_Scatter(const void* sendbuf, int sendcount,
                                   MPI_Datatype sendtype, void* recvbuf,
                                   int recvcount, MPI_Datatype recvtype,
                                   int root, MPI_Comm comm) {
  int rank, proc_count;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

  if (rank == root)
    if (sendbuf == nullptr) return MPI_ERR_BUFFER;
  if (sendcount <= 0 || recvcount <= 0 || (sendcount > recvcount))
    return MPI_ERR_COUNT;
  if ((sendtype != recvtype) || sendtype == MPI_DATATYPE_NULL ||
      recvtype == MPI_DATATYPE_NULL)
    return MPI_ERR_TYPE;
  if (root < 0 || root >= proc_count) return MPI_ERR_ROOT;

  int local_root = (proc_count - 1) >> 1;
  int parent = -1;
  int left_son = -1;
  int rigth_son = -1;

  int size_of_send = sendcount * proc_count;

  int data_type_size = 0;
  MPI_Type_size(sendtype, &data_type_size);

  int position = rank * recvcount * data_type_size;
  int tmp_position = position;

  // std::cout << "Hello from rank = " << rank << std::endl;
  // if (rank == 1)
  //{
  //  std::cout << "Position = " << position << std::endl;
  //  std::cout << "local_root = " << local_root << std::endl;
  //}

  std::vector<char> local_send_buf(
      size_of_send /
      static_cast<int>(std::pow(
          2,
          Binary_Search(0, proc_count - 1, &rank, &parent, &left_son,
                        &rigth_son, &position, &data_type_size, &recvcount))) *
      data_type_size);
  // if (rank == 4)
  //{
  //  std::cout << "Rank = " << rank << " , loc_sen_buf_size =" <<
  //  local_send_buf.size() << std::endl; std::cout << "Rank = " << rank << " ,
  //  parent = " << parent << std::endl; std::cout << "Rank = " << rank << " ,
  //  position = " << position << std::endl; std::cout << "Rank = " << rank << "
  //  , size_of_send = " << size_of_send << std::endl;
  //  //std::cout << "sendcount * proc_count = " << sendcount * proc_count <<
  //  std::endl; std::cout << "Rank = " << rank << " , std::pow = " <<
  //  std::pow(2, Binary_Search(0, proc_count - 1, rank, parent, left_son,
  //  rigth_son, position, data_type_size, recvcount)) << std::endl;
  //  //std::cout << "Rank = " << rank << " , size_buf = " <<
  //  local_send_buf.size() / data_type_size<< " , type_size = " <<
  //  data_type_size << std::endl;
  //}

  if (local_root != root) {
    if (rank == root) {
      // MPI_Send(reinterpret_cast<const char*>(sendbuf), sendcount *
      // data_type_size, sendtype, local_root, root, MPI_COMM_WORLD);
      // MPI_Send(sendbuf, sendcount, sendtype, local_root, root,
      // MPI_COMM_WORLD);

      // MPI_Send(reinterpret_cast<const char*>(sendbuf), sendcount, sendtype,
      // local_root, root, MPI_COMM_WORLD); std::cout << "Rank = " << rank << "
      // , size_of_send * data_type_size = " << size_of_send * data_type_size <<
      // std::endl;
      MPI_Send(reinterpret_cast<const char*>(sendbuf),
               size_of_send * data_type_size, MPI_CHAR, local_root, root,
               MPI_COMM_WORLD);
      // std::cout << "send * data_size = " << sendcount * data_type_size <<
      // std::endl;

      // std::cout << "send * data_size = " << size_of_send * data_type_size <<
      // std::endl; std::vector<char> my_send_buf(size_of_send *
      // data_type_size);
      // std::memcpy(my_send_buf.data(), sendbuf, my_send_buf.size());
      // std::vector<int64_t> tmp(size_of_send);
      // for (int i = 0; i < tmp.size(); ++i)
      //{
      //  std::memcpy(tmp.data() + i, my_send_buf.data() + i * data_type_size,
      //  data_type_size); std::cout << "tmp[" << i << "] = " << tmp[i] <<
      //  std::endl;
      //}
      // std::cout << std::endl;
    } else if (rank == local_root) {
      MPI_Status status;
      status.MPI_ERROR = MPI_SUCCESS;
      // MPI_Recv(local_send_buf.data(), local_send_buf.size(), MPI_CHAR, root,
      // root, MPI_COMM_WORLD, &status); std::cout << "Rank = " << rank << " ,
      // size_of_send * data_type_size = " << size_of_send * data_type_size <<
      // std::endl;
      MPI_Recv(reinterpret_cast<char*>(local_send_buf.data()),
               size_of_send * data_type_size, MPI_CHAR, root, root,
               MPI_COMM_WORLD, &status);

      if (left_son != -1) {
        MPI_Send(local_send_buf.data(),
                 local_send_buf.size() / 2 -
                     ((local_send_buf.size() / 2) % data_type_size),
                 MPI_CHAR, left_son, 0, MPI_COMM_WORLD);
      }
      if (rigth_son != -1) {
        // MPI_Send(local_send_buf.data() + local_send_buf.size() / 2 *
        // data_type_size, local_send_buf.size() / 2, MPI_CHAR, rigth_son, 0,
        // MPI_COMM_WORLD);
        MPI_Send(local_send_buf.data() + (local_send_buf.size() / 2) -
                     (((local_send_buf.size() / 2) % data_type_size)),
                 local_send_buf.size() / 2 -
                     ((local_send_buf.size() / 2) % data_type_size),
                 MPI_CHAR, rigth_son, 0, MPI_COMM_WORLD);
      }
      std::memcpy(recvbuf,
                  local_send_buf.data() + rank * data_type_size * recvcount,
                  recvcount * data_type_size);
      // std::cout << "Rank = " << rank << " , local_send_buf.size() / 2 -
      // ((local_send_buf.size() / 2) % data_type_size) = " <<
      // local_send_buf.size() / 2 - ((local_send_buf.size() / 2) %
      // data_type_size) << std::endl; std::cout << "Rank = " << rank << ",
      // loc_size = " << local_send_buf.size() << std::endl; std::cout << "Rank
      // = " << rank << ", position = " << position << std::endl;
    }
  } else {
    if (rank == root) {
      std::memcpy(recvbuf, reinterpret_cast<const char*>(sendbuf) + position,
                  recvcount * data_type_size);
      if (left_son != -1) {
        // MPI_Send(local_send_buf.data(), local_send_buf.size() / 2, MPI_CHAR,
        // left_son, 0, MPI_COMM_WORLD);
        MPI_Send(reinterpret_cast<const char*>(sendbuf),
                 local_send_buf.size() / 2 -
                     ((local_send_buf.size() / 2) % data_type_size),
                 MPI_CHAR, left_son, 0, MPI_COMM_WORLD);
      }
      if (rigth_son != -1) {
        // MPI_Send(local_send_buf.data() + local_send_buf.size() / 2 *
        // data_type_size, local_send_buf.size() / 2, MPI_CHAR, rigth_son, 0,
        // MPI_COMM_WORLD); MPI_Send(local_send_buf.data() +
        // local_send_buf.size() / 2, local_send_buf.size() / 2, MPI_CHAR,
        // rigth_son, 0, MPI_COMM_WORLD);
        MPI_Send(
            reinterpret_cast<const char*>(sendbuf) + local_send_buf.size() / 2,
            local_send_buf.size() / 2 -
                ((local_send_buf.size() / 2) % data_type_size),
            MPI_CHAR, rigth_son, 0, MPI_COMM_WORLD);
      }
      // std::cout << "Rank = " << rank << " , loc_sen_buf_size = " <<
      // local_send_buf.size() << std::endl;
    }
  }

  if (rank != local_root /* && rank != root*/) {
    MPI_Recv(reinterpret_cast<char*>(local_send_buf.data()),
             local_send_buf.size(), MPI_CHAR, parent, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    // if (rank < parent)
    // std::memcpy(recvbuf, local_send_buf.data() + rank * data_type_size *
    // recvcount, recvcount * data_type_size); std::memcpy(recvbuf,
    // local_send_buf.data() + position, recvcount * data_type_size);
    // else
    // std::memcpy(recvbuf, local_send_buf.data() - local_send_buf.size() * 2 +
    // rank * data_type_size * recvcount, recvcount * data_type_size);
    // std::memcpy(recvbuf, local_send_buf.data() + position, recvcount *
    // data_type_size);
    std::memcpy(recvbuf, local_send_buf.data() + position,
                recvcount * data_type_size);

    // if (rank == 4)
    //{
    //  std::cout << "Rank = " << rank << " , Position = " << position <<
    //  std::endl; std::cout << "Rank = " << rank << " , local_send_buf.size() =
    //  " << local_send_buf.size() << std::endl;

    //  std::vector<int64_t> tmp(local_send_buf.size() / data_type_size);
    //  std::memcpy(tmp.data(), local_send_buf.data(), local_send_buf.size());

    //  for (int i = 0; i < tmp.size(); ++i)
    //  {
    //    std::memcpy(tmp.data() + i, local_send_buf.data() + i *
    //    data_type_size, data_type_size); std::cout << "Rank = " << rank << "
    //    tmp[" << i << "] = " << tmp[i] << '\n';
    //  }
    //  std::cout << std::endl;
    //  //for (int i = 0; i < recvcount; ++i)
    //  //{
    //  //  std::cout << reinterpret_cast<int64_t*>(recvbuf)[i] << ' ';
    //  //}
    //  //std::cout << std::endl;
    //}

    //  std::vector<int64_t> tmp(local_send_buf.size() / data_type_size);
    //  std::memcpy(tmp.data(), local_send_buf.data(), local_send_buf.size());

    //  for (int i = 0; i < tmp.size(); ++i)
    //  {
    //    std::memcpy(tmp.data() + i, local_send_buf.data() + i *
    //    data_type_size, data_type_size); std::cout << "tmp[" << i << "] = " <<
    //    tmp[i] << '\n';
    //  }
    //  std::cout << std::endl;
    //}

    if (left_son != -1) {
      // MPI_Send(local_send_buf.data(), local_send_buf.size() / 2, MPI_CHAR,
      // left_son, 0, MPI_COMM_WORLD);
      MPI_Send(local_send_buf.data(),
               local_send_buf.size() / 2 -
                   ((local_send_buf.size() / 2) % data_type_size),
               MPI_CHAR, left_son, 0, MPI_COMM_WORLD);
    }
    if (rigth_son != -1) {
      // MPI_Send(local_send_buf.data() + local_send_buf.size() / 2 *
      // data_type_size, local_send_buf.size() / 2, MPI_CHAR, rigth_son, 0,
      // MPI_COMM_WORLD); std::cout << "Rank = " << rank << " ,
      // local_send_buf.size() / 2 = " << local_send_buf.size() / 2 -
      // ((local_send_buf.size() / 2) % data_type_size) << std::endl;
      // MPI_Send(local_send_buf.data() + local_send_buf.size() / 2,
      // local_send_buf.size() / 2 - ((local_send_buf.size() / 2) %
      // data_type_size), MPI_CHAR, rigth_son, 0, MPI_COMM_WORLD);
      // MPI_Send(local_send_buf.data() + local_send_buf.size() / 2 -
      // ((local_send_buf.size() / 2) % data_type_size), local_send_buf.size() /
      // 2 - ((local_send_buf.size() / 2) % data_type_size), MPI_CHAR,
      // rigth_son, 0, MPI_COMM_WORLD); MPI_Send(local_send_buf.data() +
      // static_cast<int>(ceil(local_send_buf.size() / 2.0)) -
      // (static_cast<int>(ceil(local_send_buf.size() / 2.0)) % data_type_size),
      // local_send_buf.size() / 2 - ((local_send_buf.size() / 2) %
      // data_type_size), MPI_CHAR, rigth_son, 0, MPI_COMM_WORLD);

      // if ((local_send_buf.size() / 2 / data_type_size) % 2 == 0)
      //  MPI_Send(local_send_buf.data() + (local_send_buf.size() / 2) -
      //  (((local_send_buf.size() / 2) % data_type_size)),
      //  local_send_buf.size() / 2 - ((local_send_buf.size() / 2) %
      //  data_type_size), MPI_CHAR, rigth_son, 0, MPI_COMM_WORLD);
      // else if (tmp_position < (local_send_buf.size() / 2) -
      // (((local_send_buf.size() / 2) % data_type_size)))
      //  MPI_Send(local_send_buf.data() + (local_send_buf.size() / 2) -
      //  (((local_send_buf.size() / 2) % data_type_size)) - data_type_size,
      //  local_send_buf.size() / 2 - ((local_send_buf.size() / 2) %
      //  data_type_size), MPI_CHAR, rigth_son, 0, MPI_COMM_WORLD);
      // else
      //  MPI_Send(local_send_buf.data() + (local_send_buf.size() / 2) -
      //  (((local_send_buf.size() / 2) % data_type_size)) + data_type_size,
      //  local_send_buf.size() / 2 - ((local_send_buf.size() / 2) %
      //  data_type_size), MPI_CHAR, rigth_son, 0, MPI_COMM_WORLD);

      MPI_Send(local_send_buf.data() + (local_send_buf.size() / 2) -
                   (((local_send_buf.size() / 2) % data_type_size)),
               local_send_buf.size() / 2 -
                   ((local_send_buf.size() / 2) % data_type_size),
               MPI_CHAR, rigth_son, 0, MPI_COMM_WORLD);
    }
  }
  return MPI_SUCCESS;
}

int my_scatter::Binary_Search(int left, int right, const int* rank,
                              int* _parent, int* _left_son, int* _right_son,
                              int* _position, const int* _type_size,
                              const int* _recvcount) {
  if (left > right) {
    std::cout << "Binary_Search - incorrect boundaries!, left > right"
              << std::endl;
    exit(-1);
  }

  int hops = 0;
  int mid = *_parent;
  int parent = -1;

  int l = 0;
  int r = 11;

  // if (rank == 1)
  //  std::cout << "V binare position = " << _position << std::endl;
  while (left <= right) {
    // if (rank == 3)
    //{
    //  std::cout << "Pos = " << _position << std::endl;
    //}
    mid = (left + right) / 2;
    if (*rank == mid) {
      *_parent = parent;
      int new_left = mid + 1;
      int new_right = mid - 1;
      if (new_left <= right) {
        *_right_son = (new_left + right) / 2;
      } else {
        *_right_son = -1;
      }

      if (left <= new_right) {
        *_left_son = (left + new_right) / 2;
      } else {
        *_left_son = -1;
      }

      return hops;
    } else if (*rank < mid) {
      right = mid - 1;
      if ((r - l) % 2 != 0)
        r = (r - l) / 2 + (r - l) % 2 - 1;
      else
        r = (r - l) / 2;
    } else {
      *_position -= ((right - left) / 2 + 1) * (*_recvcount) * (*_type_size);
      // _position -= ((right - left) * _recvcount * _type_size / 2) +
      // _recvcount
      // * _type_size;
      left = mid + 1;
      // l = (r - l + 1) / 2;
      if ((r - l) % 2 != 0)
        l = (r - l) / 2 - (r - l) % 2 + 1;
      else
        l = (r - l) / 2;
      // _position = _position - l * _type_size;

      // if (rank == 1)
      //  std::cout << "V binare position = " << _position << std::endl;
    }

    parent = mid;
    ++hops;
  }
  return hops;
}

std::vector<int64_t> create_random_vector_int64(const size_t size,
                                                const int64_t min,
                                                const int64_t max) {
  if (size == 0ull) return std::vector<int64_t>();
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_int_distribution<int64_t> distance(min, max);

  std::vector<int64_t> vector(size);
  for (size_t i = 0ull; i < size; ++i) vector[i] = distance(generator);
  return vector;
}

std::vector<int> create_random_vector_int(const size_t size, const int64_t min,
                                          const int64_t max) {
  if (size == 0ull) return std::vector<int>();
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_int_distribution<int> distance(min, max);

  std::vector<int> vector(size);
  for (size_t i = 0ull; i < size; ++i) vector[i] = distance(generator);
  return vector;
}

std::vector<float> create_random_vector_float(const size_t size,
                                              const int64_t min,
                                              const int64_t max) {
  if (size == 0ull) return std::vector<float>();
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_real_distribution<float> distance(min, max);

  std::vector<float> vector(size);
  for (size_t i = 0ull; i < size; ++i) vector[i] = distance(generator);
  return vector;
}

std::vector<double> create_random_vector_double(const size_t size,
                                                const int64_t min,
                                                const int64_t max) {
  if (size == 0ull) return std::vector<double>();
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_real_distribution<double> distance(min, max);

  std::vector<double> vector(size);
  for (size_t i = 0ull; i < size; ++i) vector[i] = distance(generator);
  return vector;
}
