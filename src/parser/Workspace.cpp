/**
 * @file
 * This file contains the implementation of Workspace, which is
 * used to hold the global workspace and the user workspace.
 *
 * @brief Implementation of Workspace
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @extends Frame
 * @package parser
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "ConstructorFunction.h"
#include "Distribution.h"
#include "DistributionReal.h"
#include "DistributionFunction.h"
#include "Parser.h"         // For PRINTF
#include "FunctionTable.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbObject.h"
#include "Workspace.h"

// Objects added to the workspace in initializeGlobalWorkspace()
#include "Dist_exp.h"
#include "Dist_norm.h"
#include "Dist_unif.h"
#include "Func__range.h"
#include "Func_ls.h"
#include "Func_model.h"
#include "Func_quit.h"
#include "Func_source.h"
#include "Mcmc.h"
#include "Move_mscale.h"
#include "Move_mslide.h"
#include "PosReal.h"
#include "RbInt.h"
#include "RbDouble.h"

#include <sstream>
#include <vector>


/** Constructor of global workspace */
Workspace::Workspace()
    : Frame(), functionTable(new FunctionTable()) {
}


/** Constructor of user workspace */
Workspace::Workspace(Workspace* parentSpace)
    : Frame(parentSpace), functionTable(new FunctionTable(globalWorkspace().getFunctionTable())) {
}


/** Destructor */
Workspace::~Workspace() {

    delete functionTable;
}


/** Assignment operator */
Workspace& Workspace::operator=(const Workspace& x) {

    if (this != &x) {

        delete functionTable;
        functionTable = new FunctionTable(x.functionTable);
    }

    return (*this);
}


/** Add distribution to the workspace */
bool Workspace::addDistribution(const std::string& name, Distribution* dist) {

    PRINTF("Adding distribution to workspace\n");

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, StringVector>(name, dist->getClass()));

    functionTable->addFunction(name, new ConstructorFunction(dist));
    functionTable->addFunction("d" + name, new DistributionFunction(dist, DistributionFunction::DENSITY));
    functionTable->addFunction("r" + name, new DistributionFunction((Distribution*)(dist->clone()), DistributionFunction::RVALUE));

    return true;
}


/** Add real-valued distribution to the workspace */
bool Workspace::addDistribution(const std::string& name, DistributionReal* dist) {

    PRINTF("Adding real-valued distribution %s to workspace\n", name.c_str());

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, StringVector>(name, dist->getClass()));

    functionTable->addFunction(name, new ConstructorFunction(dist));
    functionTable->addFunction("d" + name, new DistributionFunction(dist, DistributionFunction::DENSITY));
    functionTable->addFunction("r" + name, new DistributionFunction((Distribution*)(dist->clone()), DistributionFunction::RVALUE));
    functionTable->addFunction("p" + name, new DistributionFunction((Distribution*)(dist->clone()), DistributionFunction::PROB));
    functionTable->addFunction("q" + name, new DistributionFunction((Distribution*)(dist->clone()), DistributionFunction::QUANTILE));

    return true;
}


/** Add function to the workspace */
bool Workspace::addFunction(const std::string& name, RbFunction* func) {

    PRINTF("Adding function %s to workspace\n", name.c_str());
    
    if (existsVariable(name))
        throw RbException("There is already a variable named '" + name + "' in the workspace");

    functionTable->addFunction(name, func);
    
    return true;
}


/** Add type to the workspace */
bool Workspace::addType(const RbObject* exampleObj) {

    PRINTF("Adding type %s to workspace\n", exampleObj->getType().c_str());

    std::string name = exampleObj->getType();

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, StringVector>(exampleObj->getType(), exampleObj->getClass()));

    delete exampleObj;

    return true;
}


/** Add type with constructor to the workspace */
bool Workspace::addTypeWithConstructor(const std::string& name, MemberObject* templ) {

    PRINTF("Adding type %s with constructor to workspace\n", name.c_str());

    if (typeTable.find(name) != typeTable.end())
        throw RbException("There is already a type named '" + name + "' in the workspace");

    typeTable.insert(std::pair<std::string, StringVector>(name, templ->getClass()));

    functionTable->addFunction(name, new ConstructorFunction(templ));

    return true;
}


/** Execute function */
const RbObject* Workspace::executeFunction(const std::string& name, const std::vector<Argument>& args) const {

    return functionTable->executeFunction(name, args);
}


/** Get function */
RbFunction* Workspace::getFunction(const std::string& name, const std::vector<Argument>& args) {

    return functionTable->getFunction(name, args);
}


/** Get function value */
RbObject* Workspace::getFunctionValue(const std::string& name, const std::vector<Argument>& args) const {

    return functionTable->getFunctionValue(name, args);
}


/** Convenient access to default random number generator */
RandomNumberGenerator* Workspace::get_rng(void) {

    return (RandomNumberGenerator*) getValue("_rng");
}


/** Initialize global workspace */
void Workspace::initializeGlobalWorkspace(void) {

    /* Add global variables */
    std::vector<unsigned int> seeds;
    seeds.push_back(1);
    seeds.push_back(1337);
    addVariable("_rng", new RandomNumberGenerator(seeds));

    /* Add types */
    addType(new RbInt(0));
    addType(new RbDouble());
    addType(new PosReal(1.0));
    addType(new RandomNumberGenerator(seeds));

    /* Add member object types with auto-generated constructors */
    addTypeWithConstructor("mcmc", new Mcmc());
    addTypeWithConstructor("mslide", new Move_mslide());
    addTypeWithConstructor("mscale", new Move_mscale());

    /* Add distributions with distribution constructors and distribution functions*/
    addDistribution("exp",  new Dist_exp());
    addDistribution("norm", new Dist_norm());
    addDistribution("unif", new Dist_unif());

    /* Add basic internal functions */
    addFunction("_range", new Func__range());

    /* Add regular functions */
    addFunction("ls",     new Func_ls());
    addFunction("model",  new Func_model());
    addFunction("q",      new Func_quit());
    addFunction("quit",   new Func_quit());
    addFunction("source", new Func_source());
}


/** Type checking using type table */
bool Workspace::isXOfTypeY(const std::string& x, const std::string& y) const {

    /* Simplest case first */
    if (x == y)
        return true;

    if (typeTable.find(x) == typeTable.end()) {
        if (parentFrame == NULL)
            throw RbException("Unknown type named '" + x + "'");
        else
            return ((Workspace*)(parentFrame))->isXOfTypeY(x, y);
    }

    const StringVector& xVec = (*typeTable.find(x)).second;
    size_t i;
    for (i=0; i<xVec.size(); i++) {
        if (xVec[i] == y)
            break;
    }

    if (i == xVec.size())
        return false;
    else
        return true;
}


/** Print workspace */
void Workspace::printValue(std::ostream& o) const {

    o << "Variable table:" << std::endl;
    Frame::printValue(o);
    o << std::endl;

    o << "Function table:" << std::endl;
    functionTable->printValue(o);
    o << std::endl;

    o << "Type table:" << std::endl;
    std::map<std::string, StringVector>::const_iterator i;
    for (i=typeTable.begin(); i!=typeTable.end(); i++) {
        o << (*i).first << " = " << (*i).second << std::endl;
    }
}


