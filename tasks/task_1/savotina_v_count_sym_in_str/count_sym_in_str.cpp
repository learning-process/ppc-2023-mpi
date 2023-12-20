// Copyright 2023 Savotina Valeria
#include "task_1/savotina_v_count_sym_in_str/count_sym_in_str.h"

int seqCountSymInString(const std::string& str, char sym) {
  return std::count(str.begin(), str.end(), sym);
}

int parCountSymInString(const std::string& str, const char sym, MPI_Comm comm) {
  if (str.empty()) return 0;

  int rank = 0;
  int sizeWorld = 0;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &sizeWorld);

  const int sizeStr = str.size();
  const int delta = sizeStr / sizeWorld;
  const int remainder = sizeStr % sizeWorld;

  std::vector<int> sendCounts(sizeWorld, delta);
  std::vector<int> displs(sizeWorld);

  for (int proc = 0; proc < sizeWorld; ++proc) {
    if (proc < remainder) ++sendCounts[proc];
    displs[proc] = proc == 0 ? 0 : displs[proc - 1] + sendCounts[proc - 1];
  }

  std::string localStr;
  localStr.resize(sendCounts[rank]);

  MPI_Scatterv(str.data(), sendCounts.data(), displs.data(), MPI_CHAR,
               localStr.data(), sendCounts[rank], MPI_CHAR, 0, comm);

  int localCount = seqCountSymInString(localStr, sym);
  int globalCount = 0;

  MPI_Allreduce(&localCount, &globalCount, 1, MPI_INT, MPI_SUM, comm);

  return globalCount;
}

std::string createRandomString(size_t size, char minSym, char maxSym) {
  std::string str;

  if (size != 0) {
    str.resize(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(minSym, maxSym);

    for (auto& sym : str) sym = static_cast<char>(dist(gen));
  }

  return str;
}
