## NOTE: ENux 3.0 Will Release Today. The Patch Notes Are:
# ENux has added support for Opensuse, and Centos. But we have removed Fedora, because ENux uses Fedora 41, and it is almost EOL


# ENux 2.1

Welcome to **ENux 2.1**, a Debian-based Linux distribution with **Bedrock Linux integrated**.



## What is ENux?

ENux 2.1 is a Debian-based distribution with Bedrock Linux on top, allowing you to use multiple mainstream Linux package managers on a single system:

- apk (Alpine)
- xbps-install (Void)
- dnf / rpm (Fedora / Red Hat)
- emerge (Gentoo)
- pacman (Arch)
- dpkg / apt (Debian)
- pmm (Package Manager Manager)

**Conflict resolution:**  
Bedrock Linux handles most compatibility headaches. For beginners, **pmm** simplifies package management into one easy-to-use tool.

---

## Features

- Debian base for stability and reliability  
- Bedrock Linux integration during installation  
- Access to multiple package managers on one system  
- Unified **pmm** tool for simplified package management  
- Lightweight **XFCE** desktop for performance  
- Clean, minimal, and beginner-friendly setup  

---

## Difference Between ENux 1.0, 2.0, and 2.1

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

---

## Hardware Requirements

**Minimum:**
- CPU: 64-bit processor  
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

1. Download the ENux 2.1 ISO:
   - [ENux-2.1.iso](http://www.emirpasha.com/ENux-2.1.iso)
   - [ENux-2.1.iso (Alternative)](https://sourceforge.net/projects/enux/files/ENux-2.1.iso/download)

2. Flash the ISO to a USB drive using tools such as **Rufus** or **Balena Etcher**.

3. Boot from the USB drive and run the **ENux Installer (Calamares)**.

4. After installation and reboot:
   - The **ENux Finaliser** will run automatically on first boot  
   - Alternatively, it can be launched manually using the desktop shortcut  


## Known Behavior (Finaliser Mirroring)

During the ENux Finaliser, Bedrock Linux strata are fetched from external mirrors.

In rare cases, strata fetching may fail due to:
- Temporary mirror outages
- Slow or unstable internet connections
- Regional mirror availability issues

If this happens, you can try **brl fetch**ing the **strata** again with different mirrors
No system reinstallation is required.

Once the strata are fetched successfully, ENux is fully ready to use.

## Troubleshooting

If you encounter issues:
- Visit [r/ENux on Reddit](https://www.reddit.com/r/ENux/)  
- Ask questions, report bugs, or share feedback with the community

