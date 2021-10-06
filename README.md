# MCD - Modern CD
Easy alternative for long path aliases in shell configuration files (boring).

## Motivation
You may just want faster way to navigate in your linux filesystem (like I did), or just get rid of always making alias for a path in your shell configuration file. This software is exactly for that.

I've noticed [`weezel`](https://github.com/weezel) has made same kind of project, and thought it would be fun to create it myself.
Credits to him. 
I don't exactly wan't to *copy* him, but to practice my C skills, to be clear.

Myself, i don't really _love_ moving along the linux filesystem when it comes to larger paths.
To prevent this, this app can store your most used paths and you can access them later easily.

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

### 
