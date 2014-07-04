//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "BiogeographyRateMapFunction.h"
#include "DeterministicNode.h"
#include "GtrRateMatrixFunction.h"
#include "Func_dec_biogeo.h"
#include "RateMap_Biogeography.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMap.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"
#include "Vector.h"

using namespace RevLanguage;

/** default constructor */
Func_dec_biogeo::Func_dec_biogeo( void ) : Function( ) {
    
}


/** Clone object */
Func_dec_biogeo* Func_dec_biogeo::clone( void ) const {
    
    return new Func_dec_biogeo( *this );
}


RevObject* Func_dec_biogeo::execute() {
    
    RevBayesCore::TypedDagNode<std::vector<double> >* glr = static_cast<const Vector<RealPos> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    unsigned nc = static_cast<const Natural&>( this->args[1].getVariable()->getRevObject() ).getValue();
    bool fe = static_cast<const RlBoolean &>( this->args[2].getVariable()->getRevObject() ).getValue();
//    RevBayesCore::TypedDagNode<std::vector<double> >* r = static_cast<const Vector<RealPos> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::BiogeographyRateMapFunction* f = new RevBayesCore::BiogeographyRateMapFunction(nc,fe); //(nc, true);
    f->setGainLossRates(glr);
    RevBayesCore::DeterministicNode<RevBayesCore::RateMap> *detNode = new RevBayesCore::DeterministicNode<RevBayesCore::RateMap>("", f);
    
    RateMap* value = new RateMap( detNode );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& Func_dec_biogeo::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "gainLossRates", true, Vector<RealPos>::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "numCharacters", true, Natural::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "forbidExtinction", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_dec_biogeo::getClassName(void) {
    
    static std::string rbClassName = "Func_dec_biogeo";
    
	return rbClassName;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_dec_biogeo::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass;
}


/* Get return type */
const TypeSpec& Func_dec_biogeo::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMap::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_dec_biogeo::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
