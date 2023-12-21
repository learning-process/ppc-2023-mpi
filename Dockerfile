FROM ubuntu:latest

# Устанавливаем зависимости
RUN apt-get update && \
    apt-get upgrade && \
    apt-get install -y software-properties-common && \
    add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y gcc-12 g++-12 cppcheck ninja-build mpich libmpich mpi openmpi-bin libomp-dev python3-pip valgrind
    
RUN python3 -m pip install --upgrade pip 
RUN python3 -m pip install setuptools cpplint

# Устанавливаем ccache
RUN apt-get install -y ccache

# Копируем код проекта внутрь контейнера
COPY . /app
WORKDIR /app

# Обновляем submodules
RUN git submodule update --init --recursive

# Выполняем настройку CMake
RUN mkdir build
WORKDIR /app/build
RUN cmake -D CMAKE_C_COMPILER_LAUNCHER=ccache -D CMAKE_CXX_COMPILER_LAUNCHER=ccache -G Ninja -D USE_SEQ=ON -D USE_MPI=ON -D USE_OMP=ON -D USE_TBB=ON -D USE_STD=ON -D CMAKE_BUILD_TYPE=RELEASE ..
WORKDIR /app

# Собираем проект с помощью Ninja
RUN export PATH="/usr/lib/ccache:$PATH" && cd build && ninja
WORKDIR /app

# Устанавливаем переменную среды для количества потоков OpenMP
ENV OMP_NUM_THREADS=8

# Запускаем скрипт для запуска тестов
CMD ["scripts/run.sh"]