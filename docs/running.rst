**************
Running RECONR
**************

The output of RECONR is an ENDF-like file that is generally referred to as a *PENDF* file, for pointwise-ENDF.

Input Description
=================
RECONR follows the same rules  as all the other modules. Please see the more `general information <https://docs.njoy21.io/LegacyInputs.html>`_ about NJOY inputs if unfamiliar.
RECONR input is given in six different cards. Each card has one or more arguments and is (optionally) ended with the forward slash: ``/``.

.. note::
   The input for modern RECONR is the same as for Legacy RECONR, but modern RECONR use all of the arguments. Listed here are only the required arguments. If other arguments are given, they are silently ignored.

Card 1
------

.. glossary::
   ``nendf``
      Unit for ENDF tape input. See `input/output unit <https://docs.njoy21.io/LegacyInputs.html>`_.
   
   ``npend``
      Unit for PENDF tape output. See `input/output unit <https://docs.njoy21.io/LegacyInputs.html>`_.

Card 2
------

.. glossary::
   ``tlabel``
      Single-quote delimited string of 66 ASCII characters or less, ending with a forward slash. The value of ``tlabel`` will be used as the TPID for the PENDF file.

Card 3
------

.. glossary::
   ``mat``
      ENDF Material number to be reconstructed. This is a positive four-digit number.

   ``ncards``
      Number of cards of descriptive text (default=0)

   ``ngrid``
      Number of user specified energy grid points to be added (default=0)

Card 4
------

.. _err:

.. glossary::
   

   ``err``
      Fractional reconstruction tolerance used to linearize cross sections and reconstruct resonance parameters.

Please note that Legacy RECONR does have limited capabilities for reconstruction at a particular temperature. Modern RECONR does not and reconstructs everything at zero Kelvin.

Card 5
------

.. glossary::
   ``cards``
      Single-quote delimited string of 66 ASCII characters or less, ending with a forward slash. The value of the argument is added to MF=1/MT=451. This card must be repeated ``ncards`` times where ``ncards`` is given in Card 3.

Card 6
------

.. glossary::
   ``enode``
      A list of energy grid points with units of electon volt. These energy grid points will be added to the generated list of energy points.

Cards 3, 4, 5, and 6 are repeated for every material that should be processed. While this is generally not done anymore, the capability still exists.

RECONR is terminated with a final card that is just ``0 /``

Examples
========

Simple Input
------------

.. literalinclude:: inputs/simple.njoy
   :language: html

Input with Descriptive Text and User-defined Energies
-----------------------------------------------------

.. literalinclude:: inputs/cardsandenode.njoy
   :language: html

Multi-Material Input
--------------------
.. literalinclude:: inputs/multiMaterial.njoy
   :language: html
