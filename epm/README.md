# epm
epm is a minimal package manager written in C that uses its own minimal .epm package format.

## What is .epm package format

.epm package format is technically a tar archive that the .epm package manager extracts. The .epm package needs to have 2 important things:

### Control

Control, which needs to be executable, is the heart of the package manager. After extracting the .epm file, the package manager runs the Control file.

The control file must consits of where the package is going to be copied to, what happens after both install, warnings and etc.

### The program itself

The program itself, is going to be inside of a directory like /usr/bin, which the epm package manager will log the program's place onto /var/epm/installed/[package]

## epm options

| option  | usage           |
|-------|-------------------|
| install   | Downloads & installs the package   |
| purge  | Removes the package          |
| sync    | Pings the avaible mirros at /etc/epm/mirror.list|
| clean  | Removes logs and cache |


## How to install epm

- **Step 1**: git clone the repository via ```git clone --depth 1 https://github.com/ENux-Distro/epm``` Note: You can also download the raw files from the ENux repository, located at ```https://github.com/ENux-Distro/ENux/epm/```
- **Step 2** Change your directory to the cloned repo, and then run ```sh ./install```. This compiles epm.c as a dynamic executable
- **Step 3** Install epm to your system via ```sh ./install install```
- **Step 4** Test epm. We recommend you download enux-utils or enux-dotfiles for testing purposes.
- **Step 5** ***Enjoy***
