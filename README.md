# MCD - Modern CD
Easy alternative for long path aliases in shell configuration files (boring).

## Motivation
You may just want faster way to navigate in your linux filesystem (like I did), or just get rid of always making alias for a path in your shell configuration file. This software is exactly for that, and uses SQLite to store the paths.

I've noticed [`weezel`](https://github.com/weezel) has made same kind of project, and thought it would be fun to create it myself.
Credits to him. 
I don't exactly wan't to *copy* him, but to practice my C skills, to be clear.

Myself, i don't really _love_ moving along the linux filesystem when it comes to longer paths.
To prevent this, this app can store your most used paths to local SQLite database and you can access them later easily with custom `alias`.

## Installation 
The easiest way to install is __Github's releases__ feature. That way you can download already compiled binary, and do whatever you want to do. More information below at `Downloading from release`.

Another possibility is to download and compile manually from source. You can do that if you want to contribute or just want to check source code yourself before running. 

### Downloading from release
You can download the most latest version from github's [`releases`](https://github.com/toke8677/mcd/releases) tab.
**Optional**: After that, you may want to move mcd to `/usr/local/bin/`, so you can access it later just by running `mcd` and not `/path/to/mcd`. This can be done by running `mv mcd /usr/local/bin/mcd`.

### Installing from source
This option can be better for development, contributing or you may just want to be sure you downloaded correct binary. 

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
# clean object files and compile 
$ make clean all 

# Optional: make available in PATH env
$ ln -s mcd /usr/local/bin/mcd
```

## Documentation
It *may* be easier to read a simple documentation than reading the source code to learn how this works.

### Using an alias
To use a path, you can use a simple shell trick called command substition like following:

```shell
$ cd $(./mcd [alias]) 
```

`./mcd [alias]`  returns the `path` linked to given `alias` argument, command substition then replaces the command inside `$()` with the command's output so that `cd` can use it.

For more information about command substition, check [this](https://www.linuxjournal.com/article/7385) article's "command substition" entry.

### Creating a new path
You can easily save a new path for later like following

```sh
# usage: mcd [flags] [alias] [long path] 
$ ./mcd website /var/www/html 
```

### Deleting a new path
To delete a path, just put `-d` flag

```sh
# usage: mcd [flags] [alias] [long path] 
$ ./mcd -d website
```

### Updating existing path
To update an existing path, just reassign a value to it like following

```sh
$ ./mcd website /var/www/my_website
```

## File structure
What does any of these files even contain?

### main.c
`main.c` contains the *core* of this program. It determines how things go, in which order, and handles errors.

### tests.c
`tests.c` contains some tests for database and the base program

### database.c
`database.c` contains the database handler and other database related stuff.

### utils.c
`utils.c` contains some helping functions for other modules such as help menu, etc.
