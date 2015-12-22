#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "MetropolisHastingsMove.h"
#include "Move_NodeTimeSlideBeta.h"
#include "NodeTimeSlideBetaProposal.h"
#include "RbException.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_NodeTimeSlideBeta::Move_NodeTimeSlideBeta() : Move()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Move_NodeTimeSlideBeta* Move_NodeTimeSlideBeta::clone(void) const
{
    
	return new Move_NodeTimeSlideBeta(*this);
}


void Move_NodeTimeSlideBeta::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::Tree> *tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::Tree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Tree> *>( tmp );
    double d = static_cast<const RealPos &>( delta->getRevObject() ).getValue();
    double o = static_cast<const RealPos &>( offset->getRevObject() ).getValue();
    bool tu = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    
    RevBayesCore::Proposal *p = new RevBayesCore::NodeTimeSlideBetaProposal(t, d, o);
    value = new RevBayesCore::MetropolisHastingsMove(p,w,tu);
    
}


/** Get Rev type of object */
const std::string& Move_NodeTimeSlideBeta::getClassType(void) {
    
    static std::string revType = "Move_NodeTimeSlideBeta";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_NodeTimeSlideBeta::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Move_NodeTimeSlideBeta::getMoveName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "NodeTimeSlideBeta";
    
    return c_name;
}


/** Return member rules (no members) */
const MemberRules& Move_NodeTimeSlideBeta::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule( "tree", TimeTree::getClassTypeSpec(), "The tree variable on which this move operates.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        memberRules.push_back( new ArgumentRule( "delta"  , RealPos::getClassTypeSpec()  , "The concentration parameter.", ArgumentRule::BY_VALUE    , ArgumentRule::ANY       , new RealPos( 1.0 ) ) );
        memberRules.push_back( new ArgumentRule( "offset"  , RealPos::getClassTypeSpec()  , "The offset for the proposal density.", ArgumentRule::BY_VALUE    , ArgumentRule::ANY       , new RealPos( 2.0 ) ) );
        memberRules.push_back( new ArgumentRule( "tune"   , RlBoolean::getClassTypeSpec(), "Should we tune the concentration parameter during burnin?", ArgumentRule::BY_VALUE    , ArgumentRule::ANY       , new RlBoolean( true ) ) );

        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& Move_NodeTimeSlideBeta::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void Move_NodeTimeSlideBeta::printValue(std::ostream &o) const {
    
    o << "Move_NodeTimeSlideBeta(";
    if (tree != NULL) {
        o << tree->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void Move_NodeTimeSlideBeta::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "delta" )
    {
        delta = var;
    }
    else if ( name == "offset" )
    {
        offset = var;
    }
    else if ( name == "tune" )
    {
        tune = var;
    }
    else
    {
        Move::setConstParameter(name, var);
    }
    
}
