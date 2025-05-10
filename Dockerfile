# See https://docs.astral.sh/uv/guides/integration/docker/#available-images
FROM ghcr.io/astral-sh/uv:bookworm

# Same values as docker-stack-foundation.
# See
# https://github.com/jupyter/docker-stacks/blob/aabaa58072623dcc62a71c9e84e3501ab11f5e9d/images/docker-stacks-foundation/Dockerfile#L11-L13
ARG NB_USER="jovyan"
ARG NB_UID="1000"
ARG NB_GID="100"


# Install system dependencies and tools.
USER root

# Create NB_USER with name jovyan user with UID=1000 and in the 'users' group
# and make sure these dirs are writable by the `users` group.
RUN useradd --no-log-init --create-home --shell /bin/bash --uid "${NB_UID}" --no-user-group "${NB_USER}" && \
    chown -R ${NB_USER} "/home/${NB_USER}"

RUN apt-get update -y && \
    apt-get install less vim -y


##############################
### Non-Root Configuration ###
##############################
USER $NB_UID

WORKDIR /home/jovyan/work


COPY --chown=${NB_USER}:${NB_GID} ./hack/start-jupyter-lab.sh "/home/${NB_USER}/"
CMD ["/home/jovyan/start-jupyter-lab.sh"]
