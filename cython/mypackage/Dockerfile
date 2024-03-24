FROM python:3.12

WORKDIR /home/user

COPY requirements.txt .

# build-essential python3-dev are from instrucitons on installing Cython.
# gfortran because we copied the meson.build configuration from scipy.
# ninja-build because we need the ninja-backend
RUN apt-get update -y && \
    apt-get install vim -y && \
    apt-get install build-essential python3-dev -y && \
    apt-get install gfortran -y && \
    apt-get install ninja-build -y && \
    pip install -r requirements.txt


# Manually creating a user. The other option is to run with
# -u $(id -u ${USER}):$(id -g ${USER})
RUN useradd --shell /bin/bash user
USER user
ENV PATH=$PATH:/home/user/.local/bin

