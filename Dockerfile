# Use a base image with both Node.js and C++ tools
FROM node:20-slim

# Install C++ compiler (clang), make, and build tools
RUN apt-get update && apt-get install -y \
    clang \
    make \
    g++ \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy compiled compiler and source (simplified for now, ideally build in docker)
# We will build inside docker to ensure compatibility
COPY Makefile .
COPY src ./src
COPY rox .

# Build the compiler in the container
RUN make

# Copy web server
COPY web ./web

# Install web dependencies
WORKDIR /app/web
RUN npm install

# Expose port
EXPOSE 3000

# Start server
CMD ["node", "server.js"]
