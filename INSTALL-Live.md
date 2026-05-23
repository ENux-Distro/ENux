# ENux Installation Guide

This document explains how to install ENux Live XFCE step-by-step.

ENux is a Debian-based Linux distribution with Bedrock Linux pre-installed and 13 package managers

---

## 1. Download ENux

Download the latest ENux Live ISO:

- [From emirpasha.com](http://www.emirpasha.com)
- [From Sourceforge](https://sourceforge.net/projects/enux/files/)
- [From GitHub](https://github.com/ENux-Distro/ENux/releases/)

---

## 2. Create a Bootable USB

You will need a USB drive [minimum: 2GB] [recommended 4GB+].

Use one of the following tools:

- Rufus (Windows **Recommended**)
- Ventoy (Linux / Windows **Recommended**)
- Balena Etcher (Linux / Windows / macOS)
- dd (Linux advanced users)

Example (Linux dd method):

```sudo dd if=ENux-5.3.3.iso of=/dev/sdX bs=4M status=progress oflag=sync```


Replace /dev/sdX with your corresponding drive, like /dev/sdb

## 3 Boot ENux Live

- Ensure the ISO file is flashed to the USB
- Enter BIOS/UEFI boot menu
- Select the USB device
- Boot into the ENux Live XFCE environment

## 3.5 Chose the Installer

Inside the live environment, you can choose either the CLI or the TUI installer, here's the rundown of both:

### **CLI**: 
 
  **Pros**:
  - Extremely minimal
  - Pretty straight forward

  **Cons**:
  - Less beginner-friendly
  - Easier to make mistakes

### **TUI**:
 
  **Pros**:
  - More beginner friendly
  - More interactive
  
  **Cons**:
  - Less straight-forward compared to the CLI version
  - Requires more interaction compared to the CLI version

### **My recommendation**:

**CLI** If advanced/power-user

**TUI** If intermediate/first time ENux'er

## 4 Start the Installer

When you first spin up the ENux Installers, they'll do a quick internet check to see if you have proper internet connection.

If you don't have proper internet connection, check https://github.com/ENux-Distro/ENux/blob/main/README.md#wifi-not-working-out-of-the-box for on how to setup your internet

Once the tests are completed succesfully, the installer will ask you for your partition. Let's say we want to install ENux to my 30G sda drive. 
We'll type in **/dev/sda**

Nice, it opened up the interactive partition editor **cfdisk**. I recommend you partition your drive like this:

- **90% of it for /**
- **9% of it for swap**
- **1% of it for EFI (if using a UEFI PC)**

After we partition our disks, the installer will ask us if we want to return to cfdisk again. Return to cfdisk if you are unsure about your partitionings, or
want to sanity check; continue if you are sure of everything.

Now, after partitioning the installer will ask for our **root (/)** partition. Let's assume my partitions on /dev/sda looks like this:

- **/dev/sda1: 25GB - Type: Linux filesystem - Will be our /**
- **/dev/sda2: 4GB - Type: Linux swap - Will be our [SWAP]**
- **/dev/sda3: 1GB - Type: EFI System - Will be our EFI partition**

**NOTE: If you are not on an EFI system, DON'T create an EFI partition. Instead take that 1GB you created earlier and put it towards swap or root**

Let's enter my root partition **/dev/sda1**. It asks if I created swap, which we'll answer as yes. My swap partition will be **/dev/sda2**. 
The installer asks our root partition will be erased, and if we want to continue or not. We want to continue. 

**NOTE: As you saw right there, it didn't ask for our EFI partition. It'll ask us shortly on the firmware detection part.**

The installer will now bootstrap ENux, and then installer the base packages. This might take a while so be patient. I recommend you grab some coffee, or play 
your instrument in the meantime.

Once the installer asks for our hostname, type in something creative, like **enux-btw** because we're using ENux, by the way. 
For the username, I'll enter my real name **emir**
Once it creates the user, and handles the hostname creation, it'll ask us for the user and root password. Make sure its something secure like **1234** (:D)

It's time for the firmware detection and bootloader setup. As I told you earlier, the installer asks for our EFI partition, which is **/dev/sda3** in our case
so we'll enter that. Now the installer will install **GRUB** (the bootloader), generate the initramfs, generate the fstab, create the grub.cfg file.

Once that's done, the installer will run the **ENux Bedrock Setup** which auto hijacks Bedrock Linux on our target root. After that, it'll 
enable the services, unmount safely, sync the drives and ask for us to reboot, which we'll say yes to.

## 5 Post Install

While rebooting, remove the USB and optionally go into your BIOS/UEFI settings for extra sanity checks on the drive which'll be booted.
**NOTE: You might see a **UEFI OS** bootloader entry on your BIOS. That's most likely ENux, but double check that**

When you reboot to your target drive, you should see the **GRUB** bootloader menu. We'll press enter on **ENux GNU/Linux** and then encounter Bedrock Linux's init.
After we wait or select the single **enux** stratum, we'll see the **LightDM** display manager. We login as **emir**, since that's our non-root user and look,
we get welcomed with the **ENux Welcomer**. It asks which packages it should install. I usually install these packages any why:

- **git** for cloning repositories like **linux**
- **gcc** for compiling C code
- **binutils** for important stuff like **as** and **ld**
- **alsa-utils, alsa-tools, alsa-tools-gui** for getting sound on **ENux**

After we type these packages, we can press enter, enter our password and install the packages. Once the packages are installed, the **ENux Welcomer** explains
what ENux is, and then it installs the package managers. This will take **really** long, so I recommend you grab another coffee, do your homework, play
your instruments.

Alright so the package managers are installed, now it asks us if we want to test a package manager. I press yes and I interactively install **htop** via **xbps**.
Then, it asks us if we want to install **WingAI** or not.
**NOTE: The WingAI that is in the ENux Welcomer isn't for dating, its used for programming**

**Pros of WingAI**:
- It's a Meta AI, which uses 3 models, packaged with our Response Packager
- Perfect for people who like to tinker with LLMs, and like to code

**Cons of WingAI**:
- Some people may consider this as bloat
- Can get heavy

After **WingAI** it now asks if we want to install ENux-AI.

**Pros of ENux-AI**:
- It uses a powerful and minimal Qwen 2.5 7B model running locally
- Built generally for ENux
- Perfect for people who have capable hardware, and want a buddy that they can get help from

**Cons of ENux-AI**:
- Can get **extremely** heavy on lower-end setups
- Some people may consider this as bloat

After those, the ENux Welcomer *bye bye's* you can:

- Test out the package managers
- Use the ENux tools to understand how ENux is built and installed
- Download our [dotfiles](https://github.com/ENux-Distro/dotfiles) and make ENux look way more asthetic
- **AND MORE**

