FROM ubuntu:18.04 as builder
ARG branch=laomao/v1.8.0-rc2-blacklist-plugin

RUN apt-get update && apt-get -y install sudo openssl git ca-certificates vim&& rm -rf /var/lib/apt/lists/*
RUN git clone -b $branch https://github.com/EOSLaoMao/eos.git --recursive

WORKDIR /
RUN ./eos/scripts/eosio_build.sh -s EOS -y -P \
    && ./eos/scripts/eosio_install.sh 

FROM ubuntu:18.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssl ca-certificates && rm -rf /var/lib/apt/lists/*
COPY --from=builder /usr/local/lib/* /usr/local/lib/
COPY --from=builder /root/opt/eosio/bin /opt/eosio/bin
ENV EOSIO_ROOT=/opt/eosio
ENV LD_LIBRARY_PATH /usr/local/lib
ENV PATH /opt/eosio/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
