/**
 * @file
 * This file contains the declaration of ConstantNode, which is derived
 * from DAGNode. ConstantNode is used for DAG nodes holding constant
 * values.
 *
 * @brief Declaration of ConstantNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-11-19 17:29:33 +0100 (Tor, 19 Nov 2009) $
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: ConstantNode.h 69 2009-11-19 16:29:33Z ronquist $
 */

#ifndef ConstantNode_H
#define ConstantNode_H

#include "DAGNode.h"

class RbObject;
class StringVector;

class ConstantNode : public DAGNode {

    public:
                                ConstantNode(RbObject* val);                //!< Constructor from value
                                ConstantNode(const std::string& valType);   //!< Constructor from value type
                                ConstantNode(const ConstantNode& x);        //!< Copy constructor
                                ~ConstantNode();                            //!< Destructor

        // Assignment operator
        ConstantNode&           operator=(const ConstantNode& x);           //!< Assignment operator

        // Basic utility functions
        ConstantNode*           clone(void) const;                          //!< Clone this object
        const StringVector&     getClass(void) const;                       //!< Get class vector
        void                    printStruct(std::ostream& o) const;         //!< Print struct for user
        void                    printValue(std::ostream& o) const;          //!< Print struct for user
        std::string             toString(void) const;                       //!< Complete info on object

        // ConstantNode functions
        const RbObject*         getStoredValue(void) { return value; }              //!< Get stored value
        const RbObject*         getValElement(const IntVector& index) const;        //!< Get element of value    
        const RbObject*         getValue(void) { return value; }                    //!< Get value
        const RbObject*         getValue(void) const { return value; }              //!< Get value (const)
        void                    setElement(const IntVector& index, RbObject* val);  //!< Set element of value
        void                    setValue(RbObject* val);                            //!< Set value

        // DAG functions
        ConstantNode*           cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const; //!< Clone entire graph
        bool                    isTouched (void) const { return false; }            //!< Touched by a move?
        void                    touchAffected(void);                                //!< Tell affected nodes value is reset

    protected:
        RbObject*               value;                                              //!< The constant value
};

#endif

