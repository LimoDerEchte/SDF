# SDF
This Is A Fork Of The Subnautica Data Framework Mod Made By Limo, Mostly For Testing Purposes.

## Copyright notice
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.

## Building Guide:-

1. Clone The Repo
```shell
git clone https://github.com/LabrynthKing/SDF
cd SDF
```
2. Pull Submodules (Need Access To Unreal Engine Repo To Succeed)
```shell
git submodule update --init --recursive
```
3. Build Using Your Preferred Method

## Building On Linux:-
After Trying To Build This Repo On Linux, I Encountered A Few Issues Due To How The SDK & UE4SS Are Setup. This Is Just What I Did To Fix These Issues, This May Or May Not Work For You.

Some Info About My Linux Installation:-
- Distro: CachyOS (Arch Based)
- Shell: Fish
- Using Hyprland (Don't Think This Is Important But Oh Well)

## Issues:-
1. Enums From The SDK Do Not Compile Due To `clang-cl` Enforcing Some Type Validation During Casting.
2. Windows Header Macro Pollution => `InterlockedCompareExchangePointer` & `MemoryBarrier` Clash With UE4SS' `FWindowsPlatformAtomics`
3. Const Qualifier Drop Errors In The SDK Files => `*DrumComponent = Parms.DrumComponent;`
4. MASM Assembler Emulation

## Fixes To Apply:-
1. Use `/clang:-Wno-c++11-narrowing` To Ignore The Enum Errors - **PRE-APPLIED**
2. In `UnrealDef.hpp`, Add This 
```c++
#pragma once

// Undefine Windows Macros

#undef InterlockedCompareExchangePointer
#undef MemoryBarrier
```
3. Explicitly Cast Qualifiers
```c++
// *DrumComponent = Parms.DrumComponent;
// *DrumComponent_0 = Parms.DrumComponent_0;
*DrumComponent = (class UPrimitiveComponent*)Parms.DrumComponent;
*DrumComponent_0 = (class UPrimitiveComponent*)Parms.DrumComponent_0;
```
4. For The MASM, I Used `JWasm`, Then I Used A Script To Filter Out Clang Flags & Link JWasm As `ml64`
```shell
yay -S jwasm
```
```shell
echo '#!/bin/bash
args=()
for arg in "$@"; do
    if [[ "$arg" != /clang:* && "$arg" != -w ]]; then
        args+=("$arg")
    fi
done
exec /usr/bin/jwasm -win64 "${args[@]}"' | sudo tee /usr/local/bin/ml64 > /dev/null

sudo chmod +x /usr/local/bin/ml64
```

## Contributing
This Repo Is Just A Fork For Testing Purposes So Sure Go Ahead If You Have Something Great To Add