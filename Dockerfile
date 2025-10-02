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
RUN bash bin/build.sh

# --- Runner stage ---
FROM alpine:latest AS runner
WORKDIR /app
COPY --from=builder /app/out/json-cli /app/json-cli
ENTRYPOINT ["/app/json-cli"]
