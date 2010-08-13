/**
 * @file
 * This file contains the implementation of Move_mslide,
 * which moves a real value uniformly within a sliding window.
 *
 * @brief Implementation of Move_mslide
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "Distribution.h"
#include "Move_mslide.h"
#include "PosReal.h"
#include "RandomNumberGenerator.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "StringVector.h"
#include "Workspace.h"

#include <cmath>


/** Constructor for parser */
Move_mslide::Move_mslide(void)
    : SimpleMove(getMemberRules()) {
}


/** Constructor for internal use */
Move_mslide::Move_mslide(StochasticNode* node, double delta, double weight, RandomNumberGenerator* rng)
    : SimpleMove(getMemberRules()) {

    setValue("delta",  new PosReal(delta));
    setValue("weight", new PosReal(weight));
    setValue("rng",    rng);

    setNodePtr(node);
}


/** Clone object */
RbObject* Move_mslide::clone(void) const {

    return new Move_mslide(*this);
}


/** Get class vector describing type of object */
const StringVector& Move_mslide::getClass() const {

    static StringVector rbClass = StringVector(Move_mslide_name) + Move::getClass();
    return rbClass;
}


/** Return member rules */
const MemberRules& Move_mslide::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) {

        memberRules.push_back(new ArgumentRule("delta", PosReal_name));

        /* Inherit weight and rng from Move, put it at back */
        const MemberRules& inheritedRules = Move::getMemberRules();
        memberRules.insert(memberRules.end(), inheritedRules.begin(), inheritedRules.end()); 

        rulesSet = true;
    }

    return memberRules;
}


/** Perform the move */
double Move_mslide::perform(std::set<StochasticNode*>& affectedNodes) {

    RbDouble*              valPtr = (RbDouble*)(nodePtr->getValuePtr(affectedNodes));
    RandomNumberGenerator* rng    = (RandomNumberGenerator*)(getValue("rng"));
    double                 delta  = ((RbDouble*)(getValue("delta")))->getValue();

    double minVal = ((RbDouble*)(nodePtr->getDistribution()->getMin()))->getValue();
    double maxVal = ((RbDouble*)(nodePtr->getDistribution()->getMax()))->getValue();

    double curVal = valPtr->getValue();
    double u      = rng->rv01();
    double newVal = curVal + (delta*(u-0.5));

    /* reflect the new value */
    do {
        if (newVal < minVal)
            newVal = 2.0 * minVal - newVal;
        else if (newVal > maxVal)
            newVal = 2.0 * maxVal - newVal;
    } while (newVal < minVal || newVal > maxVal);

    valPtr->setValue(newVal);
	
    return 0.0;
}


