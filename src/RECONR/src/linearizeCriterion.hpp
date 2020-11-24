bool linearizeCriterion( double trial, double reference,
                         double xLeft, double xRight,
                         double relTol, double absTol ){

    static constexpr double infinity = 
        std::numeric_limits< double >::infinity();
    if( xRight == std::nextafter( xLeft, infinity ) ){ return true; }

    // Limit of ENDF-6 precision
    auto ratio = 1.0 - ( xLeft/xRight );
    if( ratio < 1E-7 ){ return true; }

    auto diff = std::abs( trial - reference );
    auto reldiff = (diff/reference);

    return ( diff < absTol ) or ( reldiff < relTol );
}
