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


WORKDIR /home/user

RUN apt-get update -y && \
    apt-get install bash-completion -y && \
    . /etc/profile.d/bash_completion.sh

# Manually creating a user. The other option is to run with
# -u $(id -u ${USER}):$(id -g ${USER})
# Another good example: https://github.com/jupyter/docker-stacks/blob/b2efa3cd8ce7a2b900f76ff4a17e11a000666cea/images/docker-stacks-foundation/Dockerfile#L72
RUN useradd --no-log-init --create-home --shell /bin/bash --uid 1000 --no-user-group  user && \
    chown -R user /home/user
USER user
ENV PATH=$PATH:/home/user/.local/bin

COPY Basic2D.cpp .
