# Jump 
Easy alternative for long path aliases in shell configuration files (boring).

## Motivation
You may just want a faster way to navigate in your filesystem chaos (like I did), or just get rid of always making alias for a path in your shell configuration file. This software is exactly for that, and uses SQLite to store the paths.

I've noticed [`weezel`](https://github.com/weezel) has made same kind of project, and thought it would be fun to create it myself.
Credits to him. 
I don't exactly want to *copy* him, but to practice my C skills, to be clear.

Myself, I don't really _love_ moving along the linux filesystem when it comes to longer paths.
To prevent this, this app can store paths of your choice, so you can access them easier later on.

## Installation 

The easiest way to install is __GitHub's releases__ feature. 
That way you can download already compiled binary, and do whatever you want to do. 
More information below at `Downloading from release`.

Another possibility is to download and compile manually from source. 
You can do that if you want to contribute or just want to check source code yourself before running. 

### Requirements

There is couple of required libraries what you need to install before compiling. They are listed down below.

- libbsd-dev
- libsqlite3-dev

#### libbsd-dev

Gives us `strlcpy(), strlcat()` functions to prevent buffer overflow when managing data inside this program.

#### libsqlite3-dev

`libsqlite3-dev` offers us library which we can treat data and manage connections in database with.

### Downloading from release

You can download the latest version from GitHub's [`releases`](https://github.com/toke8677/jump/releases) tab.

**Optional**: After that, you may want to move jmp to `/usr/local/bin/`, 
so you can access it later just by running `jmp` and not `/path/to/jmp`.
This can be done by running `mv jmp /usr/local/bin/jmp`.

### Installing from source
This option can be better for development, contributing, or you may just want to be sure you downloaded the correct binary. 

#### Download
Though, if you want, you can download the source and build it locally:

```sh
# HTTP - use if you haven't got ssh-keys set up in Github
$ git clone https://github.com/toke8677/jump.git

# ssh - use if you've got ssh-keys set up in Github
$ git clone git@github.com:toke8677/jump.git
```

#### Compiling
After download, you should be able to compile the project with make:

```sh
# clean object files and compile with optimization flags 
$ make clean release

# Optional: make available anywhere
$ cp jump /usr/local/bin/jump
```

## Documentation
Everyone loves documentations. Especially when they are well-written.

### Using an alias

To use a path, you can use a simple shell trick called command substitution like following:

```shell
$ cd $(jmp <alias>)
```

`jmp <alias>` returns the `path` linked to `alias` argument, command substitution then replaces the command inside `$()` with the command's output so that `cd` can use it.
So, `cd $(jmp <alias>)` is translated to `cd (output of jump)`.

For more information about command substitution, check [this](https://www.linuxjournal.com/article/7385) article's "command substitution" entry.

#### Making it easier

You can create function in your `.zshrc/.bashrc` file to automate this. Function can be something like following:

```sh
function jumpTo() {
	cd $(jmp $1); # where $1 is the first argument after jumpto
}
```

After reinitializing shell you may use it with `jumpto <alias>`.

### Creating a new path
You can easily save a new path for later like following

```sh
# usage: jump [flags] <alias> [long path] 
$ jump website /var/www/html 
```

### Deleting a new path
To delete a path, just put `-d` flag

```sh
# usage: jump [flags] <alias> [long path] 
$ jmp -d website
```

### Updating existing path
To update an existing path, just reassign a value to it like following

```sh
$ jmp website /var/www/my_website
```

### Resetting database

Which may be done with `-r` flag. 

```sh
$ jmp -r
```

## File structure

What does any of these files even contain?

<<<<<<< dev
### main.c
`main.c` contains the *core* of this program. It determines how things go, in which order, and handles errors.

### tests.c
`tests.c` contains some tests for database and the base program
=======
```sh
$ jmp -l
```
>>>>>>> local

### database.c
`database.c` contains the database handler and other database related stuff.

### utils.c
`utils.c` contains some helping functions for other modules such as help menu, etc.
