FROM ubuntu:22.04

# Install essential build tools and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    wget \
    git \
    libssl-dev \
    && apt-get clean

# Download and install Boost libraries
RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz \
    && tar xzf boost_1_82_0.tar.gz \
    && cd boost_1_82_0 \
    && ./bootstrap.sh --with-libraries=system,thread,json \
    && ./b2 install \
    && cd .. \
    && rm -rf boost_1_82_0*

# Copy application source code
COPY . /app

# Set working directory
WORKDIR /app

# Set LD_LIBRARY_PATH to include /usr/local/lib
ENV LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"

# Compile the application
RUN g++ -o mollybettechchallenge MollybetTechChallenge.cpp -lboost_system -lboost_thread -lboost_json -lpthread -lssl -lcrypto -std=c++17 -O2

# Set default command
CMD ["./mollybettechchallenge"]
