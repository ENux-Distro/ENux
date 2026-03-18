# ENux 5.1.1

Welcome to **ENux 5.1.1**, a Debian-based Linux distribution with **Bedrock Linux integrated**.


## What is ENux?

ENux 5.1.1 is a Debian-based distribution with Bedrock Linux on top, and is the first ever Linux distribution that has Bedrock Linux pre-installed.

This configuration allows you to use multiple mainstream Linux package managers on a single system:

- dpkg / apt (Debian)
- apk (Alpine)
- xbps (Void)
- dnf / rpm (Fedora / Red Hat) **(New)**
- zypper (openSUSE)
- emerge / portrage (Gentoo)
- pacman (Arch)
- enux (Package Manager Manager)
- nix (NixOS)

**Conflict resolution:**  
Bedrock Linux handles most compatibility headaches. For beginners, **enux** simplifies package management into one easy-to-use tool.
Note: nix isn't a part of brl/pmm, it is independent
---

## Features

- Debian base for stability and reliability  
- Bedrock Linux pre-installed
- Access to multiple package managers on one system  
- Unified **enux** tool for simplified package management
- A nice ENux Welcome CLI program
- Lightweight **XFCE** desktop for performance  
- Clean, minimal, and beginner-friendly setup
- Our own Linux 7.0-rc2-enux-enux kernel, ensuring you got the latest bleeding edge hardware and firmware support
- Created with our own ISO Creator tool

---

## Difference Between ENux 1.0, 2.0, 2.1, 3.0 and 4.0, 4.5, 5.0 and 5.1.1

### ENux 1.0
- Multi-step installation process  
- Required running Calamares, then **ENux Finaliser Phase 1**, then **Finaliser Phase 2**  
- Bedrock Linux installed after the base system  

### ENux 2.0
- Bedrock Linux is downloaded and installed directly during Calamares using `wget` and `--hijack`  
- Installation reduced to a single main step  
- Only one Finaliser is required to **brl fetch** the stratas  

### ENux 2.1
- ENux Finaliser now runs automatically as a **first-boot script**  
- Keeping the **.desktop shortcut** to manually access the Finaliser  
- Bug fixes and improvements to the Finaliser  
- Improved system branding and metadata  
- Overall installation flow is smoother and more polished

### ENux 3.0
 - Added support for Centos 10-stream, giving you a more stable and long term dnf support
 - Added openSuse Tumbleweed support, now you can use zypper too.
 - Removed Fedora, because ENux used to use Fedora 41 and it is almost EOL
 - The Finalizer isn't a hybrid first-boot script with a .desktop shortcut. It is returned to old fashioned .desktop shortcut installation.

### ENux 4.0
 - Added support for NixOS's nix package manager
 - Changed the live username from "enux" to "ENux", making the username look more polished
 - Changed the live hostname from "enux" to "ENux-Live-System", making the user feel more like in a live system
 - Now using Cinnamon's Lightdm, giving you a simpler, less flashy experience

### ENux 4.5
 - Switched from Debian's 6.12.57-deb13+amd64 kernel, to 6.18.5-enux kernel
 - The kernel experience will stay the same
 - The overall user experience got more polished

### ENux 5.0
 - Created an **ENux Welcomer** first boot CLI script, that installs recommended packages, tells you what ENux is, brl fetches + adds nix and let's you test a package manager (xbps)
 - Replaced **ENux Finalizer** with **ENux Welcomer**
 - Fixed some minor bugs from **Calamares**, **enuxfetch** and **config.jsonc** for **enuxfetch**
 - Using zstd compression instead of xz for faster boot speeds

### ENux 5.0.1
  - Introducing the ENux Package Manager, a nice tool that uses "pmm" as for the backend
  - Improved the visuals on the terminal 

### ENux 5.1.1
  - ENux 5.1.1 is the first ever Linux distribution that has Bedrock Linux pre-installed
  - This version was created with our own tools such as the ENux ISO Creator, that uses ENuxbootstrap, and other tools on the backend
  - ENux 5.1.1 also replaced Calamares, with our own installer, which is both avaible on CLI and TUI
  - The ISO size is only 1.04 GB, compared to 5 GB on ENux 5.0.1
  - ENux 5.1.1 now uses Linux kernel 7.0-rc2-enux-enux, ensuring you have the latest hardware and firmware support
  - We've switched from CentOS to Fedora, ensuring you have the latest and cutting-edge software support
  - Fixed minor bugs on the ENux Package Manager
  - Fixed minor bugs on branding 

---

## Hardware Requirements

**Minimum:**
- CPU: x86_64-v2 (Required for CentOS fetch) 
- RAM: 550 MB  
- Storage: 25 GB  

**Recommended:**
- CPU: Dual-core  
- RAM: 800 MB  
- Storage: 35 GB  

**High-end:**
- CPU: Quad-core  
- RAM: 1 GB  
- Storage: 45+ GB  

---
## Installation Guide

1. Download the ENux 5.1.1 ISO:
   - [ENux-5.1.1.iso](http://www.emirpasha.com/ENux-5.1.1.iso)
   - [ENux-5.1.1.iso (Sourceforge)](https://sourceforge.net/projects/enux/files/latest/download)
   - [ENux-5.1.1.iso (GitHub)]([https://sourceforge.net/projects/enux/files/latest/download](https://github.com/ENux-Distro/ENux/releases/download/ENux-5.1.1/ENux-5.1.1.iso))

2. Flash the ISO to a USB drive using tools such as **Rufus** or **Balena Etcher**.

3. Boot from the USB drive and run the **ENux Installer (Calamares)**.

4. After installation and reboot, and the **ENux Welcomer** will welcome you


## Known Behaviors

### ENux Welcomer Mirroring

During the ENux Welcomer, Bedrock Linux strata(s) are fetched from external mirrors.

In rare cases, strata fetching may fail due to:
- Temporary mirror outages
- Slow or unstable internet connections
- Regional mirror availability issues

If this happens, you can try **brl fetch**ing the **strata** again with different mirrors
No system reinstallation is required.

Once the strata are fetched successfully, ENux is fully ready to use.

### LightDM Not Appearing After Installation

In rare cases of ENux Installation, LightDM couldn't be fully setup.
Therefore after installation, you may be on the TTY terminal.

In order to get to **XFCE**, you must run
 - **startx**

after you log in

## Dev Notes

- The username on the live system is `ENux`
- The password on the live system is `enux`

## Troubleshooting

If you encounter issues:
- Visit [r/ENux on Reddit](https://www.reddit.com/r/ENux/)  
- Ask questions, report bugs, or share feedback with the community

