#ifndef DiversityDependentPureProcess_H
#define DiversityDependentPureProcess_H

#include "AbstractBirthDeathProcess.h"

namespace RevBayesCore {
    
    class Clade;
    class Taxon;
    
    /**
     * @brief Diversity-dependent pure-birth (Yule) process.
     *
     *
     * The diversity-dependent pure-birth process is a species tree distribution without extinction events.
     * The speciation rate depends on the current diversity by the function:
     * lambda = (1.0 - n/K) * lambda_0
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-17, version 1.0
     *
     */
    class DiversityDependentPureBirthProcess : public AbstractBirthDeathProcess {
        
    public:
        DiversityDependentPureBirthProcess(const TypedDagNode<double> *o, const TypedDagNode<double> *s, const TypedDagNode<int> *k, 
                                           const std::string &cdt, const std::vector<Taxon> &tn, const std::vector<Clade> &c);                                //!< Constructor
        
        // public member functions
        DiversityDependentPureBirthProcess*                 clone(void) const;                                                                                  //!< Create an independent clone
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                            //!< Implementation of swaping parameters
        
    private:
        
        // helper functions
        double                                              computeLnProbabilityTimes(void) const;                                                                         //!< Compute the log-transformed probability of the current value.
        std::vector<double>*                                simSpeciations(size_t n, double origin) const;                                        //!< Simulate n speciation events.
        double                                              pSurvival(double start, double end) const;                                                      //!< Compute the probability of survival of the process (without incomplete taxon sampling).
        
        // members
        const TypedDagNode<double>*                         initialSpeciation;                                                                                  //!< The initial speciation rate (lambda_0).
        const TypedDagNode<int>*                            capacity;                                                                                           //!< The carrying capacity (K).
        
    };
    
}

#endif
