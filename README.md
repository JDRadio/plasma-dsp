# plasma-dsp
C++ DSP library

## Goal
To be the de facto DSP library used in new projects, and the fastest, for the RF portion (acquisition, tracking, recovery) of DSP with SDRs.

## What it's NOT
- Currently, it's not in a functional state, not unless you know exactly how to get to the usable code
- A replacement to anything, it's just a different library
- For embedded hardware, although this could change in the future
- For data processing after you recover the bit stream, there are a plethora of libraries for that
- A fixed-point integer DSP library, modern processors work just as well with floating point when optimized properly

## What it IS
- A work in progress, I'm still trying to figure out the best format for this library so please don't use it just yet
- A collection of optimized blocks to perform RF DSP operations on both receivers and transmitters
- A floating-point DSP library, modern processors can process those just as fast as fixed-point when optimized properly
- For RF processing, to get you from the RF to the bit stream (receivers), or from the bit stream to the RF (transmitters)

## What it could be
- A teaching tool: I will try to document every block and its inner workings, and provide a "typical" implementation alongside the "optimized" implementation
- You could use this for audio processing, but most of the blocks will not have a purpose and there are much better specialized libraries available
