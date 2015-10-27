/**
 * @file
 * This file contains the declaration of ConstructorFunction, which is used
 * for functions that construct objects using the RevLanguage.
 *
 * @brief Declaration of ConstructorFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-06-01 14:40:37 +0200 (Fri, 01 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: ConstructorFunction.h 1602 2012-06-01 12:40:37Z hoehna $
 */

#ifndef DistributionFunctionPdf_H
#define DistributionFunctionPdf_H

#include "RlTypedDistribution.h"
#include "RlTypedFunction.h"

#include <string>
#include <vector>

namespace RevLanguage {
    
    template <class valueType>
    class DistributionFunctionPdf : public TypedFunction<Real> {
        
    public:
        DistributionFunctionPdf(TypedDistribution<valueType> *d);                                                                       //!< Object constructor
        DistributionFunctionPdf(const DistributionFunctionPdf& obj);                                                                    //!< Copy constructor
        
        // overloaded operators
        DistributionFunctionPdf&                        operator=(const DistributionFunctionPdf& c);
        
        // Basic utility functions
        DistributionFunctionPdf*                        clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get language type of the object
        
        // Regular functions
        RevBayesCore::TypedFunction<double>*            createFunction(void) const;                                                     //!< Create a function object
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        
    protected:
        
        ArgumentRules                           argRules;                                                                       //!< Member rules converted to reference rules
        TypedDistribution<valueType>*           templateObject;                                                                 //!< The template object
        
    };
    
}

#include "ArgumentRule.h"
#include "DeterministicNode.h"
#include "DistributionFunctionPdf.h"
#include "Probability.h"
#include "ProbabilityDensityFunction.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "Real.h"
#include "TypeSpec.h"

#include <sstream>

/** Constructor */
template <class valueType>
RevLanguage::DistributionFunctionPdf<valueType>::DistributionFunctionPdf( TypedDistribution<valueType> *d ) : TypedFunction<Real>(),
    templateObject( d )
{
    
    argRules.push_back( new ArgumentRule("x", valueType::getClassTypeSpec(), "The observed value.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    const ArgumentRules &memberRules = templateObject->getParameterRules();
    for (std::vector<ArgumentRule*>::const_iterator it = memberRules.begin(); it != memberRules.end(); ++it)
    {
        argRules.push_back( (*it)->clone() );
    }
    argRules.push_back( new ArgumentRule("log", RlBoolean::getClassTypeSpec(), "Log-transformed probability?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true)) );
}


/** Constructor */
template <class valueType>
RevLanguage::DistributionFunctionPdf<valueType>::DistributionFunctionPdf(const DistributionFunctionPdf& obj) : TypedFunction<Real>(obj),
    argRules( obj.argRules )
{
    
    templateObject = obj.templateObject->clone();
    
}


template <class valueType>
RevLanguage::DistributionFunctionPdf<valueType>& RevLanguage::DistributionFunctionPdf<valueType>::operator=(const DistributionFunctionPdf &c) {
    
    if (this != &c) {
        Function::operator=(c);
        
        templateObject = c.templateObject->clone();
        
        argRules = c.argRules;
    }
    
    return *this;
}


/** Clone the object */
template <class valueType>
RevLanguage::DistributionFunctionPdf<valueType>* RevLanguage::DistributionFunctionPdf<valueType>::clone(void) const
{
    
    return new DistributionFunctionPdf<valueType>(*this);
}


/** Execute function: we reset our template object here and give out a copy */
template <class valueType>
RevBayesCore::TypedFunction<double>* RevLanguage::DistributionFunctionPdf<valueType>::createFunction( void ) const
{
    
    TypedDistribution<valueType>* copyObject = templateObject->clone();
    
    for ( size_t i = 1; i < (args.size()-1); i++ )
    {
        
        if ( args[i].isConstant() )
        {
            copyObject->setConstParameter( args[i].getLabel(), RevPtr<const RevVariable>( (const RevVariable*) args[i].getVariable() ) );
        }
        else
        {
            copyObject->setParameter( args[i].getLabel(), args[i].getReferenceVariable() );
        }
    }
    
    
    
    RevBayesCore::TypedDagNode<typename valueType::valueType>* arg = static_cast<const valueType &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    bool useLog = static_cast<const RlBoolean &>( this->args[args.size()-1].getVariable()->getRevObject() ).getValue();
    RevBayesCore::ProbabilityDensityFunction<typename valueType::valueType>* f = new RevBayesCore::ProbabilityDensityFunction<typename valueType::valueType>( arg, copyObject->createDistribution(), useLog );
    
    return f;
}


/** Get argument rules */
template <class valueType>
const RevLanguage::ArgumentRules& RevLanguage::DistributionFunctionPdf<valueType>::getArgumentRules(void) const {
    
    return argRules;
}


/** Get Rev type of object */
template <class valueType>
const std::string& RevLanguage::DistributionFunctionPdf<valueType>::getClassType(void) { 
    
    static std::string revType = "DistributionFunctionPdf";
    
	return revType; 
}

/** Get class type spec describing type of object */
template <class valueType>
const RevLanguage::TypeSpec& RevLanguage::DistributionFunctionPdf<valueType>::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}

/** Get type spec */
template <class valueType>
const RevLanguage::TypeSpec& RevLanguage::DistributionFunctionPdf<valueType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


#endif
