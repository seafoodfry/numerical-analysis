FROM ubuntu:22.04

WORKDIR /tmp

copy gsl_key.txt .

RUN apt-get update -y && \
    apt-get install vim less wget gnupg build-essential -y && \
    wget https://ftp.gnu.org/gnu/gsl/gsl-2.7.tar.gz && \
    wget https://ftp.gnu.org/gnu/gsl/gsl-2.7.tar.gz.sig

RUN gpg --import gsl_key.txt && \
    gpg --verify gsl-2.7.tar.gz.sig && \
    tar -xzvf gsl-2.7.tar.gz
    
RUN cd gsl-2.7 && \
    ./configure && \
    make && \
    make install
