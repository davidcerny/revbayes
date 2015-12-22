#ifndef DiscreteEventCategoryRandomWalkProposal_H
#define DiscreteEventCategoryRandomWalkProposal_H

#include <set>
#include <string>

#include "HeterogeneousRateBirthDeath.h"
#include "Proposal.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    /**
     * The birth-death proposal for events along a tree.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team
     * @since 2009-09-08, version 1.0
     *
     */
    class DiscreteEventCategoryRandomWalkProposal : public Proposal {
        
    public:
        DiscreteEventCategoryRandomWalkProposal( StochasticNode<Tree> *n);                                                                //!<  constructor
        
        // Basic utility functions
        void                                            cleanProposal(void);                                                                //!< Clean up proposal
        DiscreteEventCategoryRandomWalkProposal*        clone(void) const;                                                                  //!< Clone object
        double                                          doProposal(void);                                                                   //!< Perform proposal
        const std::string&                              getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                                            printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                                            prepareProposal(void);                                                              //!< Prepare the proposal
        void                                            tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                            undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        
        void                                            swapNodeInternal(DagNode *oldN, DagNode *newN);                                     //!< Swap the DAG nodes on which the Proposal is working on
        
    private:
        // parameters
        
        StochasticNode<Tree>*                           variable;                                                                           //!< The variable the Proposal is working on
        HeterogeneousRateBirthDeath*                    distribution;
        
        CharacterEvent*                                 stored_value;                                                                        //!< The stored value of the Proposal used for rejections.
        size_t                                          stored_category;                                                                              //!< The value we propose.
        bool                                            failed;
    };
    
}

#endif

