FROM ubuntu:24.04

RUN apt-get update
RUN apt-get install -y sudo
RUN apt-get install -y build-essential
RUN apt-get install -y git
RUN apt-get install -y python3
RUN apt-get install -y python3-pip
RUN python3 -m pip config set global.break-system-packages true
RUN pip install ipykernel
RUN pip install pandas
RUN pip install matplotlib
