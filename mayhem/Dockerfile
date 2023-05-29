# Build Stage:
FROM --platform=linux/amd64 ubuntu:20.04 as builder

## Install build dependencies.
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y make clang golang ca-certificates

## Add Source Code
ADD . hebcal
WORKDIR /hebcal/

USER root

## Build Step
RUN make install && \
    make all && \
    chmod +rwx sample_in.txt


# Package Stage
FROM --platform=linux/amd64 ubuntu:20.04
COPY --from=builder /hebcal /hebcal
COPY --from=builder /hebcal/sample_in.txt /hebcal
RUN chmod +rwx /hebcal/sample_in.txt