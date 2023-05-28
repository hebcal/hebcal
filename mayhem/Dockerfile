# Build Stage:
FROM --platform=linux/amd64 ubuntu:20.04 as builder

## Install build dependencies.
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y make clang golang ca-certificates

## Add Source Code
ADD . hebcal
WORKDIR /hebcal/

## Build Step
# RUN cd hebcal/ && \
#     make install && \
#     make all && \
#     chmod u+r sample_in.txt && \
#     chmod g+r sample_in.txt


# Package Stage
FROM --platform=linux/amd64 ubuntu:20.04
COPY --from=builder /hebcal /