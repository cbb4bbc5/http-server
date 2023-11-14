# HTTP-server #

> simple C++ HTTP server written for computer networks class

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Installation ##

``` shell
git clone https://github.com/cbb4bbc5/http-server.git
```

or if you use SSH

``` shell
git clone git@github.com:cbb4bbc5/http-server.git
```

``` shell
cd http-server
make
```

## Usage ##

The program was developed for usage in Linux environment so it has to be run
there.

Firstly, create a directory containing sample webpages. It can be anywhere on
the system where the user can access. Then add contents of this directory (the
names of main subdirectories) to `/etc/hosts` so that they are associated with
`127.0.1.1`

Finally, run `./server port_number webpages` where `webpages` refers to folder
with page sources.
