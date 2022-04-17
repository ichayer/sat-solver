# Pull base image
FROM agodio/itba-so:1.0
LABEL maintainer="ichayer <ichayer@itba.edu.ar>"

# Set work directory
WORKDIR /code

# Install dependencies
RUN wget -q -O - https://files.pvs-studio.com/etc/pubkey.txt | apt-key add -
RUN wget -O /etc/apt/sources.list.d/viva64.list https://files.pvs-studio.com/etc/viva64.list
RUN apt-get -y install apt-transport-https
RUN apt-get -y update
RUN apt-get -y install pvs-studio
RUN pvs-studio-analyzer credentials "PVS-Studio Free" "FREE-FREE-FREE-FREE"
RUN apt -y install minisat
