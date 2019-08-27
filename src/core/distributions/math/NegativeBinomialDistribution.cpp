#include "NegativeBinomialDistribution.h"
#include "DistributionNegativeBinomial.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"

using namespace RevBayesCore;

/** NegatvieBinomialDistribution Constructor
 * @param m the number of failures
 * @param q the probability of success
 *
 */

NegativeBinomialDistribution::NegativeBinomialDistribution(const TypedDagNode<long> *m, const TypedDagNode<double> *q) : TypedDistribution<long>( new long( 0 ) ),
    r( m ),
    p( q )
{
    
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( r );
    addParameter( q );
    
    *value = RbStatistics::NegativeBinomial::rv(r->getValue(), p->getValue(), *GLOBAL_RNG);
}


NegativeBinomialDistribution::~NegativeBinomialDistribution(void) {

    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



NegativeBinomialDistribution* NegativeBinomialDistribution::clone( void ) const {

    return new NegativeBinomialDistribution( *this );
}


double NegativeBinomialDistribution::computeLnProbability( void )
{
    
    // check that the value is inside the boundaries
    if ( *value < 0 )
    {
        return RbConstants::Double::neginf;
    }
    
    return RbStatistics::NegativeBinomial::lnPdf(r->getValue(), p->getValue(), *value);
}



void NegativeBinomialDistribution::redrawValue( void ) {

    *value = RbStatistics::NegativeBinomial::rv(r->getValue(), p->getValue(), *GLOBAL_RNG);
}


/** Swap a parameter of the distribution */
void NegativeBinomialDistribution::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{

    if (oldP == p)
    {
        p = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == r)
    {
        r = static_cast<const TypedDagNode<long>* >( newP );
    }

}


