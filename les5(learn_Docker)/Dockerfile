FROM ubuntu:xenial
MAINTAINER Michail Silkin <silkin.mixail@yandex.ru>
ADD ./install.sh ./install.sh
VOLUME ["/home/michail/HelloWorld"]
RUN apt-get update
RUN apt-get install -y git gcc make
RUN git clone https://github.com/DeyMass/summerSchool
RUN cd summerSchool && make
RUN chmod +x install.sh
CMD cd /summerSchool && cp */bin/*.out /buildResults/
