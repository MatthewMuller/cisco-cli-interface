# Development Dockerfile for Cisco CLI Interface
FROM ubuntu:22.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV CC=gcc
ENV CFLAGS="-Wall -Wextra -std=c99 -g"

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    libncurses5-dev \
    libncursesw5-dev \
    git \
    make \
    && rm -rf /var/lib/apt/lists/*

# Create app directory
WORKDIR /app

# Copy source code
COPY . .

# Build the application
RUN make clean && make

# Default command (can be overridden by docker-compose)
CMD ["/bin/bash"]
