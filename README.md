# SPI I/O Interfaces for AC483

SPI I/O Interfaces for audio chip 483.

## Build & install

get the source code:

```
git clone https://github.com/ClarenceYk/SPI-AC483-Dev.git
```

autotools:

```
cd SPI-AC483-Dev
autoreconfig --install
```

build:

```
mkdir build-x86 && cd build-x86
../configure
make
```

*If you want to install this library to a custom directory, add `--prefix=[custom directory]` when running `configure`.*

install:

```
make install
```

## Testing

make & running unit testing:

```
../configure --enable-gcov
make clean && make && make check
```


## For cross-compiling(armhf)

Make sure you have installed the `check` library in your cross-compiling environment.

set environment variables:

```
export CC=[gcc in your cross-compiling env]
export PKG_CONFIG_PATH=[pkgconfig in your cross-compiling env]
```

autotools:

```
cd SPI-AC483-Dev
autoreconfig --install
```

build:

```
mkdir build-armhf && cd build-armhf
../configure --prefix=[custom directory] --build x86_64-pc-linux-gun --host arm-linux-gnueabihf
make
```

install:

```
make install
```

## Testing

make & running unit testing:

```
../configure --build x86_64-pc-linux-gun --host arm-linux-gnueabihf --enable-gcov
make clean && make && make check
```

Ignore the error prompts and run the runable file `check-io` under `tests` in your target platform for testing.
