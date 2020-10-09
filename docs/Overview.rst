********************
Overview
********************

RECONR module is used to reconstruct resonance cross sections from resonance parameters and to reconstruct cross sections from ENDF nonlinear interpolation schemes.  The output is written as a pointwise-ENDF [#]_ file with all cross sections on a unionized energy grid suitable for linear interpolation to within a specified tolerance.  Redundant reactions (for example, total inelastic, charged-particle reactions) are reconstructed to be exactly equal to the sum of their reconstructed and linearized parts at all energies. 


Linearization
=============
In the evaluation file, the cross sections are provided using a handful of interpolations and often uses multiple interpolation regions to represent a cross section across the full energy range. 

RECONR linearizes the cross sections and combines the interpolation regions into a single region for each reaction. Modern RECONR uses the broken-stick algorithm to perform the linearization. This algorithm is implemented in our `twig <https://github.com/njoy/twig>`_ component.

.. _broken-stick:

Broken Stick Algorithm
----------------------
Here is the original paper of the algorithm: :cite:`Poh:2017Proba-0` .


Energy Grid Unionization
========================

Resonance Reconstruction
========================
The actual resonance reconstruction is performed using the `resonanceReconstruction <https://github.com/njoy/resonanceReconstruction>`_ repository, one of the `components <https://docs.njoy21.io/Components/>`_ that make up NJOY21. The details of how the reconstruction is performed is not given here.

``resonanceReconstruction`` provides functions that return cross section values for a given energy. The linearization of the reconstructed cross section is done using these functions and the linearization strategy described :ref:`earlier <broken-stick>`.

``resonanceReconstruction`` supports the following resonance formalisms:

- Resolved

   - Single- and Multi-level Breit Wigner (LRF=1 and 2)
   - Reich-Moore (LRF=3)
   - R-Matrix Limited (LRF=7)

- Unresolved

   - Energy independent (Case A)
   - Energy dependent fission widths (Case B)
   - Energy dependent (Case C)

   Only the fully energy dependent parameters were implemented since the other two formats can be translated to the more general case of fully energy dependent parameters.

.. note::

   The Adler-Adler formalism (LRF=4) is not supported in modern RECONR. The last evaluation that used the Adler-Adler formalism was :sup:`233`\ U from ENDF/B-VI.8, which is now 20+ years old. If there is a need to process evaluations with the Adler-Adler formalism, please use `Legacy NJOY <https://github.com/njoy/NJOY2016>`_.


Once the cross sections have been reconstructed from the parameters given on the evaluation file, they are added to the background cross section.

Summation of Redundant Reactions
================================


But the RECONR module does *so much more* than just :

* Linearization of cross sections
* Reconstruction of cross sections from resolved and unresolved resonance parameters
* Unionization of energy grid
* Addition of reconstructed cross sections to background cross sections

  Because the cross sections (reconstructed and background) have been linearized and are on the same energy grid, adding the cross sections is just adding two numbers. 

* Summation of cross sections to calculate redundant reactions

.. [#] Often refered to as a PENDF file.
