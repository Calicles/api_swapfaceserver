FROM swapface:latest
#RUN mkdir swapface && mkdir swapface/build
#ADD ./build swapface/build
#ADD ./ swapface
#RUN cd swapface/build && cmake .. && make
CMD ["./swapface/build/faceswap"]