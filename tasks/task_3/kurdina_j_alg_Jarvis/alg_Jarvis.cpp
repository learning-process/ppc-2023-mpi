// Copyright 2023 Kurdina Julia
#include "task_3/kurdina_j_alg_Jarvis/alg_Jarvis.h"

int cross(vec a, vec b) {
  return a.x * b.y - b.x * a.y;
}

std::vector<point> get_convex_hull(const std::vector<int>& image, int n, int m) {
  std::vector<point> points;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (image[i * m + j] > 120) {
        point p;
        p.x = i;
        p.y = j;
        points.push_back(p);
      }
    }
  }
  for (int i = 1; i < points.size(); ++i) {
    if (points[i].x < points[0].x ||
       (points[i].x == points[0].x && points[i].y < points[0].y)) {
      std::swap(points[0], points[i]);
    }
  }
  std::vector<point> ch;
  int cur = 0;
  while (true) {
    ch.push_back(points[cur]);
    int id = -1;
    for (int i = 0; i < points.size(); ++i) {
      if (i != cur && (id == -1 ||
          cross(vec(points[cur], points[i]), vec(points[cur], points[id])) > 0)) {
        id = i;
      }
    }
    if (id == 0) {
      break;
    }
    cur = id;
  }
  return ch;
}

std::vector<point> get_convex_hull_parallel(const std::vector<int>& image, int n, int m) {
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  std::vector<point> res;
  std::vector<point> points;
  point item1;
  point item4;
  int size = n / ProcNum;
  int ost = n % ProcNum;
  std::vector<int> sendcounts(ProcNum);
  std::vector<int> recvcounts(ProcNum);
  std::vector<int> displs(ProcNum);
  std::vector<int> displs2(ProcNum);
  std::vector<int> part_image(size * m);
  std::vector<point> start_points;
  std::vector<point> from_proc;

  if (ProcRank == 0) {
    for (int i = 0; i < ProcNum; i++) {
      sendcounts[i] = size * m;
      displs[i] = 0;
    }
    sendcounts[0] += ost * m;
    for (int i = 1; i < ProcNum; i++) {
      displs[i] = displs[i - 1] + sendcounts[i - 1];
    }
    part_image.resize((size + ost) * m);
    start_points.resize(ProcNum);
    from_proc.resize(ProcNum);
  }
  MPI_Scatterv(image.data(), sendcounts.data(), displs.data(),
    MPI_INT, part_image.data(), part_image.size(), MPI_INT, 0, MPI_COMM_WORLD);
  int count = size;
  if (ProcRank == 0)
    count += ost;

  for (int i = 0; i < count; i++) {
    for (int j = 0; j < m; j++) {
      if (part_image[i * m + j] > 120) {
        if (ProcRank != 0) {
          point p;
          p.x = (count + ost + (ProcRank - 1) * size) + i;
          p.y = j;
          points.push_back(p);
        } else {
          point p;
          p.x = i;
          p.y = j;
          points.push_back(p);
        }
      }
    }
  }
  for (int i = 1; i < points.size(); ++i) {
    if (points[i].x < points[0].x ||
      (points[i].x == points[0].x && points[i].y < points[0].y)) {
      std::swap(points[0], points[i]);
    }
  }
  point item;
  if (!points.empty()) {
    item = {
    item.x = points[0].x,
    item.y = points[0].y
    };
  } else {
    item = {
    item.x = m + 1,
    item.y = n + 1
    };
  }
  MPI_Gather(&item, sizeof(point), MPI_BYTE,
    start_points.data(), sizeof(point), MPI_BYTE, 0, MPI_COMM_WORLD);
  if (ProcRank == 0) {
    for (int i = 1; i < start_points.size(); ++i) {
      if (start_points[i].x < start_points[0].x ||
        (start_points[i].x == start_points[0].x && start_points[i].y < start_points[0].y)) {
        std::swap(start_points[0], start_points[i]);
      }
    }
    item1 = {
    item1.x = start_points[0].x,
    item1.y = start_points[0].y
    };
  }
  MPI_Bcast(&item1, sizeof(point),
    MPI_BYTE, 0, MPI_COMM_WORLD);
  point cur = item1;
  while (true) {
    if (ProcRank == 0) {
      res.push_back(cur);
    }
    int id = -1;
    for (int i = 0; i < points.size(); i++) {
      if ((points[i].x != cur.x || points[i].y != cur.y) &&
        (id == -1 || cross(vec(cur, points[i]), vec(cur, points[id])) > 0)) {
        id = i;
      }
    }
    point item3;
    if (id != -1) {
      item3 = {
      item3.x = points[id].x,
      item3.y = points[id].y
      };
    } else {
      item3 = {
      item3.x = cur.x,
      item3.y = cur.y
      };
    }
    MPI_Gather(&item3, sizeof(point), MPI_BYTE,
      from_proc.data(), sizeof(point), MPI_BYTE, 0, MPI_COMM_WORLD);
    if (ProcRank == 0) {
      int id = -1;
      for (int i = 0; i < from_proc.size(); i++) {
        if ((from_proc[i].x != cur.x || from_proc[i].y != cur.y) &&
          (id == -1 || cross(vec(cur, from_proc[i]), vec(cur, from_proc[id])) > 0)) {
          id = i;
        }
      }
      item4 = {
      item4.x = from_proc[id].x,
      item4.y = from_proc[id].y
      };
    }
    MPI_Bcast(&item4, sizeof(point), MPI_BYTE, 0, MPI_COMM_WORLD);
    cur = item4;
    if (cur.x == item1.x && cur.y == item1.y)
      break;
  }
  return res;
}

bool check_answers(std::vector<point> not_par, std::vector<point> par) {
  if (not_par.size() != par.size()) return false;
  for (int i = 0; i < not_par.size(); i++) {
    if (not_par[i].x != par[i].x || not_par[i].y != par[i].y)
      return false;
  }
  return true;
}
