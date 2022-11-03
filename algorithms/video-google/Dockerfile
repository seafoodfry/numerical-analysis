FROM python:3.11

RUN apt-get update -y && apt-get install bash-completion vim less ffmpeg -y && \
    . /etc/profile.d/bash_completion.sh

WORKDIR /src/
COPY requirements.txt .
RUN pip install -r requirements.txt
