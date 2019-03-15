# OTF2 Merger
Otf2Merger v0.1 unifies multiple OTF2 traces in one absolute resulting trace.
The merger is tested using traces produced by [Score-P](https://www.vi-hps.org/projects/score-p/).

## Prerequisites
Build and compile require the followings:
* C++17 (ISO/IEC 14882:2017)
* OTF2 version 2.1
* CMAKE version 3.10 or higher

## Supported trace features
* Serial 
* P-threads  
* OpenMP
* MPI 
* File I/O

> Note: The otf2Merger can merge all possible combinations of above mentioned
features, including in the traces.  

## Building and compiling
We used an open-source, cross-platform building system name CMake to build and test the merger. Just create a 'build' folder to do 'cmake' and use 'make' command to compile, as follows:

```
$ cmake ../
$ make
```
>Note: Load the otf2 module or set its paths during building process (cmake).

## Usage
In case of the successful compilation, use the --help to see the input command line options.
```
./bin/otf2-merger --help

OTF2 Merger unifies multiple input traces into absolute one. 

Commands:

  --traceFile <tfile>: Set trace files with absloute paths. (req.)
  --commGroupPattern <pname>: Provide a pattern name, to be followed by comm and group defintions in the unified trace. (req.)
  --outputPath <path>: Set an output trace path. (req.)
  --mergedTraceName <tname>: Set an output trace name, by default "traces". 
  --version: Display otf2 merger version.
  --help: Show help. 

Required (req.)
```
A user needs to input the following options to achieve a successful unified trace:

1) --traceFile - Input the number of trace files needs to be unified with their respective absolute paths.                                                                     
For example: --traceFile "/home/otf2Merger/traces/comp1/traces.otf2"
             --traceFile "/home/otf2Merger/traces/comp2/traces.otf2"

2) --commGroupPattern - Input the user defined pattern name to be used by Comm and Location Group Definitions in the unified trace.                
For example: --commGroupPattern "pycomp"

3) --outputPath - The user has to set the output path for the unified trace.                                                                                       For example: --outputPath "/home/otf2Merger/traces/unified"

4) --mergedTraceName - The user can provide the output trace name, by default it    is "traces". This particular argument is optional.

Example run:
```
./bin/otf2-merger --traceFile "trace1/traces.otf2" --traceFile "trace2/traces.otf2" --commGroupPattern matrix --outputPath "../output"
```

## Use Case
In order to enable the Score-P support for frameworks using heterogeneous distribution systems or custom node to node communications such as big data frameworks (spark, flink, PyCOMMPs etc) and machine learning frameworks (tensorflow etc), we have come up with this new tool called otf2Merger.
It combines all the measurements of traces from different nodes into one trace for complete performance evaluations.

## Clock properties riddle
Currently Clock properties of the traces are adjusted with respect to the first trace's clock properties in the list. We are computing the ratio (relative clock factor) of time resolution of respective trace to the time resolution of the first trace. The relative clock factor and global offsets of the first and respective trace are used to adjust the timestamps of the events.  
