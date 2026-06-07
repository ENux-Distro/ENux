# ENux Issues 

## typo ? Distrobution by disgorge69 from GitHub issues (closed)

https://www.emirpasha.com/

The minimal, Linux distrobution with 9 package managers. Distribution ? (or maybe I'm wrong)

On landing page of website.

### Fix

**Fixed by replacing distrobution with distribution in on the main website**

---

## Calamares Creating an Empty Filesystem Skeleton by NoReplacement5627 from Reddit (closed)

(...) Conclusion: Calamares is unpacking a base tarball but never executing the package installation stage. The ISO appears to be missing its package manifest or the post-install hooks are not firing.

### Fix

**ENux doesn't use Calamares after 5.1.1, this issue was created after ENux 5.1.1, which doesn't use Calamares**

---

## Distrowatch Weekly Issue 1170, ENux not installing properly by Jesse Smith from Distrowatch (fixed)

(...) 
At this point in my experiment the system installer window closed with no warning or error message; I was simply returned to the desktop. I confirmed nothing had been written to my fresh root partition and then ran the installer again. Again, the installer asked me to divide the disk, select partitions for root and swap, and then closed the window. I tried both the TUI and CLI launchers for the installer and both acted in the same manner.

Next, I tried opening a terminal window and running the install script manually. A little experimenting with this determined the installer was failing when trying to run the mkfs.ext4 command, which is used to format a partition. This command was failing because no valid partition name was being passed to the program and, upon this error happening, the install script would immediately terminate.

Eventually, I figured out the problem. When the installer asks us which disk we want to use, it selects a default for us and displays the name of the disk. Pressing Enter to accept the suggestion then launches the cfdisk partition manager to work on the selected disk, as expected. Later, when we are asked which partition should be used for the root filesystem the installer picks a default choice and displays it. However, pressing Enter to accept this default offering does not work and mkfs.ext4 gets run with no partition set as the target. To fix this we need to manually type the full name of the root partition, such as /dev/sda2, or the script will fail without any error or warning. 

Once I had worked out this solution the installer was able to format my partitions and then install its packages to my disk. This took several minutes and then the installer fetched some packages from the Internet. It looks as though the kernel package is fetched from a remote software repository and then installed, though the need for a network connection isn't mentioned anywhere on the website or during the install process. Luckily, I had connected to the Internet earlier to test Firefox, and the installer successfully fetched the kernel package.

The install script then did two (new) weird things. First, the installer reported it had successfully finished. Then it reported it was unable to install GNOME's network manager, the firmware-linux package, the realtek firmware package, and the iwlwifi package, due to missing packages or dependencies. Then installer then stopped, returning me once more to a shell prompt.

I checked the root partition I had used for ENux and discovered that it was not only missing the firmware bundles, the installer also had not got around to installing a boot loader. In short, the operating system could not be booted. 

### Fix

**ENux 5.3.1 fixed this issue, with proper disk handling and fixed the installer on which packages would be installed**

---

## No LICENSE.md? by LambBread from GitHub issues (closed)

I have noticed this repository does not have a LICENSE.md or similar; without one, it is assumed by default that all rights are reserved. This makes it unclear how others are allowed to use, modify, or distribute the code.

Do you have a license in mind for this project? If you are open to adding a license, adding one such as MIT, GPL, Apache-2.0, BSD-3 Clause, etc. would make things clearer. I am also happy to open a pull request with a license if necessary.

### Fix

**Fixed by adding GNU GPL-v3 License**

---

## Reduce ENux Installation Step from 2 to 1 by ENux-Distro (me) from GitHub (open)

This issue has been open for 6 months. The reason why this is so difficult is because of Bedrock Linux's brl fetch commands not working nicely on chroot non-PID 1 environments.

With ENux 5.5.3 having the stratas pre-fetched in the live environment, which required me to do a lot of tricks and workarounds in chroot non-PID 1 extracted squashfs environment to get it working, the possibility of reducing the installation steps is getting higher.

This issue will stay open until I figure out how can I make the install brl fetch the stratas.

How did ENux 5.5.3 get brl fetch commands to work on chroot non-PID 1 environments: https://www.reddit.com/r/bedrocklinux/comments/1tynwwy/enux_553_stratas_prefetched/

---
