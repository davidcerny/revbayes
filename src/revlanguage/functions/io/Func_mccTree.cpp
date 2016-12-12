#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Func_mccTree.h"
#include "ModelVector.h"
#include "NexusWriter.h"
#include "RbException.h"
#include "RevNullObject.h"
#include "RlBranchLengthTree.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlTraceTree.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "TreeSummary.h"
#include "TraceTree.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_mccTree* Func_mccTree::clone( void ) const
{
    
    return new Func_mccTree( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_mccTree::execute( void )
{
    
    TraceTree& tt = static_cast<TraceTree&>( args[0].getVariable()->getRevObject() );
    const std::string& filename = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    
    //int burnin = static_cast<const Integer &>(args[2].getVariable()->getRevObject()).getValue();
    //tt.getTreeSummary().setBurnin(burnin);
    
    RevBayesCore::Tree* tree = tt.getValue().mccTree();
    
    
    if ( filename != "" )
    {
        
        RevBayesCore::NexusWriter writer(filename);
        writer.openStream();
        
        std::vector<RevBayesCore::Taxon> taxa;
        tree->getRoot().getTaxa(taxa);
        RevBayesCore::Clade c( taxa );
        writer.writeNexusBlock(c);
        
        writer.writeNexusBlock(*tree);
        
        writer.closeStream();
        
    }
    
    return new RevVariable( new Tree( tree ) );
}



/** Get argument rules */
const ArgumentRules& Func_mccTree::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rules_set = false;
    
    if (!rules_set)
    {
        
        argumentRules.push_back( new ArgumentRule( "trace", TraceTree::getClassTypeSpec(), "The samples of trees from the posterior.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "file"     , RlString::getClassTypeSpec(), "The name of the file where to store the tree.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        //argumentRules.push_back( new ArgumentRule( "burnin"   , Integer::getClassTypeSpec(), "The number of trees to discard as burnin.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Integer(-1) ) );
        
        rules_set = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_mccTree::getClassType(void)
{
    
    static std::string revType = "Func_mccTree";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_mccTree::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_mccTree::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "mccTree";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_mccTree::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
const TypeSpec& Func_mccTree::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = Tree::getClassTypeSpec();
    return returnTypeSpec;
}
