****************
Code Description
****************

All modern NJOY components are being written in C++. They can be used stand-alone, but are also used in connection with NJOY21. 

The RECONR module is implemented as a class with a single public method, the call operator, which is called by NJOY21 and the input parameters are passed to it.

.. code-block:: c++

   class RECONR {
   /* private methods */
   public:
      void operator()( const nlohmann::json& njoyArgs, 
                       std::ostream& output,
                       std::ostream& error,
                       const nlohmann::json& other ){
      }
   };

.. note::
   All NJOY21 modules have the same argument list for the call operator.

The arguments to ``operator()`` are:

.. glossary::
   ``njoyArgs``
      This argument is a JSON object translation of the original input arguments. When RECONR is executed, this JSON object is echoed to the output

   ``output``
      Where informational messages are printed. You can specify what file this is written to using the ``--output`` command-line option in NJOY21. When not specified, these messages are written to ``stdout``.

   ``error``
      Where error messages are printed. You can specify what file this is written to using the ``--error`` command-line option in NJOY21. When not specified, these messages are written to ``stderr``.

   ``other``
      This argument is currently unused in RECONR. It is a place holder for if/when we decide to give additional data to *all* NJOY21 modules.

The call operator is analogous to the ``reconr`` subroutine in Legacy NJOY. It then makes a series of calls to other methods to perform all the calculations that are performed.

The data needed to perform the operations done in RECONR is contained in a ``ResonanceReconstructionDataDelivery`` :doc:`R2D2` object. After the input evaluation file is read in with the ``getEvaluated`` function, the data is passed to the ``ResonanceReconstructionDataDelivery`` constructor to:

RECONR Methods
--------------

Listed here are the different methods (i.e., functions) called by the ``RECONR`` class to perform the operations of the RECONR module.

``getEvaluated``
    Read the evaluated file; i.e., ENDF or (eventually) GNDS. This returns a ``std::variant`` which is then passed to ``findR2D2``.

``findR2D2``
    Create an instance of the :ref:`ResonanceReconstructionDatatDelivery <R2D2>` object.

``linearizeXS`` 
   Linearize all of the background cross sections and photon production cross sections. It uses the interpolation library to interpolate between cross section values.

``unionizeEnergyGrid``
    Create an energy grid used as a common energy grid for reconstruction. It uses these energy values:

    - Cross section energies,
    - Photon production energies,
    - Resonance range boundaries, and
    - Resonance energies.

    It is in this method that the infamous ``sigfig`` method is used to avoid discontinuities. When two adjacent energy values are found, the first value is nudged down and the second value is nudged up so that they are no longer identical. ``sigifig`` does the nudging.

    I tried to remove the need for ``sigfig``, but it caused problems at the interface of the resonance regions, giving incorrect answers.

``reconstructResonances``
   This is an overloaded function that, well, reconstructs the resonances based on the formalism. These functions call to `resonanceReconstruction <https://github.com/njoy/resonanceReconstruction>`_ to do the actual work. The function creates a linear-linear interpolation table for each of the reactions that are reconstructed and adds them to the ``ResonanceReconstructionDataDelivery`` instance.

``reconstructUnresolved``
   This method reconstructs the unresolved resonances, also using `resonanceReconstruction <https://github.com/njoy/resonanceReconstruction>`_. The function creates a linear-linear interpolation table for each of the reactions that are reconstructed and adds them to the ``ResonanceReconstructionDataDelivery`` instance.

``reconstructCrossSections``
   After all the resonances have been reconstructed and the energy grid unionized, ``reconstructCrossSections`` creates :math:`(E, \sigma)` pairs (actually stored as a ``std::pair< std::vector< double >, std::vector< double > >``). 

``combineUnresolved``
    Combine the unresolved cross sections with the background cross sections. This is all dependent on the value of ``LSSF`` flag.

``combineReconstructed``
    This will add the reconstructed cross sections to the background cross sections.

``summateUnresolved``
    This sums up the unresolved cross sections and---depending on the ``LSSF`` flag---adds it to the background cross section.

``summateReactions``
    Add up all the partial cross sections to calculate the redundant (e.g., total) cross section. To determine the definition of what is redundant and what is not, it uses the ENDF definition of a redundant cross section---Table 14 in the current edition as of this writing.

    .. note:: 
    
       NJOY21 ignores redundant cross sections as given in the evaluation file. It will simply recaluclate the redundant cross section from its partials. If there are no partials, it treats the redundant reaction as any other reaction.
    

``summateProductions``
    Similar to ``summateReactions``, this adds up the partial photon production cross sections to create the redundant photon production cross sections.

``truncateReactions``
    All the cross sections are evaluated on the same energy grid. For some reactions (i.e., threshold), the cross section is zero when the energy is less than the :math:`Q`-value. This function will remove energy, cross section values from the beginning of a reaction when the cross section value is zero. 

.. toctree::
   :maxdepth: 2
   :caption: Supporting classes

   R2D2.rst
   ProcessedEvaluation.rst
   Reaction.rst

Additional Components
=====================
RECONR takes advantage of a number of other components. Some of these components are libraries that we have developed with the bigger NJOY21 project, others components are third-party libraries that are available with a compatible Open Source license. Listed here are some of those components.

**Internally Developed Components**
While these components/libraries are internally developed as part of the NJOY21 project, they are generally available on the `NJOY GitHub site <https://github.com/njoy>`_.

* `ENDFtk <https://github.com/njoy/ENDFtk>`_ is used to read and write ENDF-formatted data.
* `resonanceReconstruction <https://github.com/njoy/resonanceReconstruction>`_ contains all the capabilities for performing the reconstruction of unresolved and resolved resonance.
* `elementary <https://github.com/njoy/elementary>`_ is used to create reaction identifiers given the products and reactants of a nuclear reaction.
* `interpolation <https://github.com/njoy/interpolation>`_ is a library that can make interpolation tables that can then be, ahem, interpolated using various interpolation schemes.
* `constants <https://github.com/njoy/constants>`_ is a library providing mathematical and physical constants.
* `dimwits <https://github.com/njoy/DimensionalAnalysis>`_ is a library providing dimensional analysis with units.


**Third-Party Components**

* `Catch <https://github.com/catchorg/Catch2>`_ is a testing frame work that we use to test our code.
* `Range-v3 <https://github.com/ericniebler/range-v3>`_ a wonderful little tool that has been (mostly) adopted into the C++20 standard. As of this writing, we are using a rather old version of this library.
* `JSON for Modern C++ <https://nlohmann.github.io/json/>`_ is used for working with JSON data.

