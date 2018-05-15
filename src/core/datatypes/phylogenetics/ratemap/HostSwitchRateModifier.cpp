#include <iomanip>
#include <cmath>
#include "CharacterEventDiscrete.h"
#include "HostSwitchRateModifier.h"
#include "RbConstants.h"
#include "Tree.h"
#include "TreeUtilities.h"

using namespace RevBayesCore;

HostSwitchRateModifier::HostSwitchRateModifier(size_t ns, size_t nc) : CharacterHistoryRateModifier(ns, nc),
    scale( 1.0 ),
    num_branches( 0 )
{
    ;
}

HostSwitchRateModifier::HostSwitchRateModifier(const HostSwitchRateModifier& g) : CharacterHistoryRateModifier(g)
{
    
    if (&g != this)
    {
        tau = g.tau;
        scale = g.scale;
        distance = g.distance;
        num_branches = g.num_branches;
    }
}

HostSwitchRateModifier& HostSwitchRateModifier::assign(const Assignable &m)
{
    
    const HostSwitchRateModifier *crm = dynamic_cast<const HostSwitchRateModifier*>(&m);
    if ( crm != NULL )
    {
        return operator=(*crm);
    }
    else
    {
        throw RbException("Could not assign character history rate modifier.");
    }
}

double HostSwitchRateModifier::computeRateMultiplier(std::vector<CharacterEvent *> currState, CharacterEventDiscrete* newState, double age)
{
    // which character will change?
    size_t index = newState->getSiteIndex();
    
    // what is the state change?
    size_t from_state = static_cast<CharacterEventDiscrete*>(currState[index])->getState();
    size_t to_state = newState->getState();
    

    
    // losses are independent of other species
    if (from_state > to_state)
    {
        return 1.0;
    }
    
    // if the gain event level's scaling factor equals zero, then there's no effect
    if ( scale[ to_state ] == 0.0 )
    {
        return 1.0;
    }
    
    
//    double f = 1.0;
    double r = 0.0;
    size_t n = 0;
    // which characters are currently occupied?
    std::vector<std::set<size_t> > states_by_sites(this->num_characters);
    for (size_t i = 0; i < this->num_characters; i++)
    {
        size_t s = static_cast<CharacterEventDiscrete*>(currState[i])->getState();
        states_by_sites[i].insert(s);
        
        if (s != 0) {
            r += std::exp( -scale[s] * distance[s][to_state] );
            n += 1;
        }
    }
    
    // TODO: get phylogenetic distance between current lineage and new lineage at age
   
    
    // for now, we get the phylogenetic distance between current and new lineage at
    
    
    
    return r;
}

double HostSwitchRateModifier::computeSiteRateMultiplier(const TopologyNode& node, CharacterEvent* currState, CharacterEvent* newState, double age)
{
    return 1.0;
}

double HostSwitchRateModifier::computeSiteRateMultiplier(const TopologyNode& node, unsigned from, unsigned to, unsigned charIdx, double age)
{
    return 1.0;
}


HostSwitchRateModifier* HostSwitchRateModifier::clone(void) const
{
    return new HostSwitchRateModifier(*this);
}

void HostSwitchRateModifier::update(void)
{
    ; // do nothing
}

void HostSwitchRateModifier::setTree(const RevBayesCore::Tree &t)
{
    tau = t;
    num_branches = tau.getNumberOfNodes() - 1;
    distance = *RevBayesCore::TreeUtilities::getDistanceMatrix ( tau );
    
}

void HostSwitchRateModifier::setScale(const std::vector<double>& s)
{
    scale = s;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const HostSwitchRateModifier& x)
{
    o << "HostSwitchRateModifier";
    return o;
}
