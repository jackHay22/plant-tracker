FROM ubuntu:bionic

USER root
RUN apt-get update
RUN apt-get install -y git
RUN apt-get install -y curl wget
RUN apt-get update -y && apt-get install -y --no-install-recommends g++ make cmake pkg-config build-essential zlib1g-dev
RUN apt-get update -y && apt-get install -y --no-install-recommends libcurl4-openssl-dev libssl-dev uuid-dev libpulse-dev libssl1.1

# Install Boost
RUN wget http://downloads.sourceforge.net/project/boost/boost/1.68.0/boost_1_68_0.tar.gz \
    && tar xfz boost_1_68_0.tar.gz \
    && rm boost_1_68_0.tar.gz \
    && cd boost_1_68_0 \
    && ./bootstrap.sh --with-libraries=system,serialization,thread,locale,random,chrono,regex,filesystem \
    && ./b2 install

WORKDIR /
RUN rm -r boost_1_68_0

#install served
RUN git clone https://github.com/reiform/served && \
    cd served && \
    mkdir cmake.build && \
    cd cmake.build && \
    cmake ../ && \
    make -j8 && \
    make install

WORKDIR /
RUN rm -r served

#upgrade cmake to 3.12 for prometheus
RUN wget http://www.cmake.org/files/v3.12/cmake-3.12.1.tar.gz && \
    tar -xvzf cmake-3.12.1.tar.gz && \
    cd cmake-3.12.1/ && \
    ./configure && \
    make -j8 && \
    make install && \
    update-alternatives --install /usr/bin/cmake cmake /usr/local/bin/cmake 1 --force

WORKDIR /
RUN rm -r cmake-3.12.1

#install prometheus C++ Client Library
RUN git clone https://github.com/jupp0r/prometheus-cpp.git && \
    cd prometheus-cpp && \
    git submodule init && \
    git submodule update && \
    mkdir _build && \
    cd _build && \
    cmake .. -DBUILD_SHARED_LIBS=ON && \
    make -j 4 && \
    #ctest -V && \
    mkdir -p deploy && \
    make install

WORKDIR /
RUN rm -r prometheus-cpp

RUN mkdir plant_build
WORKDIR plant_build/

COPY Makefile Makefile
COPY src/ src/
COPY libs/ libs/
RUN make -j8

EXPOSE 8000
EXPOSE 8088

CMD ["./plant_server.out -p 8000"]
