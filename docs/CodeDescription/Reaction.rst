Reactions
*********
So there are really three different reaction classes. 

``Reaction``
   Contains data for a standard (i.e., from an ENDF MF=3 section or a GNDS ``reaction``) reaction.

``UnresolvedReaction``
   Contains data for unresolved reactions.

``PhotonProduction``
   Contains data for photon production reactions (i.e., from an ENDF MF=13 section).

They each contain a variety of parameters read from the evaluation file as well as containing a representation of the cross sections. The representation changes as RECONR progress. It begins as an interpolation table and ends as pairs of energy, cross section values.

