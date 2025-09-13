# Use the official GCC image with a version that supports C++20
FROM gcc:12

# Update package list and install necessary tools
RUN apt-get update && \
    apt-get install -y \
    git \
    cmake \
    valgrind \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Set specific compiler versions (gcc-12 and g++-12)
ENV CC=/usr/bin/gcc-12
ENV CXX=/usr/bin/g++-12
ENV CXXFLAGS="-std=c++20"

# Create symlinks to ensure generic names work
RUN ln -sf /usr/bin/gcc-12 /usr/bin/gcc && \
    ln -sf /usr/bin/g++-12 /usr/bin/g++

# Create a workspace directory
WORKDIR /workspace

# Default command (can be overridden)
CMD ["/bin/bash"]