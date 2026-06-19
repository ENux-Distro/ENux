# ENux 6.x

This README file contains the Prototype architecture of ENux 6.x

## The ENux Layer

The ENux Layer is going to be the core of ENux 6.x lineup. It's going to be heavily inspired by Bedrock with its ability on making multi Linux co-exists and its tooling.

## How It'll Work

The ENux Layer will create a /enux directory, just like /bedrock. That directory is going to contain these:

- /enux/bin/: This'll contain The ENux Layer tooling, such as pmm, layer (strat clone) and enux (brl clone)
- /enux/layer/: This'll contain the layers. This'll be a clone of Bedrock Linux's stratas. You can add new layers by running ``enux install arch`` or ``enux install layer:arch`` for example
- /enux/layerdata.enuxconfig: This .enuxconfig is going to contain what is the current, used layer. This'll also include the order of executing a file, and top priorities. Here's how layerdata.enuxconfig will look like:
```bash
active_layer="enux"
exec_order=["enux"]

# WARNING: The top priority and defining which executable will run on which layer IS INCREDIBLY DANGEROUS. THESE ARE COMMENTED ON PURPOSE
# top_priority="arch"
# gcc="fedora"
```
When a new layer gets installed, the exec_order will be updated. The first layer will always be the first on the exec_order list.
- /enux/sbin/: Will contain the init. Thinking of re-writing init.c for this.
- /enux/libexec/: Will contain some of the program's core. For example there's pmm in /bedrock/bin/, which calls /bedrock/libexec/pmm with the apt interface. The pmm in /bedrock/libexec contains the core of everything, while /bedrock/bin/ are usually wrappers calling the programs, same with The ENux Layer.
- /enux/etc/: Will contain stuff like os-release and enux.conf.

## What About Programs

The ENux Layer will integrate these programs:

- pmm: The unified package manager, just like Bedrock Linux's pmm. The idea is going to be same as Bedrock's, just changed and optimized for The ENux Layer. You can still install a package from a selected layer, with ``pmm install layer:arch neofetch``
- layer: The strat clone. This'll allow you to run programs for your chosen layer. Thinking of something like "chroot $layer <command>", but probably will be better than that in the future.
- enux: The brl clone. It let's you install layers (with ``enux install arch`` or ``enux install layer:arch`` to be more specific), remove a layer, enable a layer, show a layer, create an alias layer and stuff like that.

## How to Install and Use (Detailed Edition)

During early alpha builds, I'm planning of releasing The ENux Layer to the public. After everything is finalized, I'll integrate that to ENux 6.x Alpha Builds.

During early alpha builds of The ENux Layer, you'll do these:

- Download the script
- Make it executable and run ./the-enux-layer --install and then type Not revertable since its in early alpha builds and has the risk of damaging your system.
- Reboot to finalize everything

When The ENux Layer reaches to ENux 6.x Alpha builds, you'll do these:

- Download the ENux ISO Image and flash it onto a USB
- Boot that USB and start using The ENux Layer without any manual labor.

## Daily Driving Senario (Detailed Edition)

You open up a terminal and then type ``enux list layer`` for it to list the available layers.
Then you type in ``enux install arch`` with installs the arch layer to your system. I'm thinking of grabbing it's rootfs, extracting it to /enux/layer/arch and then do some wizardy to get it working (haven't planned the wizardy). 

After the arch layer is installed, you run ``enux install layer:fedora``. After that is done you run ``enux remove --disable arch`` or ``enux remove --disable layer:arch``. Then, you reboot and then get greeted with init.c and you chose the fedora layer. Since it doesn't have any X or Wayland, you get dropped into an TTY. Oh no, didn't meant to do that, reboot to enter the layer with your X/W stuff.

## Extras

The ENux Layer will be friendly towards container, chroot, non-PID 1 environments. Unlike Bedrock, which to get stuff working you need your own wizardy, you just run ``enux install arch`` and you can use it in the container environment, or on your ISO.

## What About ENux 5.x?

ENux 6.x will be my top pritority alongside The ENux Layer. ENux 5.x won't be EOL, it'll still recieve minor updates. But when 6.x stabilizes to a point where its kinda usable, ENux 5.x support will be dropped.
