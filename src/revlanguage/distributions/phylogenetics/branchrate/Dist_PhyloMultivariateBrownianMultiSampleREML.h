#ifndef Dist_PhyloMultivariateBrownianMultiSampleREML_H
#define Dist_PhyloMultivariateBrownianMultiSampleREML_H

#include "ContinuousCharacterData.h"
#include "RlContinuousCharacterData.h"
#include "RlTypedDistribution.h"
#include "Tree.h"

namespace RevLanguage {
    
    class Dist_PhyloMultivariateBrownianMultiSampleREML : public TypedDistribution< ContinuousCharacterData > {
        
    public:
        Dist_PhyloMultivariateBrownianMultiSampleREML( void );
        virtual ~Dist_PhyloMultivariateBrownianMultiSampleREML();
        
        // Basic utility functions
        Dist_PhyloMultivariateBrownianMultiSampleREML*                                  clone(void) const;                                                                      //!< Clone the object
        static const std::string&                                                       getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                                                          getClassTypeSpec(void);                                                                 //!< Get class type spec
        std::string                                                                     getDistributionFunctionName(void) const;                                                //!< Get the Rev-name for this distribution.
        const TypeSpec&                                                                 getTypeSpec(void) const;                                                                //!< Get the type spec of the instance
        const MemberRules&                                                              getParameterRules(void) const;                                                          //!< Get member rules (const)
        void                                                                            printValue(std::ostream& o) const;                                                      //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::TypedDistribution< RevBayesCore::ContinuousCharacterData >*       createDistribution(void) const;
        
    protected:
        
        void                                                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);       //!< Set member variable
        
        
    private:
        
        RevPtr<const RevVariable>                                                       tree;
        RevPtr<const RevVariable>                                                       branch_rates;
//        RevPtr<const RevVariable>                                                       site_rates;
        RevPtr<const RevVariable>                                                       within_species_variances;
//        RevPtr<const RevVariable>                                                       n_sites;
        RevPtr<const RevVariable>                                                       rate_matrix;
        RevPtr<const RevVariable>                                                       taxa;                                                                                   //!< The taxons
        
        
    };
    
}

#endif
