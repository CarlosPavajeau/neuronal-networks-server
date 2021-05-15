FROM gcc:latest AS build

USER root
COPY ./doc/install-dependencies.sh /usr/
RUN chmod +x /usr/install-dependencies.sh

RUN apt-get update
RUN /usr/install-dependencies.sh

RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

WORKDIR .
COPY . .

RUN mkdir build && cd build
RUN cmake ../ && make

FROM build as deploy
WORKDIR /opt/neuronal-networks-server
COPY --from=build src/NeuronalNetworksServer/ ./

EXPOSE 3000
CMD ["./neuronal_networks_server"]
