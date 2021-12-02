# Jump
Alternative for long path aliases in shell configuration files.

## Motivation
You may just want a faster way to navigate in your filesystem chaos (like I did), or just get rid of always making alias for a path in your shell configuration file. This software is exactly for that.

I've noticed [weezel](https://github.com/weezel) has made same kind of project, and thought it would be fun to create it myself.
Credits to him. 
I don't exactly want to *copy* him in this project, but to practice my skills as I haven't done much with C.

Myself, I don't really _love_ moving along the linux filesystem when it comes to longer paths.
To prevent this, this app can store paths of your choice, so you can access them easier later on with custom alias.

## Installation 

The easiest way to install is __GitHub's releases__ feature. 
That way you can download already compiled binary and run it, with no setup required. 
More information about below at `Downloading from releases`.

Another possibility is to download and compile manually from source, what you can do by `git `and `make`.
You can do that if you want to contribute, or just want to verify the source code yourself before running.  

### Requirements

There is couple of required libraries what you need to install before compiling. They are listed down below.

- libbsd-dev
- libsqlite3-dev

#### libbsd-dev

Gives us `strlcpy(), strlcat()` functions to prevent buffer overflow when managing memory

#### libsqlite3-dev

`libsqlite3-dev` gives us library which we can treat data and manage connections in database with

### Downloading from releases

You can download the latest version from GitHub's [`releases`](https://github.com/toke8677/jump/releases) tab.

**Optional**: After that, you may want to move jump binary to `/usr/local/bin/`, so you can access it later just by running `jmp` and not `/path/to/jmp`.

This can be done by `mv` (move the binary) or `cp` (copy the binary). The following example shows how it's done with `mv`.

`mv jmp /usr/local/bin/jmp`

### Installing from source
This option can be better for development, contributing, or you may just want to be sure you downloaded the correct binary. 

#### Download
You can download the source code and build it locally:

```sh
# HTTP - use if you haven't got ssh-keys set up in Github
$ git clone https://github.com/toke8677/jump.git

# ssh - use if you've got ssh-keys set up in Github
$ git clone git@github.com:toke8677/jump.git
```

#### Compiling
After download you should be able to compile the project with `make`:

```sh
# clean object files and compile with optimization flags for faster experience
$ make clean release

# Optional: make available in path
$ cp jump /usr/local/bin/jump
```

## Documentation
Everyone loves documentations. Especially when they are well written.

### Using an alias
To use a path, you can use a simple shell trick called command substitution like following:

```shell
cd $(jmp <alias>)
```

#### How command substitution works?

1. `jmp <alias>` gives us the correct path
2. Command substitution then replaces the command inside `$()` with the command's output so that `cd` can use it. In this case it will be the path we got from `jmp`.
3. `cd $(jmp <alias>)` is translated to `cd /path/`.

For more information about command substitution, check [this](https://www.linuxjournal.com/article/7385) article's "command substitution" entry.

#### Making it easier

You can create function in your shell configuration file to automate this. 
The function can be something like following:

```sh
function jumpTo() {
	cd $(jmp $1); # where $1 is the first argument after jumpto
}
```

After reinitializing shell you may use it with `jumpto <alias>`.

### Creating a new path

You can easily save a new path for later like following

```sh
# usage: jmp <alias> <long path> 
$ jmp website /var/www/html 
```

### Deleting a new path
To delete a path, just put `-d` flag

```sh
# usage: jmp -d <alias>
$ jmp -d website
```

### Updating existing path
To update an existing path, just reassign a value to it like following

```sh
$ jmp website /newpath
Update information: [ /oldpath -> /newpath ]
Want to proceed? (y/N):
```

### Resetting database
Removing all aliases may be done with `-r` flag. 
