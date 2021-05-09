FROM gcc:latest

USER root
COPY ./doc/install-dependencies.sh /usr/
RUN chmod +x /usr/install-dependencies.sh

RUN apt-get update
RUN /usr/install-dependencies.sh

RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

WORKDIR .
COPY . .

RUN mkdir build && cd build
RUN cmake ../

EXPOSE 3000