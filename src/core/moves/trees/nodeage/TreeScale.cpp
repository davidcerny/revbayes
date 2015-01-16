//
//  TreeScale.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/12/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TreeScale.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

TreeScale::TreeScale(StochasticNode<TimeTree> *t, double d, bool tu, double w) : SimpleMove( t, w, tu),
    tree( t ),
    delta( d )
{
        
}



/* Clone object */
TreeScale* TreeScale::clone( void ) const
{
    
    return new TreeScale( *this );
}



const std::string& TreeScale::getMoveName( void ) const
{
    static std::string name = "Tree-Scale";
    
    return name;
}


/** Perform the move */
double TreeScale::performSimpleMove( void )
{
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    TimeTree& tau = tree->getValue();
    
    TopologyNode& node = tau.getRoot();
    
    // we need to work with the times
    double my_age      = node.getAge();
    
    // now we store all necessary values
    storedAge = my_age;
    
    // draw new ages and compute the hastings ratio at the same time
    double u = rng->uniform01();
    double scalingFactor = std::exp( delta * ( u - 0.5 ) );
    
    // rescale the subtrees
    TreeUtilities::rescaleSubtree(&tau, &node, scalingFactor );
    
    // rescale root age
    tau.setAge( node.getIndex(), my_age * scalingFactor );
    
    // compute the Hastings ratio
    double lnHastingsratio = log( scalingFactor ) * tau.getNumberOfInteriorNodes();
    
    return lnHastingsratio;
}


void TreeScale::printParameterSummary(std::ostream &o) const
{

    o << "delta = " << delta;

}


void TreeScale::rejectSimpleMove( void )
{
    
    TimeTree& tau = tree->getValue();
    
    TopologyNode& node = tau.getRoot();
    
    // undo the proposal
    TreeUtilities::rescaleSubtree(&tau, &node, storedAge / node.getAge() );
    
    // undo root age
    tau.setAge( node.getIndex(), storedAge );
    
    
#ifdef ASSERTIONS_TREE
    if ( fabs(storedAge - node.getAge()) > 1E-8 )
    {
        throw RbException("Error while rejecting SubtreeScale proposal: Node ages were not correctly restored!");
    }
#endif
    
}


void TreeScale::swapNode(DagNode *oldN, DagNode *newN)
{
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    tree = static_cast<StochasticNode<TimeTree>* >(newN);
    
}


void TreeScale::tune( void )
{
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.234 )
    {
        delta *= (1.0 + ((rate-0.234)/0.766) );
    }
    else
    {
        delta /= (2.0 - rate/0.234 );
    }
}

