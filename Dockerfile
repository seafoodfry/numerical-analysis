FROM quay.io/jupyter/scipy-notebook:python-3.11

WORKDIR /home/jovyan/work


# Install system dependencies and tools.
USER root

RUN sudo apt-get update -y && \
    apt-get install vim -y

USER $NB_UID


# Install project dependencies.
COPY requirements.txt .

RUN pip install -r requirements.txt


CMD ["/usr/local/bin/start-notebook.sh"]
