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
RUN bash bin/build.sh -t Debug

# --- Runner stage ---
FROM alpine:latest AS runner
WORKDIR /app
COPY --from=builder /app/build/json-cli /app/json-cli
COPY --from=builder /app/build/libjson_c.so* /app/
ENV LD_LIBRARY_PATH=/app
ENTRYPOINT ["/app/json-cli"]
