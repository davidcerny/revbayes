#include "Dist_PhyloMultivariateBrownianMultiSampleREML.h"
#include "OptionRule.h"
#include "PhyloMultivariateBrownianProcessMultiSampleREML.h"
#include "RevNullObject.h"
#include "RlMatrixReal.h"
#include "RlMatrixRealSymmetric.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RlTree.h"

using namespace RevLanguage;

Dist_PhyloMultivariateBrownianMultiSampleREML::Dist_PhyloMultivariateBrownianMultiSampleREML() : TypedDistribution< ContinuousCharacterData >()
{
    
}


Dist_PhyloMultivariateBrownianMultiSampleREML::~Dist_PhyloMultivariateBrownianMultiSampleREML()
{
    
}



Dist_PhyloMultivariateBrownianMultiSampleREML* Dist_PhyloMultivariateBrownianMultiSampleREML::clone( void ) const
{
    
    return new Dist_PhyloMultivariateBrownianMultiSampleREML(*this);
}


RevBayesCore::TypedDistribution< RevBayesCore::ContinuousCharacterData >* Dist_PhyloMultivariateBrownianMultiSampleREML::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const Tree &>( tree->getRevObject() ).getDagNode();
    size_t n_nodes = tau->getValue().getNumberOfNodes();
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* var = static_cast<const ModelVector<RealPos> &>( within_species_variances->getRevObject() ).getDagNode();
    const std::vector<RevBayesCore::Taxon>      &t  = static_cast<const ModelVector<Taxon> &>( taxa->getRevObject() ).getValue();
    
    RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal>* vcv = static_cast<const MatrixRealSymmetric&>( rate_matrix->getRevObject() ).getDagNode();
    
    size_t n = vcv->getValue().getDim();
    
    RevBayesCore::PhyloMultivariateBrownianProcessMultiSampleREML *dist = new RevBayesCore::PhyloMultivariateBrownianProcessMultiSampleREML(tau, vcv, var, t, n);
    
    // set the clock rates
    if ( branch_rates->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* br = static_cast<const ModelVector<RealPos> &>( branch_rates->getRevObject() ).getDagNode();
        
        // sanity check
        size_t n_rates = br->getValue().size();
        if ( (n_nodes-1) != n_rates )
        {
            throw RbException( "The number of clock rates does not match the number of branches" );
        }
        
        dist->setBranchRate( br );
        
    }
    else
    {
        RevBayesCore::TypedDagNode< double >* br = static_cast<const RealPos &>( branch_rates->getRevObject() ).getDagNode();
        
        dist->setBranchRate( br );
    }
    
    return dist;
}



/* Get Rev type of object */
const std::string& Dist_PhyloMultivariateBrownianMultiSampleREML::getClassType(void)
{
    
    static std::string rev_type = "Dist_PhyloMultivariateBrownianMultiSampleREML";
    
    return rev_type;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_PhyloMultivariateBrownianMultiSampleREML::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_PhyloMultivariateBrownianMultiSampleREML::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "PhyloMultivariateBrownianMultiSampleREML";
    
    return d_name;
}


/** Return member rules (no members) */
const MemberRules& Dist_PhyloMultivariateBrownianMultiSampleREML::getParameterRules(void) const
{
    
    static MemberRules dist_member_rules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        dist_member_rules.push_back( new ArgumentRule( "tree" , Tree::getClassTypeSpec(), "The tree along which the process evolves.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        std::vector<TypeSpec> branchRateTypes;
        branchRateTypes.push_back( RealPos::getClassTypeSpec() );
        branchRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        dist_member_rules.push_back( new ArgumentRule( "branchRates" , branchRateTypes, "The per branch rate-multiplier(s).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        
        std::vector<TypeSpec> rateMatrixTypes;
        rateMatrixTypes.push_back( MatrixRealSymmetric::getClassTypeSpec() );
        rateMatrixTypes.push_back( MatrixReal::getClassTypeSpec() );
        dist_member_rules.push_back( new ArgumentRule( "rateMatrix", rateMatrixTypes, "The variance-covariance matrix.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        dist_member_rules.push_back( new ArgumentRule( "withinSpeciesVariances" , ModelVector<RealPos>::getClassTypeSpec(), "The per species within-species variances.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        dist_member_rules.push_back( new ArgumentRule( "taxa"  , ModelVector<Taxon>::getClassTypeSpec(), "The vector of taxa which have species and individual names.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        
        rules_set = true;
    }
    
    return dist_member_rules;
}


const TypeSpec& Dist_PhyloMultivariateBrownianMultiSampleREML::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_PhyloMultivariateBrownianMultiSampleREML::printValue(std::ostream& o) const
{
    
    o << "PhyloBrownianProcess(tree=";
    if ( tree != NULL )
    {
        o << tree->getName();
    }
    else
    {
        o << "?";
    }
    o << ", branchRates=";
    if ( branch_rates != NULL )
    {
        o << branch_rates->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
    
}


/** Set a member variable */
void Dist_PhyloMultivariateBrownianMultiSampleREML::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "branchRates" )
    {
        branch_rates = var;
    }
    else if ( name == "rateMatrix" )
    {
        rate_matrix = var;
    }
    else if ( name == "withinSpeciesVariances" )
    {
        within_species_variances = var;
    }
    else if ( name == "taxa" )
    {
        taxa = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }
    
}

