FROM ubuntu:18.04
ARG branch=laomao/v1.8.0-rc2-blacklist-plugin

RUN apt-get update && apt-get -y install sudo openssl git ca-certificates vim&& rm -rf /var/lib/apt/lists/*
RUN git clone -b $branch https://github.com/EOSLaoMao/eos.git --recursive

WORKDIR /
RUN ./eos/scripts/eosio_build.sh -s EOS -y -P \
    && ./eos/scripts/eosio_install.sh \
    && mv /root/opt/eosio/bin/nodeos /usr/local/bin \
    && mv /root/opt/eosio/bin/keosd /usr/local/bin \
    && mv /root/opt/eosio/bin/cleos /usr/local/bin \
    && rm -rf ~/* \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /eos
