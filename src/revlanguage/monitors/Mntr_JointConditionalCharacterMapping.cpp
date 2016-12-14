//
//  Mntr_JointConditionalCharacterMapping.cpp
//  RevBayes_development_branch
//
//  Created by will freyman on 12/13/16.
//  Copyright (c) 2016 will freyman. All rights reserved.
//

#include "Mntr_JointConditionalCharacterMapping.h"

#include "AbstractCharacterData.h"
#include "ArgumentRule.h"
#include "Model.h"
#include "Natural.h"
#include "NaturalNumbersState.h"
#include "RlMonitor.h"
#include "RbException.h"
#include "RevObject.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlModel.h"
#include "RlTimeTree.h"
#include "RlBranchLengthTree.h"
#include "RlString.h"
#include "StandardState.h"
#include "TypeSpec.h"


using namespace RevLanguage;


Mntr_JointConditionalCharacterMapping::Mntr_JointConditionalCharacterMapping(void) : Monitor()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Mntr_JointConditionalCharacterMapping* Mntr_JointConditionalCharacterMapping::clone(void) const
{
    
    return new Mntr_JointConditionalCharacterMapping(*this);
}


void Mntr_JointConditionalCharacterMapping::constructInternalObject( void )
{
    
    const std::string& file_name      = static_cast<const RlString  &>( filename->getRevObject()    ).getValue();
    const std::string& sep            = static_cast<const RlString  &>( separator->getRevObject()   ).getValue();
    int                print_gen      = static_cast<const Natural   &>( printgen->getRevObject()    ).getValue();
    bool               app            = static_cast<const RlBoolean &>( append->getRevObject()      ).getValue();
    std::string		   character_type = static_cast<const RlString  &>( monitorType->getRevObject() ).getValue();
    
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>*   cdbdp_tdn = static_cast<const RevLanguage::Tree&>( cdbdp->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::Tree>* cdbdp_sn  = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Tree>* >( cdbdp_tdn );
    
    delete value;
    if (character_type == "NaturalNumbers")
    {
        RevBayesCore::JointConditionalCharacterMappingMonitor<RevBayesCore::NaturalNumbersState> *m;
        m = new RevBayesCore::JointConditionalCharacterMappingMonitor<RevBayesCore::NaturalNumbersState>(cdbdp_sn, (unsigned long)print_gen, file_name, sep);
        m->setAppend( app );
        value = m;
    }
    else if (character_type == "Standard")
    {
        RevBayesCore::JointConditionalCharacterMappingMonitor<RevBayesCore::StandardState> *m;
        m = new RevBayesCore::JointConditionalCharacterMappingMonitor<RevBayesCore::StandardState>(cdbdp_sn, (unsigned long)print_gen, file_name, sep);
        m->setAppend( app );
        value = m;
    }
    else
    {
        throw RbException( "Incorrect character type specified. Valid options are: NaturalNumbers, Standard" );
    }
    
}


/** Get Rev type of object */
const std::string& Mntr_JointConditionalCharacterMapping::getClassType(void)
{
    
    static std::string revType = "Mntr_JointConditionalCharacterMapping";
    
    return revType;
    
}


/** Get class type spec describing type of object */
const TypeSpec& Mntr_JointConditionalCharacterMapping::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
    return rev_type_spec;
    
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Mntr_JointConditionalCharacterMapping::getMonitorName( void ) const
{
    
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "JointConditionalCharacterMapping";
    
    return c_name;
    
}


/** Return member rules (no members) */
const MemberRules& Mntr_JointConditionalCharacterMapping::getParameterRules(void) const
{
    
    static MemberRules monitor_rules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        monitor_rules.push_back( new ArgumentRule("cdbdp"          , TimeTree::getClassTypeSpec(),  "The character dependent birth-death process to monitor.",     ArgumentRule::BY_REFERENCE, ArgumentRule::ANY, NULL) );
        monitor_rules.push_back( new ArgumentRule("filename"       , RlString::getClassTypeSpec() , "The file to save sampled character histories.",               ArgumentRule::BY_VALUE,     ArgumentRule::ANY ) );
        monitor_rules.push_back( new ArgumentRule("type"           , RlString::getClassTypeSpec() , "The character type to monitor.",                              ArgumentRule::BY_VALUE,     ArgumentRule::ANY ) );
        monitor_rules.push_back( new ArgumentRule("printgen"       , Natural::getClassTypeSpec()  , "The frequency how often we print. 1 by default.",             ArgumentRule::BY_VALUE,     ArgumentRule::ANY, new Natural(1) ) );
        monitor_rules.push_back( new ArgumentRule("separator"      , RlString::getClassTypeSpec() , "The delimiter between variables. \t by default.",             ArgumentRule::BY_VALUE,     ArgumentRule::ANY, new RlString("\t") ) );
        monitor_rules.push_back( new ArgumentRule("append"         , RlBoolean::getClassTypeSpec(), "Should we append an existing file exists? False by default.", ArgumentRule::BY_VALUE,     ArgumentRule::ANY, new RlBoolean(false) ) );
        rules_set = true;
    }
    
    return monitor_rules;
    
}


/** Get type spec */
const TypeSpec& Mntr_JointConditionalCharacterMapping::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
    
}


/** Get type spec */
void Mntr_JointConditionalCharacterMapping::printValue(std::ostream &o) const
{
    
    o << "Mntr_JointConditionalCharacterMapping";
    
}


/** Set a member variable */
void Mntr_JointConditionalCharacterMapping::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "" ) {
        vars.push_back( var );
    }
    else if ( name == "filename" )
    {
        filename = var;
    }
    else if ( name == "separator" )
    {
        separator = var;
    }
    else if ( name == "type" )
    {
        monitorType = var;
    }
    else if ( name == "cdbdp" )
    {
        cdbdp = var;
    }
    else if ( name == "printgen" )
    {
        printgen = var;
    }
    else if ( name == "append" )
    {
        append = var;
    }
    else
    {
        Monitor::setConstParameter(name, var);
    }
    
}