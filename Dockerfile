FROM ubuntu:latest

RUN ["echo", "installing apps"]
RUN ["apt-get", "update"]
RUN ["apt-get", "install", "-y", "sudo"]
RUN ["apt-get", "install", "-y", "g++"]
RUN ["apt-get", "install", "-y", "nano"]
RUN ["apt-get", "install", "-y", "git"]

COPY ./runIt.sh /

ENTRYPOINT  ["./runIt.sh"]