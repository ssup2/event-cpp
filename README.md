# event-cpp

## Build and run

* Install packages for build
~~~
# apt install cmake
# apt install clang
~~~

* Get source and compile
~~~
# git clone https://github.com/ssup2/event-cpp.git
# cd event-cpp
# mkdir build && cd build
# cmake ..
# make
~~~

* Copy header and lib
~~~
# cp build/framework/lib/* [Target Dir]
# cp build/framework/header/* [Target Dir]
~~~

* Run examples
~~~
# ./build/examples/[Examples]
~~~

