## NOTE: ENux 5.0 is releasing today, here are the patch notes: 
 - Created a first boot CLI welcomer called **ENux Welcomer**, that installs some packages, gives information about ENux, has the ENux Finalizer built into it, and it let's you test a package manager (Void Linux's xbps to be more clear)
 - Removed **ENux Finalizer**, since it is injected on the first boot welcomer
 - Fixed minor bugs on **Calamares**, **enuxfetch**, and **config.jsonc** for **enuxfetch**


# ENux 4.5

Welcome to **ENux 4.5**, a Debian-based Linux distribution with **Bedrock Linux integrated**.



## What is ENux?

ENux 4.5 is a Debian-based distribution with Bedrock Linux on top, allowing you to use multiple mainstream Linux package managers on a single system:

- dpkg / apt (Debian)
- apk (Alpine)
- xbps (Void)
- dnf / rpm (CentOS / Red Hat)
- zypper (openSUSE)
- emerge / portrage (Gentoo)
- pacman (Arch)
- pmm (Package Manager Manager)
- nix (NixOS) **(New)**

**Conflict resolution:**  
Bedrock Linux handles most compatibility headaches. For beginners, **pmm** simplifies package management into one easy-to-use tool.
Note: nix isn't a part of brl/pmm, it is independent
---

## Features

- Debian base for stability and reliability  
- Bedrock Linux integration during installation  
- Access to multiple package managers on one system  
- Unified **pmm** tool for simplified package management  
- Lightweight **XFCE** desktop for performance  
- Clean, minimal, and beginner-friendly setup
- Our own Linux 6.18.5-enux kernel, ensuring you got the latest stable kernel. **(New)**

---

## Difference Between ENux 1.0, 2.0, 2.1, 3.0 and 4.0 and 4.5

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
---

## Hardware Requirements

**Minimum:**
- CPU: x86_64-v2 (Required for CentOS fetch) 
- RAM: 3 GB  
- Storage: 30 GB  

**Recommended:**
- CPU: Dual-core  
- RAM: 5 GB  
- Storage: 45 GB  

**High-end:**
- CPU: Quad-core  
- RAM: 8+ GB  
- Storage: 55+ GB  

---
## Installation Guide

1. Download the ENux 4.5 ISO:
   - [ENux-4.5.iso](http://www.emirpasha.com/ENux-4.5.iso)
   - [ENux-4.5.iso (Alternative)](https://sourceforge.net/projects/enux/files/ENux-4.5.iso/download)

2. Flash the ISO to a USB drive using tools such as **Rufus** or **Balena Etcher**.

3. Boot from the USB drive and run the **ENux Installer (Calamares)**.

4. After installation and reboot, run **ENux Finaliser** from the .desktop shortcut.


## Known Behavior (Finalizer Mirroring)

During the ENux Finaliser, Bedrock Linux strata are fetched from external mirrors.

In rare cases, strata fetching may fail due to:
- Temporary mirror outages
- Slow or unstable internet connections
- Regional mirror availability issues

If this happens, you can try **brl fetch**ing the **strata** again with different mirrors
No system reinstallation is required.

Once the strata are fetched successfully, ENux is fully ready to use.

## Dev Notes

- The username on the live system is `ENux`
- The password on the live system is `enux`

## Troubleshooting

If you encounter issues:
- Visit [r/ENux on Reddit](https://www.reddit.com/r/ENux/)  
- Ask questions, report bugs, or share feedback with the community

