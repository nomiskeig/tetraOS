
docker build -t tetraos_toolchain_image . 
docker run -it -v /home/simon/dev/tetraOS:/home/simon/dev/tetraOS tetraos_toolchain_image:latest
