# --- Builder stage ---
FROM alpine:latest AS builder

RUN apk add --no-cache \
        build-base \
        cmake \
        git \
        bash \
        ca-certificates
WORKDIR /app
COPY . /app
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-static" && \
    cmake --build build --target json-cli

# --- Runner stage ---
FROM scratch
WORKDIR /app
COPY --from=builder /app/out/json-cli /app/json-cli
ENTRYPOINT ["/app/json-cli"]
