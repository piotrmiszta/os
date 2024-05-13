# OS Cortex M
CortexM real time os, writing for educational purpose, currently only for CortexM4/CortexM7.

## Run
OS is currently tested on stm32h755 nucleo device,
kernel should compile for any CortexM4/M7 device but example is writted for h755 stm32 device

```
git clone https://github.com/piotrmiszta/os
cd os
```

for CortexM4

```
cd cm4
make flash
```

for CortexM7

```
cd cm7
make flash
```

TODO: Create makefile for flash two cores in one time

## Futures
TODO: update this list

## License
MIT License

Copyright (c) 2024 Piotr Miszta

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
