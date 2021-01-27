.. RECONR documentation master file, created by
   sphinx-quickstart on Wed Aug 12 08:35:52 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Modern RECONR Module
====================
The RECONR module is used to reconstruct resonance cross sections from resonance parameters and to reconstruct cross sections from ENDF nonlinear interpolations schemes. The output is written as a pointwise-ENDF (PENDF) file with all cross sections on a unionized energy grid suitable for linear interpolation to within a specified tolerance. Redundant reactions are reconstructed to be exactly equal to the sum of their reconstructed and linearized parts at all energies. 

This is the documentation for the modern RECONR module [#adapted]_. Please see the following pages for more information.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   Overview.rst
   running
   CodeDescription/index.rst
   zbibliography.rst

.. [#adapted] Much of this documentation was taken from the `NJOY2016 manual <https://github.com/njoy/NJOY2016-manual>`_ and adapted as needed for the modern module in NJOY21.


Indices and tables
==================

* :ref:`genindex`
* :ref:`search`
