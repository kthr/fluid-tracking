Licensing information
===============
The code of this library is build for research purposes and published under the MIT license. It implements the tracking approach published in the paper [FluidTracks](https://link.springer.com/chapter/10.1007/978-3-642-28502-8_12). The Algorithm is improved by the use of graph cut algorithms for image segmentation and label disambiguation. Third party algorithms/components in the lib directory have their own license though. Especially the gco algorithm, is only allowed to be redistributed for research purposes.

Relevant publications:

[http://www.csd.uwo.ca/~yuri/Papers/pami04.pdf](http://www.csd.uwo.ca/~yuri/Papers/pami04.pdf)

[http://www.csd.uwo.ca/faculty/olga/Papers/pami01_final.pdf](http://www.csd.uwo.ca/faculty/olga/Papers/pami01_final.pdf)

[http://www.csd.uwo.ca/~yuri/Papers/pami04.pdf](http://www.csd.uwo.ca/~yuri/Papers/pami04.pdf)

[http://www.cs.ucl.ac.uk/staff/V.Kolmogorov/papers/KZ-PAMI-graph_cuts.pdf](http://www.cs.ucl.ac.uk/staff/V.Kolmogorov/papers/KZ-PAMI-graph_cuts.pdf)
[http://www.csd.uwo.ca/~yuri/Papers/ijcv10_labelcost.pdf](http://www.csd.uwo.ca/~yuri/Papers/ijcv10_labelcost.pdf)

Installation
--------------
First clone the repository with
```bash
git clone https://gitlab.com/kth/fluid-tracking.git 

Then create a directory "build" inside the cloned repository
```bash
cd fluid-tracking
mkdir build
cd build
```
Finally create the binary with the following commands
with
```bash
cmake ..
make
```
