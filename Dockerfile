# Pull base image
FROM agodio/itba-so:1.0
LABEL maintainer="ichayer <ichayer@itba.edu.ar>"

# Set work directory
WORKDIR /code

# Install dependencies
RUN apt -y install minisat
