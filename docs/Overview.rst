********************
Overview
********************

RECONR module is used to reconstruct resonance cross sections from resonance parameters and to reconstruct cross sections from ENDF nonlinear interpolation schemes.  The output is written as a pointwise-ENDF [#]_ file with all cross sections on a unionized energy grid suitable for linear interpolation to within a specified tolerance.  Redundant reactions (for example, total inelastic, charged-particle reactions) are reconstructed to be exactly equal to the sum of their reconstructed and linearized parts at all energies. 


Linearization
=============
In the evaluation file, the cross sections are provided using a handful of interpolations and often uses multiple interpolation regions to represent a cross section across the full energy range. 

RECONR linearizes the cross sections and combines the interpolation regions into a single region for each reaction. Modern RECONR uses the broken-stick algorithm :cite:`Poh:2017Proba-0` to perform the linearization---which really is the same algorithm that is used in Legacy NJOY. This algorithm is implemented in our `twig <https://github.com/njoy/twig>`_ component. This is sometimes known as `Segmented Regrestion <https://en.wikipedia.org/wiki/Segmented_regression>`_.

.. _broken-stick:

Broken Stick Algorithm
----------------------


The linearization process begins with a sorted grid of energy values. For cross section linearization, the initial energy grid comes from the `Energy Grid Unionization`_. For linearization of reconstructed resonances, the initial energy grid is the peaks and half-height energies of the resonances. 

The linearization "stack" is primed with two starting values; the first two values of the initial energy grid. The stack is said to be inverted because the lower energy is at the top.

This interval or panel is now divided into two parts, and the cross section computed at the intermediate point is compared to the result of linear interpolation between the adjacent points. If the two values do not agree within various criteria, the top of the stack is moved up one notch, and the new value is inserted.  The code then repeats the checking process for the new (smaller) interval at the top of the stack.  The top of the stack rises until convergence is achieved for the top interval.  The top energy and cross section are then saved on a scratch file, the stack index is decremented, and the checks are repeated.  This process is continued with the top of the stack rising and falling in response to the complexity of the cross section until the entire panel :math:`\Delta E` has been converged.  The stack is then reprimed with the bounds of the next panel.  The process continues until the entire energy range has been processed.

The convergence criterion used for linearization is that the linearized cross section at the intermediate point is within the fractional tolerance ``err`` of the actual cross section specified by the ENDF law; ``err`` is one of the :ref:`input arguments <err>` for RECONR.

When linearizing with this procedure, one often enters into the situation---particularly with a discontinunity---where adjacent energy values become so close they will be rounded to the same number when written out [#]_. Modern RECONR avoid this by declaring the linearization "converged" when adjacent energy points are within a relative difference of 1E-7, regardless of the specified tolerance, ``err``.

This process is described graphically in :numref:`fig:linearize`.

.. _fig:linearize:

.. figure:: images/Linearize.*
   :alt: Linearization demonstration.

   Line 1 shows the two initial points (the lower energy is higher in the stack).  In line 2, a new point has been calculated at the midpoint, but the result was not converged, and the new point has been inserted in the stack.  In line 3, the midpoint of the top panel has been checked again, found to be not converged, and inserted into the stack.  The same thing happens in line 4.  In line 5, the top panel is found to be converged, and the top point (5) has been written out.  The same thing happens in line 6.  In line 7, the top panel is tested and found to be not converged. The midpoint is added to the stack.  Finally, in line 8, the top panel is found to be converged, and the top point is written out.  This leaves two points in the stack (see line 9). Note that the energy points come off the stack in the desired order of increasing energy, and that only one point has to be moved up in the stack as each new result is inserted.
   

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
.. [#] In ENDF, the precision is limited to seven significant digits.
