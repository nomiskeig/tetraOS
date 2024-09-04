FROM "ubuntu:oracular"

RUN apt-get -y update
RUN apt-get -y upgrade
RUN apt-get -y install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo git automake help2man libtool
RUN git clone https://github.com/autotools-mirror/automake.git
WORKDIR /automake
RUN git checkout tags/v1.15.1
WORKDIR /
RUN git clone https://github.com/autotools-mirror/autoconf.git
WORKDIR /autoconf
RUN git checkout tags/v2.69
RUN autoreconf -vi
RUN ./configure
RUN make; exit 0
RUN make install; exit 0
WORKDIR /automake
RUN ./bootstrap
RUN ./configure
RUN make 
RUN make install 
WORKDIR /
RUN export PATH=$PATH:/home/simon/dev/tetraOS/toolchain/build/out/bin


