#ifndef TipTimeSlideUniformProposal_H
#define TipTimeSlideUniformProposal_H

#include <string>

#include "Proposal.h"
#include "StochasticNode.h"
#include "Tree.h"

namespace RevBayesCore {
    
    /**
     * The node-age slide proposal operator using a Uniform distribution.
     *
     * This node-age proposal is a Uniform-sliding proposal on rooted subtrees without changing the topology.
     * That is, we pick a random fossil node.
     * Then, we pick an age between the parent and the present drawn from a Uniform distribution centered around the current age.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2012-07-12, version 1.0
     *
     */
    class TipTimeSlideUniformProposal : public Proposal {
        
    public:
        TipTimeSlideUniformProposal( StochasticNode<Tree> *n, TypedDagNode<double>* o, bool dyn = false );  //!<  constructor
        
        // Basic utility functions
        void                                    cleanProposal(void);                                        //!< Clean up proposal
        TipTimeSlideUniformProposal*            clone(void) const;                                          //!< Clone object
        double                                  doProposal(void);                                           //!< Perform proposal
        const std::string&                      getProposalName(void) const;                                //!< Get the name of the proposal for summary printing
        double                                  getUpdateWeight(void) const;                                //!< Get the weight of the proposal.
        void                                    prepareProposal(void);                                      //!< Prepare the proposal
        void                                    printParameterSummary(std::ostream &o) const;               //!< Print the parameter summary
        void                                    tune(double r);                                             //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                    undoProposal(void);                                         //!< Reject the proposal
        
    protected:
        
        void                                    swapNodeInternal(DagNode *oldN, DagNode *newN);             //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        
        // parameters
        StochasticNode<Tree>*                   tree;                                                   //!< The variable the Proposal is working on
        TypedDagNode<double>*                   origin;
        
        // stored objects to undo proposal
        TopologyNode*                           storedNode;
        double                                  storedAge;
        
        bool                                    dynamic;

    };
    
}

#endif

