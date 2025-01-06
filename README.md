# DriveEnumeration


## Overview
This tool enumerates all loaded drivers in the Windows operating system, providing essential details for debugging, system analysis, and security research.

**Hash**: `$2y$06$nk.Eff9h9sSL9P8yKqhal.H.ctBvPU8fMQBUhd28eTgbUuBVPqSz.`
## Hash
- This is Who You are
---

## Features
- Lists loaded drivers with:
  - Base address
  - Size
  - Name
  - Full file path
- Uses `NtQuerySystemInformation` with `SystemModuleInformation`.

---

## Requirements
- Windows OS (Windows 7+).
- Administrative privileges. (In LatestVersion Windows 11)
- Dependencies:
  - `Windows.h`
  - `winternl.h`

---

## Usage
1. **Build**: Compile the source code using a C++ compiler (e.g., Visual Studio).
2. **Run**: Execute the program as an administrator.
3. **Output Example**: