# Evolution3DS
*Nintendo 3DS "Custom Firmware"*

## Description
**Evolution3DS** is an unofficial fork of [Luma3DS](https://github.com/LumaTeam/Luma3DS). This project combines features of several Luma3DS forks that I like.

## Added/Changed Features
- Changed the configuration file name to `evoconfig.ini`
- Changed the colors of the Rosalina and Configuration menu
- Tools menu
- Option to crash the system for fun
- Set the number of Play Coins  
  ↳ [Original: Gruetzig](https://github.com/Gruetzig/Luma3DS/commit/1e329b55dade61ba74a0bb1cc6e59d2504d0bde1)
- Plugin selector  
  ↳ [Original: Tekito-256](https://github.com/Tekito-256/Luma3DS)  
  ↳ *Note: After setting a default plugin, hold Select while launching a game to open the plugin selector*
- Dangerous plugin detection  
  ↳ [Original: Tekito-256](https://github.com/Tekito-256/Luma3DS)
- Support for v0.5.x 3GX plugin loading  
  ↳ [Original: Tekito-256](https://github.com/Tekito-256/Luma3DS)
- Time display in Rosalina menu  
  ↳ [Original: JBMagination2/hide0123](https://github.com/JBMagination2/Luma3DS/commit/766173d65eeb52baac90e9e68b456ad93ebaab7b)
- Logo.bin loading feature  
  ↳ [Original: Pixel-Pop](https://github.com/Pixel-Pop/Luma3DS/commit/d225d9fa507dcccce3a6c86d0a38f7998f39b7a2)
- N3DS clock + L2 status display in Rosalina menu  
  ↳ [Original: DullPointer](https://github.com/DullPointer/Luma3DS/commit/2dbfa8b5c9b719b7f3056691f54332f42da6de8d)

For detailed technical information, please refer to the [official Luma3DS documentation](https://github.com/LumaTeam/Luma3DS/wiki) and [README](https://github.com/LumaTeam/Luma3DS/blob/master/README.md).

## Installation and upgrade

**Evolution3DS** requires [boot9strap](https://github.com/SciresM/boot9strap) to run.

Once boot9strap has been installed, simply download the [latest release archive](https://github.com/Cavvoh/Evolution3DS/releases/latest) and extract the archive onto the root of your SD card to "install" or to upgrade Evolution3DS alongside the [homebrew menu and certs bundle](https://github.com/devkitPro/3ds-hbmenu) shipped with it. Replace existing files and merge existing folders if necessary.

## Basic usage
**The Configuration menu** can be accessed by pressing <kbd>Select</kbd> at boot. The configuration file is stored in `/luma/evoconfig.ini` on the SD card (or `/rw/luma/evoconfig.ini` on the CTRNAND partition if Evolution3DS has been launched from the CTRNAND partition, which happens when SD card is missing).

**The chainloader menu** is accessed by pressing <kbd>Start</kbd> at boot, or from the configuration menu. Payloads are expected to be located in `/luma/payloads` with the `.firm` extension; if there is only one such payload, the aforementioned selection menu will be skipped. Hotkeys can be assigned to payload, for example `x_test.firm` will be chainloaded when <kbd>X</kbd> is pressed at boot.

**The overlay menu, Rosalina**, has a default button combination: <kbd>L+Down+Select</kbd>. For greater flexibility, most Rosalina menu settings aren't saved automatically, hence the "Save settings" option.

## Compiling

To build Evolution3DS:
1. Get the prerequites:
* git
* up-to-date devkitARM and libctru:
  * Install `dkp-pacman`: https://devkitpro.org/wiki/devkitPro_pacman
  * Install 3DS dev tools: `sudo dkp-pacman -S 3ds-dev --needed`
* [makerom](https://github.com/jakcron/Project_CTR) in PATH
* [firmtool](https://github.com/TuxSH/firmtool) installed
2. Clone the repository with `git clone https://github.com/Cavvoh/Evolution3DS.git`
3. Run `make`.

The produced `boot.firm` is meant to be copied to the root of your SD card for usage with Boot9Strap.

**Note:** For easy SD card file transfer, consider using [ftpd](https://github.com/mtheall/ftpd).

## Setup / Usage / Features
See https://github.com/LumaTeam/Luma3DS/wiki (needs rework)

## Credits
See https://github.com/LumaTeam/Luma3DS/wiki/Credits for original Luma3DS credits

People who made this fork possible:
- Cavvoh
- 2b-zipper ([Nexus3DS](https://github.com/2b-zipper/Nexus3DS))
- DullPointer
- Tekito-256
- JBMagination2
- hide0123
- Gruetzig
- Pixel-Pop

## Licensing
This software is licensed under the terms of the GPLv3. You can find a copy of the license in the LICENSE.txt file.

Files in the GDB stub are instead triple-licensed as MIT or "GPLv2 or any later version", in which case it's specified in the file header.

By contributing to this repository, you agree to license your changes to the project's owners.
