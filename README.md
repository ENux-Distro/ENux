# ENux 2.0

Welcome to ENux 2.0 a Debian-based Linux distribution with Bedrock Linux on top


## What is ENux?

ENux 2.0 is a Debian based distribution with Bedrock Linux on top, to be able to use:

- apk (Alpine)
- xbps-install (Void)
- dnf / rpm (Fedora/Red Hat)
- emerge (Gentoo)
- pacman (Arch)
- dpkg / apt (Debian)
- pmm (Package Manager Manager)

**Conflict resolution:** Bedrock Linux handles most compatibility headaches. For beginners, pmm simplifies package management into one easy tool.

## Features

- Debian base for stability  
- Bedrock Linux integration for cross-distro package support  
- Unified enux package manager for beginners  
- Lightweight XFCE desktop for performance

## Difference between ENux 1.0 and ENux 2.0

ENux 1.0 used to be a 3 step installer, you run Calamares, then ENux Finaliser Phase 1, then ENux Finaliser Phase 2.
But on ENux 2.0, Calamares wget's Bedrock Linux and installs (*--hijacks* it), and there is only a single Finaliser which *brl fetch*es the stratas

## Hardware Requirements

**Minimum:**
- CPU: 64-bit processor
- RAM: 3 GB
- Storage: 20 GB

**Recommended:**
- CPU: Dual-core
- RAM: 5 GB
- Storage: 33 GB

**High-end:**
- CPU: Quad-core
- RAM: 8+ GB
- Storage: 50+ GB

---

## Installation Guide

1. Download the ENux 1.0 ISO from:  
   - [ENux-2.0.iso](http://www.emirpasha.com/ENux-2.0.iso)

2. Flash the ISO to a USB drive using tools like **Rufus** or **Balena Etcher**.

3. Boot from the USB and run the **ENux Installer**.

4. After reboot, run **ENux Finaliser** to **brl fetch** the stratas

---

## Troubleshooting

If you encounter issues:
- Visit [r/ENux on Reddit](https://www.reddit.com/r/ENux/)  
- Ask questions, share feedback, or discuss ENux with the community.

