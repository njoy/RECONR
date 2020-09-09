**************
Running RECONR
**************

The output of RECONR is an ENDF-like file that is generally referred to as a *PENDF* file, for pointwise-ENDF.

Input Description
=================
RECONR input is given in six different cards. Each card has one or more arguments and is (optionally) ended with the forward slash: ``/``.

**Note:** The cards and arguments listed below are for Legacy [#Legacy]_ NJOY. While still valid, not all arguments are used in NJOY21.

Card 1
------

.. glossary::
   nendf
      **Unit for ENDF tape input** This is an integer number---between 20 and 99 inclusive---that specifies the input tape "number". RECONR will look for a filename of ``tape##`` where ``##`` is the value of ``nendf``; e.g., a value of ``20`` for ``nendf`` means RECONR will open the file ``tape20``.
   
   npend
      **Unit for PENDF tape output** This is also an integer number with the same rules as ``nendf``. It is the tape filename where the output file will be written. 

Card 2
------

.. glossary::
   tlabel

Card 3
------

.. glossary::
   mat
      ENDF Material number to be reconstructed.

   ncards
   ngrid

Card 4
------

.. glossary::
   err
   tempr
   errmax
   errint

Card 5
------

.. glossary::
   cards

Card 6
------

.. glossary::
   enode

Simple Input
============

.. literalinclude:: inputs/reconr1.njoy
   :language: html

Multi-Material Input
====================
.. literalinclude:: inputs/reconr2.njoy
   :language: html

.. [#Legacy] Legacy NJOY refers to the Fortran-based NJOY, namely, NJOY2016 and its predecessors.
