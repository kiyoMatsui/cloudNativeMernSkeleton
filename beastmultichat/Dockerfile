FROM alpine:3.12 as base_build

RUN apk add --no-cache libstdc++ make g++ cmake openssl-dev ninja linux-headers boost-dev



#FROM alpine:3.12 as deployment


COPY ./ /root
RUN mkdir build
WORKDIR /root/build
RUN cmake ..
RUN make
EXPOSE 8080

CMD ["/root/build/src/beastserver", "0.0.0.0", "8080", "/root/src", "2"]