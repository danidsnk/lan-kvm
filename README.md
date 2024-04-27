# Lan KVM
Lan KVM is a network-based keyboard and mouse sharing solution, designed for environments where you have two computers but only one monitor. This allows you to control both computers using a single set of keyboard and mouse.
## Compatibility
| OS      | Server  | Client
| ------- | ------- | -------
| Linux   | :heavy_check_mark: | :x:
| MacOS   | :x: | :heavy_check_mark:
## Dependencies
- Boost
- libevdev (Linux only)
## Build
```sh
mkdir build
cd build
cmake ..
make server
# or
make client
```
## Usage
### Server
```sh
./server --mouse <mouse_device> --keyboard <keyboard_device> --port <port>
```
### Client
```sh
./client -h <host> -p <port>
```
### Example
1. run client on your mac
2. run this script on a linux machine to switch to mac
```sh
#!/usr/bin/env bash

ddcutil setvcp 60 0x0f

server \
    -mouse "my mouse" \
    -keyboard "my keyboard" \
    -port 1234

ddcutil setvcp 60 0x12
```
3. press numpad enter to disconnect and switch back to linux

The `ddcutil` command is used to control the display settings via DDC/CI. Modify the VCP code according to your monitor's specifications.

User must be in the `input` group.

Use `cat /proc/bus/input/devices` to see all devices.
## License
MIT
