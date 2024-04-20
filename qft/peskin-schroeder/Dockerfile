FROM quay.io/jupyter/scipy-notebook:python-3.11

WORKDIR /tmp

USER root

RUN apt-get update -y && \
    apt-get install vim less wget gnupg build-essential -y


WORKDIR /home/jovyan/work

RUN apt-get update -y && \
    apt-get install bash-completion -y #&& \
#    python3 -m venv /opt/venv
#ENV PATH=/opt/venv/bin:$PATH

# Manually creating a user. The other option is to run with
# -u $(id -u ${USER}):$(id -g ${USER})
# Another good example: https://github.com/jupyter/docker-stacks/blob/b2efa3cd8ce7a2b900f76ff4a17e11a000666cea/images/docker-stacks-foundation/Dockerfile#L72
#RUN useradd --no-log-init --create-home --shell /bin/bash --uid 1000 --no-user-group  user && \
#    chown -R user /home/user && \
#    chown -R user /opt/venv

#USER user
USER $NB_UID

#ENV PATH=/opt/venv/bin:$PATH
ENV PATH=$PATH:/home/user/.local/bin

COPY deps/requirements.txt .
RUN . /etc/profile.d/bash_completion.sh && \
    pip install -Ur requirements.txt
