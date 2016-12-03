#include "Dist_phyloDolloCTMC.h"

#include "PhyloCTMCSiteHomogeneousConditional.h"
#include "PhyloCTMCSiteHomogeneousDollo.h"
#include "OptionRule.h"
#include "Probability.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlRateGenerator.h"
#include "RlString.h"
#include "RlTree.h"
#include "RlSimplex.h"

using namespace RevLanguage;

Dist_phyloDolloCTMC::Dist_phyloDolloCTMC() : TypedDistribution< AbstractHomologousDiscreteCharacterData >()
{

}


Dist_phyloDolloCTMC::~Dist_phyloDolloCTMC()
{

}



Dist_phyloDolloCTMC* Dist_phyloDolloCTMC::clone( void ) const
{

    return new Dist_phyloDolloCTMC(*this);
}


RevBayesCore::TypedDistribution< RevBayesCore::AbstractHomologousDiscreteCharacterData >* Dist_phyloDolloCTMC::createDistribution( void ) const
{

    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const Tree &>( tree->getRevObject() ).getDagNode();
    size_t n = size_t( static_cast<const Natural &>( nSites->getRevObject() ).getValue() );
    bool norm = static_cast<const RlBoolean &>( normalize->getRevObject() ).getValue();
    size_t nNodes = tau->getValue().getNumberOfNodes();
    const std::string& code = static_cast<const RlString &>( coding->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<double>* deathRateNode = static_cast<const RealPos &>( deathRate->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* siteRatesNode = NULL;
    if ( siteRates != NULL && siteRates->getRevObject() != RevNullObject::getInstance() )
    {
        siteRatesNode = static_cast<const ModelVector<RealPos> &>( siteRates->getRevObject() ).getDagNode();
    }

    RevBayesCore::TypedDistribution< RevBayesCore::AbstractHomologousDiscreteCharacterData > *d = NULL;
    const RevBayesCore::TypedDagNode< RevBayesCore::RbVector< double > > *rf = NULL;
    if ( rootFrequencies != NULL && rootFrequencies->getRevObject() != RevNullObject::getInstance() )
    {
        rf = static_cast<const Simplex &>( rootFrequencies->getRevObject() ).getDagNode();
    }

    // we get the number of states from the rates matrix
    // set the rate matrix
    size_t nChars = 1;
    if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();
        nChars = rm->getValue()[0].getNumberOfStates();
    }
    else if ( q->getRevObject() != RevNullObject::getInstance() )
    {
        RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
        nChars = rm->getValue().getNumberOfStates();
    }

    // split the coding option on "|"
    std::string s = code;
    std::vector<std::string> tokens;

    size_t pos = 0;
    while ((pos = s.find("|")) != std::string::npos) {
        tokens.push_back(s.substr(0, pos));
        s.erase(0, pos + 1);
    }
    tokens.push_back(s);

    // set the flags for each token
    int cd = RevBayesCore::RestrictionAscertainmentBias::NOABSENCESITES;
    for(size_t i = 0; i < tokens.size(); i++)
    {
        if(tokens[i] == "informative")
        {
            cd = RevBayesCore::RestrictionAscertainmentBias::INFORMATIVE;
        }
        else if(tokens[i] == "variable")
        {
            cd = RevBayesCore::RestrictionAscertainmentBias::VARIABLE;
        }
        else if(tokens[i] != "noabsencesites")
        {
            std::stringstream ss;
            ss << "Unrecognized coding option \"" << tokens[i] << "\" for Dollo model\n";
            ss << "\tAvailable codings: noabsencesites, informative, variable\n";
            throw RbException(ss.str());
        }
    }

    RevBayesCore::PhyloCTMCSiteHomogeneousDollo *dist = new RevBayesCore::PhyloCTMCSiteHomogeneousDollo(tau, nChars + 1, true, n, true, RevBayesCore::DolloAscertainmentBias::Coding(cd), norm);

    // set the root frequencies (by default these are NULL so this is OK)
    dist->setRootFrequencies( rf );

    if ( rate->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( rate->getRevObject() ).getDagNode();

        // sanity check
        if ( nNodes != clockRates->getValue().size() )
        {
            throw RbException( "The number of clock rates does not match the number of nodes" );
        }

        dist->setClockRate( clockRates );
    }
    else
    {
        RevBayesCore::TypedDagNode<double>* clockRate = static_cast<const RealPos &>( rate->getRevObject() ).getDagNode();
        dist->setClockRate( clockRate );
    }

    // set the rate matrix
    if ( q->getRevObject().isType( ModelVector<RateGenerator>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( q->getRevObject() ).getDagNode();

        // sanity check
        if ( (nNodes-1) != rm->getValue().size() )
        {
            throw RbException( "The number of substitution matrices does not match the number of branches" );
        }
        // sanity check
        if ( rootFrequencies == NULL || rootFrequencies->getRevObject() == RevNullObject::getInstance() )
        {
            throw RbException( "If you provide branch-heterogeneous substitution matrices, then you also need to provide root frequencies." );
        }


        dist->setRateMatrix( rm );
    }
    else if ( q->getRevObject() != RevNullObject::getInstance() )
    {
        RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator &>( q->getRevObject() ).getDagNode();
        dist->setRateMatrix( rm );
    }
    else
    {

    }

    if ( siteRatesNode != NULL && siteRatesNode->getValue().size() > 0 )
    {
        dist->setSiteRates( siteRatesNode );
    }

    dist->setDeathRate(deathRateNode);

    d = dist;


    return d;
}



/* Get Rev type of object */
const std::string& Dist_phyloDolloCTMC::getClassType(void)
{

    static std::string revType = "Dist_phyloDolloCTMC";

    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_phyloDolloCTMC::getClassTypeSpec(void)
{

    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );

    return revTypeSpec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_phyloDolloCTMC::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "PhyloDolloCTMC";
    
    return d_name;
}


/** Return member rules (no members) */
const MemberRules& Dist_phyloDolloCTMC::getParameterRules(void) const
{

    static MemberRules distMemberRules;
    static bool rulesSet = false;

    if ( !rulesSet )
    {
        distMemberRules.push_back( new ArgumentRule( "tree", Tree::getClassTypeSpec(), "The tree along which the process evolves.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

        std::vector<TypeSpec> rateMatrixTypes;
        rateMatrixTypes.push_back( RateGenerator::getClassTypeSpec() );
        rateMatrixTypes.push_back( ModelVector<RateGenerator>::getClassTypeSpec() );
        distMemberRules.push_back( new ArgumentRule( "Q", rateMatrixTypes, "The global or branch-specific rate matrices.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY , NULL ) );

        distMemberRules.push_back( new ArgumentRule( "deathRate", RealPos::getClassTypeSpec(), "The Dollo character death rate", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );

        // optional argument for the root frequencies
        distMemberRules.push_back( new ArgumentRule( "rootFrequencies", Simplex::getClassTypeSpec(), "The root specific frequencies of the characters, if applicable.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );

        std::vector<TypeSpec> branchRateTypes;
        branchRateTypes.push_back( RealPos::getClassTypeSpec() );
        branchRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        distMemberRules.push_back( new ArgumentRule( "branchRates", branchRateTypes, "The global or branch-specific rate multipliers.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );

        ModelVector<RealPos> *defaultSiteRates = new ModelVector<RealPos>();
        distMemberRules.push_back( new ArgumentRule( "siteRates", ModelVector<RealPos>::getClassTypeSpec(), "The rate categories for the sites.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, defaultSiteRates ) );

        distMemberRules.push_back( new ArgumentRule( "nSites", Natural::getClassTypeSpec(), "The number of sites, used for simulation.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(10) ) );

        distMemberRules.push_back( new ArgumentRule( "normalize", RlBoolean::getClassTypeSpec(), "Should we normalize the Dollo substitution matrices?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean( true ) ) );

        distMemberRules.push_back( new ArgumentRule("coding", RlString::getClassTypeSpec(), "", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("noabsencesites") ) );

        rulesSet = true;
    }

    return distMemberRules;
}


const TypeSpec& Dist_phyloDolloCTMC::getTypeSpec( void ) const
{

    static TypeSpec ts = getClassTypeSpec();

    return ts;
}


/** Print value for user */
void Dist_phyloDolloCTMC::printValue(std::ostream& o) const
{

    o << "Dollo-Character-State-Evolution-Along-Tree Process(tree=";
    if ( tree != NULL ) {
        o << tree->getName();
    } else {
        o << "?";
    }
    o << ", Q=";
    if ( q != NULL ) {
        o << q->getName();
    } else {
        o << "?";
    }
    o << ", deathRate=";
    if ( deathRate != NULL ) {
        o << deathRate->getName();
    } else {
        o << "?";
    }
    o << ", branchRates=";
    if ( rate != NULL ) {
        o << rate->getName();
    } else {
        o << "?";
    }
    o << ", siteRates=";
    if ( siteRates != NULL ) {
        o << siteRates->getName();
    } else {
        o << "?";
    }
    o << ", nSites=";
    if ( nSites != NULL ) {
        o << nSites->getName();
    } else {
        o << "?";
    }
    o << ", coding=";
    if ( coding != NULL ) {
        o << coding->getName();
    } else {
        o << "?";
    }
    o << ", normalize=";
    if ( normalize != NULL ) {
        o << normalize->getName();
    } else {
        o << "?";
    }
    o << ")";

}


/** Set a member variable */
void Dist_phyloDolloCTMC::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{

    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "Q" )
    {
        q = var;
    }
    else if ( name == "deathRate" )
    {
        deathRate = var;
    }
    else if ( name == "rootFrequencies" )
    {
        rootFrequencies = var;
    }
    else if ( name == "branchRates" )
    {
        rate = var;
    }
    else if ( name == "siteRates" )
    {
        siteRates = var;
    }
    else if ( name == "nSites" )
    {
        nSites = var;
    }
    else if ( name == "normalize" )
    {
        normalize = var;
    }
    else if ( name == "coding" )
    {
        coding = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }

}
