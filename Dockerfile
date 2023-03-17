FROM ubuntu:22.04

RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
    --mount=target=/var/cache/apt,type=cache,sharing=locked \
    rm -f /etc/apt/apt.conf.d/docker-clean \
    && apt update \
    && apt install -y python3 \
    python3-dev \
    python3-setuptools \
    python3-pip \
    make \
    gcc \
    git \
    valgrind

COPY . /usr/src
WORKDIR /usr/src
RUN pip3 install -r requirements.txt -r requirements-dev.txt
