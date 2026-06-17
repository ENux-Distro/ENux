# ENux Roadmap

This document outlines current goals, experimental features, and future plans for ENux.

ENux is a Debian-based experimental Linux distribution with Bedrock Linux pre-installed and has 13 package managers


## Current Version (ENux 5.x Series)

### Core Features
- Debian-based stable base system
- Bedrock Linux pre-installed
- 13 package managers
- Custom kernels
- ENux Welcomer
- CLI/TUI installer
- XFCE desktop environment


## Short-Term Goals

### Installer Improvements
- Improve CLI/TUI installer usability
- Add GUI installer

### Kernels
- Create more kernel packages
- Create a repo that hosts ENux's kernels which can be download via apt 


## Experimental Features (Work in Progress)

### Custom Init System (init.c)
- Optional replacement init system
- Focuses on speed
- Currently in “works on my machine” stage

### Bedrock Live Integration
- Pre-configured strata in live ISO
- Testing `brl fetch` reliability in live/chroot environments


## About Next Year

[About Next Year](https://github.com/ENux-Distro/ENux/ABOUT-NEXTYEAR.md)


## Long-Term Ideas (Research Phase)

- Switch bases from Debian to LFS
- A custom package manager with its own package format

## ENux 6.0

ENux 6.0 is planning to move from Debian + Bedrock to LFS + The ENux Layer, which'll be a custom Bedrock-style layer. This will take a long time to build you folks :D
## Philosophy

ENux is not a traditional Linux distribution.

It:
- Has Bedrock Linux pre-installed
- Has 15 package managers
- Uses custom kernels
