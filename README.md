# MCD - Modern CD
Easy alternative for long path aliases in shell configuration files (boring).

## Motivation
You may just want faster way to navigate in your linux filesystem (like I did), or just get rid of always making alias for a path in your shell configuration file. This software is exactly for that, and uses SQLite to store the paths.

I've noticed [`weezel`](https://github.com/weezel) has made same kind of project, and thought it would be fun to create it myself.
Credits to him. 
I don't exactly wan't to *copy* him, but to practice my C skills, to be clear.

Myself, i don't really _love_ moving along the linux filesystem when it comes to longer paths.
To prevent this, this app can store your most used paths and you can access them later easily.

## Installation 

You can download the most latest version from [`releases`](https://github.com/toke8677/mcd/releases) tab.
**Optional**: After that, you may want to move mcd to `/usr/local/bin/`, so you can access it later just by running `mcd` and not `/path/to/mcd`. 

### Installing from source

#### Download

Though, if you want, you can download the source and build it locally:

```sh
# HTTP - use if you haven't got ssh-keys set up in Github
$ git clone git@github.com:toke8677/mcd.git

# ssh - use if you've got ssh-keys set up in Github
$ git clone git@github.com:toke8677/mcd.git
```

#### Compiling

After download, you should be able to compile the project with make:

```sh
$ make # compile
$ make clean # remove object files (*.o)

# Optional: make available in PATH
$ mv mcd /usr/local/bin/mcd
```

## Usage

So you don't need to read the source code to know how this dumb thing works :)

### Creating a new path

You can easily save a new path for later like following

```sh
# usage: mcd [path alias] [long, long path] [flags] 
$ ./mcd website /var/www/html 
```

### Deleting a new path

To delete a path, just put -d flag

```sh
# usage: mcd [path alias] [long, long path] [flags] 
$ ./mcd website -d
```

### Updating existing path

To update an existing path, just reassign a value to it like following

```sh
$ ./mcd website /var/www/my_website
```

## File structure

What does any of these files even contain?

### main.c

`main.c` contains the *core* of this program. It determines how things should go, in which order etc.

### tests.c

`tests.c` contains some tests for database and the base program

### database.c

`database.c` contains the database handler and other database related stuff.
