# Pull base image
FROM agodio/itba-so:1.0
LABEL maintainer="ichayer <ichayer@itba.edu.ar>"

# Set work directory
WORKDIR /code

# Install dependencies
RUN pvs-studio-analyzer credentials "PVS-Studio Free" "FREE-FREE-FREE-FREE"
RUN apt -y install minisat
