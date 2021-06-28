#include <iostream>
#include <fstream>
#include <cppyabm/bases.h>
#include "cppyabm/mesh.h"
#include <random>

struct CovidModel;
struct CovidAgent;
double random(double min, double max){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min,max);
    return dis(gen);
};
double poisson(double mean){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::poisson_distribution<int> dis(mean;
    return dis(gen);
};
enum Stage{
    SUSCEPTIBLE = 1,
    EXPOSED = 2,
    ASYMPTOMATIC = 3,
    SYMPDETECTED = 4,
    ASYMPDETECTED = 5,
    SEVERE = 6,
    RECOVERED = 7,
    DECEASED = 8,
};



enum AgeGroup{
    C00to09 = 0,
    C10to19 = 1,
    C20to29 = 2,
    C30to39 = 3,
    C40to49 = 4,
    C50to59 = 5,
    C60to69 = 6,
    C70to79 = 7,
    C80toXX = 8
};



enum SexGroup{
    MALE = 1,
    FEMALE = 2
};



enum ValueGroup{
    PRIVATE = 1,
    PUBLIC = 2
};



struct CovidAgent: public Agent<CovidModel,CovidAgent,Patch>{
    using baseAgent = Agent<CovidModel, CovidAgent, Patch>;
    using baseAgent::baseAgent;
    CovidAgent(shared_ptr<CovidModel> model, string name, float mort):baseAgent(model, name){
        this->stage = Stage.SUSCEPTIBLE;
        //        this->age_group = ageg;
        //        this->sex_group = sexg;
        // These are fixed values associated with properties of individuals
        this->incubation_time = poisson(model->avg_incubation);
        this->dwelling_time = poisson(model->avg_dwell);
        this->recovery_time = poisson(model->avg_recovery);
        this->prob_contagion = this->env->prob_contagion_base;
        // Mortality in vulnerable population appears to be around day 2-3
        this->mortality_value = mort / (this->env->dwell_15_day * this->recovery_time);
        // Severity appears to appear after day 5
        this->severity_value = model->prob_severe / (this->env->dwell_15_day * this->recovery_time);
        this->curr_dwelling = 0;
        this->curr_incubation = 0;
        this->curr_recovery = 0;
        this->curr_asymptomatic = 0;
        // Isolation measures are set at the model step level
        this->isolated = false;
        this->isolated_but_inefficient = false;
        // Contagion probability is local
        this->test_chance = 0;
        // Horrible hack for isolation step
        this->in_isolation = false;
        this->in_distancing = false;
        this->in_testing = false;
        this->astep = 0;
        this->tested = false;
        // Economic assumptions
        this->cumul_private_value = 0;
        this->cumul_public_value = 0;
        // Employment
        this->employed = true;
        // Contact tracing: this is only available for symptomatic patients
        this->tested_traced = false;
        // All agents
        this->contacts = set();
        // We assume it takes two full days
        this->tracing_delay = 2 * model->dwell_15_day;
        this->tracing_counter = 0;
    }
    this->
    stage = Stage.SUSCEPTIBLE;
    //        this->age_group = ageg;
    //        this->sex_group = sexg;
    // These are fixed values associated with properties of individuals
    this->
    incubation_time = poisson(model->avg_incubation);
    this->
    dwelling_time = poisson(model->avg_dwell);
    this->
    recovery_time = poisson(model->avg_recovery);
    this->
    prob_contagion = this->env->prob_contagion_base;
    // Mortality in vulnerable population appears to be around day 2-3
    this->
    mortality_value = mort / (this->env->dwell_15_day * this->recovery_time);
    // Severity appears to appear after day 5
    this->
    severity_value = model->prob_severe / (this->env->dwell_15_day * this->recovery_time);
    this->
    curr_dwelling = 0;
    this->
    curr_incubation = 0;
    this->
    curr_recovery = 0;
    this->
    curr_asymptomatic = 0;
    // Isolation measures are set at the model step level
    this->
    isolated = false;
    this->
    isolated_but_inefficient = false;
    // Contagion probability is local
    this->
    test_chance = 0;
    // Horrible hack for isolation step
    this->
    in_isolation = false;
    this->
    in_distancing = false;
    this->
    in_testing = false;
    this->
    astep = 0;
    this->
    tested = false;
    // Economic assumptions
    this->
    cumul_private_value = 0;
    this->
    cumul_public_value = 0;
    // Employment
    this->
    employed = true;
    // Contact tracing: this is only available for symptomatic patients
    this->
    tested_traced = false;
    // All agents
    this->
    contacts = set();
    // We assume it takes two full days
    this->
    tracing_delay = 2 * model->dwell_15_day;
    this->
    tracing_counter = 0;
}
/**
void alive(self):
print(f'{this->unique_id} {this->age_group} {this->sex_group} is alive')

def is_contagious(self):
return (this->stage == Stage.EXPOSED) or (this->stage == Stage.ASYMPTOMATIC)

def dmult(self):
// In this function, we simulate aerosol effects exhibited by droplets due to
// both the contributions of a) a minimum distance with certainty of infection
// and a the decreasing bioavailability of droplets, modeled as a sigmoid function.
// Units are in meters. We assume that after 1.5 meter bioavailability decreases as a
// sigmoid. This case supposses infrequent sneezing, but usual saliva droplets when
// masks are not in use. A multiplier of k = 10 is used as a sharpening parameter
// of the distribution and must be further callibrated.
mult = 1.0

if this->env.distancing >= 1.5:
k = 10
mult = 1.0 - (1.0 / (1.0 + np.exp(k*(-(this->env.distancing - 1.5) + 0.5))))

return mult

// In this function, we count effective interactants
        def interactants(self):
count = 0

if (this->stage != Stage.DECEASED) and (this->stage != Stage.RECOVERED):
for agent in this->get_patch().get_agents():
if self != agent: //TODO: check this out
if not(agent.isolated) or this->isolated_but_inefficient:
count = count + 1

return count

// A function that applies a contact tracing test
        def test_contact_trace(self):
// We may have an already tested but it had a posterior contact and became infected
if this->stage == Stage.SUSCEPTIBLE:
this->tested_traced = true
elif this->stage == Stage.EXPOSED:
this->tested_traced = true

if bernoulli.rvs(this->env.prob_asymptomatic):
this->stage = Stage.ASYMPDETECTED
else:
this->stage = Stage.SYMPDETECTED
elif this->stage == Stage.ASYMPTOMATIC:
this->stage = Stage.ASYMPDETECTED
this->tested_traced = true
else:
return

def add_contact_trace(self, other):
if this->env.tracing_now:
this->contacts.add(other)

def step(self):
// We compute unemployment in general as a probability of 0.00018 per day.
// In 60 days, this is equivalent to a probability of 1% unemployment filings.
if this->employed:
if this->isolated:
if bernoulli.rvs(32*0.00018/this->env.dwell_15_day):
this->employed = false
else:
if bernoulli.rvs(8*0.00018/this->env.dwell_15_day):
this->employed = false

// We also compute the probability of re-employment, which is at least ten times
// as smaller in a crisis.
if not(this->employed):
if bernoulli.rvs(0.000018/this->env.dwell_15_day):
this->employed = true

// Social distancing
if not(this->in_distancing) and (this->astep >= this->env.distancing_start):
this->prob_contagion = this->dmult() * this->env.prob_contagion_base
this->in_distancing = true

if this->in_distancing and (this->astep >= this->env.distancing_end):
this->prob_contagion = this->env.prob_contagion_base
this->in_distancing = false

// Testing
if not(this->in_testing) and (this->astep >= this->env.testing_start):
this->test_chance = this->env.testing_rate
this->in_testing = true

if this->in_testing and (this->astep >= this->env.testing_end):
this->test_chance = 0
this->in_testing = false

// Self isolation is tricker. We only isolate susceptibles, incubating and asymptomatics
if not(this->in_isolation):
if (this->astep >= this->env.isolation_start):
if (this->stage == Stage.SUSCEPTIBLE) or (this->stage == Stage.EXPOSED) or \
                    (this->stage == Stage.ASYMPTOMATIC):
if bool(bernoulli.rvs(this->env.isolation_rate)):
this->isolated = true
else:
this->isolated = false
this->in_isolation = true
elif (this->astep >= this->env.isolation_end):
if (this->stage == Stage.SUSCEPTIBLE) or (this->stage == Stage.EXPOSED) or \
                    (this->stage == Stage.ASYMPTOMATIC):
if bool(bernoulli.rvs(this->env.after_isolation)):
this->isolated = true
else:
this->isolated = false
this->in_isolation = true


// Using a similar logic, we remove isolation for all relevant agents still locked
if this->in_isolation and (this->astep >= this->env.isolation_end):
if (this->stage == Stage.SUSCEPTIBLE) or (this->stage == Stage.EXPOSED) or \
                (this->stage == Stage.ASYMPTOMATIC):
this->isolated = false
this->in_isolation = false

// Using the model, determine if a susceptible individual becomes infected due to
// being elsewhere and returning to the community
if this->stage == Stage.SUSCEPTIBLE:
if bernoulli.rvs(this->env.rate_inbound):
this->stage = Stage.EXPOSED

if this->stage == Stage.SUSCEPTIBLE:
// Important: infected people drive the spread, not
// the number of healthy ones

// If testing is available and the date is reached, test
// Testing of a healthy person should maintain them as
// still susceptible.
// We take care of testing probability at the top level step
// routine to avoid this repeated computation
if not(this->tested or this->tested_traced) and bernoulli.rvs(this->test_chance):
this->tested = true
// First opportunity to get infected: contact with others
// in near proximity
cellmates = this->get_patch().get_agents()
infected_contact = false

// Isolated people should only be contagious if they do not follow proper
// shelter-at-home measures
for c in cellmates:
if c.is_contagious():
        c.add_contact_trace(self)
if this->isolated and bernoulli.rvs(1 - this->env.prob_isolation_effective):
this->isolated_but_inefficient = true
infected_contact = true
break
else:
infected_contact = true
break

// Value is computed before infected stage happens
isolation_private_divider = 1
isolation_public_divider = 1

if this->employed:
if this->isolated:
isolation_private_divider = 0.3
isolation_public_divider = 0.01


this->cumul_private_value = this->cumul_private_value + \
                    ((len(cellmates) - 1) * this->env.stage_value_dist[ValueGroup.PRIVATE][Stage.SUSCEPTIBLE])*isolation_private_divider
this->cumul_public_value = this->cumul_public_value + \
                    ((len(cellmates) - 1) * this->env.stage_value_dist[ValueGroup.PUBLIC][Stage.SUSCEPTIBLE])*isolation_public_divider
else:
this->cumul_private_value = this->cumul_private_value + 0
this->cumul_public_value = this->cumul_public_value - 2*this->env.stage_value_dist[ValueGroup.PUBLIC][Stage.SUSCEPTIBLE]

if infected_contact:
if this->isolated:
if bernoulli.rvs(this->prob_contagion) and \
                        not(bernoulli.rvs(this->env.prob_isolation_effective)):
this->stage = Stage.EXPOSED
else:
if bernoulli.rvs(this->prob_contagion):
this->stage = Stage.EXPOSED

// Second opportunity to get infected: residual droplets in places
// TODO

if not(this->isolated):
this->displace()
elif this->stage == Stage.EXPOSED:
// Susceptible patients only move and spread the disease.
// If the incubation time is reached, it is immediately
// considered as detected since it is severe enough.

// We compute the private value as usual
cellmates = this->get_patch().get_agents()

isolation_private_divider = 1
isolation_public_divider = 1

if this->employed:
if this->isolated:
isolation_private_divider = 0.3
isolation_public_divider = 0.01

this->cumul_private_value = this->cumul_private_value + \
                    ((len(cellmates) - 1) * this->env.stage_value_dist[ValueGroup.PRIVATE][Stage.EXPOSED])*isolation_private_divider
this->cumul_public_value = this->cumul_public_value + \
                    ((len(cellmates) - 1) * this->env.stage_value_dist[ValueGroup.PUBLIC][Stage.EXPOSED])*isolation_public_divider
else:
this->cumul_private_value = this->cumul_private_value + 0
this->cumul_public_value = this->cumul_public_value - 2*this->env.stage_value_dist[ValueGroup.PUBLIC][Stage.EXPOSED]

// Assignment is less expensive than comparison
do_move = true

// If testing is available and the date is reached, test
if not(this->tested or this->tested_traced) and bernoulli.rvs(this->test_chance):
if bernoulli.rvs(this->env.prob_asymptomatic):
this->stage = Stage.ASYMPDETECTED
else:
this->stage = Stage.SYMPDETECTED
do_move = false

this->tested = true
else:
if this->curr_incubation < this->incubation_time:
this->curr_incubation = this->curr_incubation + 1
else:
if bernoulli.rvs(this->env.prob_asymptomatic):
this->stage = Stage.ASYMPTOMATIC
else:
this->stage = Stage.SYMPDETECTED
do_move = false

// Now, attempt to move
if do_move and not(this->isolated):
this->displace()

// Perform the move once the condition has been determined
elif this->stage == Stage.ASYMPTOMATIC:
// Asymptomayic patients only roam around, spreading the
// disease, ASYMPDETECTEDimmune system
cellmates = this->get_patch().get_agents()

isolation_private_divider = 1
isolation_public_divider = 1

if this->employed:
if this->isolated:
isolation_private_divider = 0.3
isolation_public_divider = 0.01

this->cumul_private_value = this->cumul_private_value + \
                        ((len(cellmates) - 1) * this->env.stage_value_dist[ValueGroup.PRIVATE][Stage.ASYMPTOMATIC])*isolation_private_divider
this->cumul_public_value = this->cumul_public_value + \
                        ((len(cellmates) - 1) * this->env.stage_value_dist[ValueGroup.PUBLIC][Stage.ASYMPTOMATIC])*isolation_public_divider
else:
this->cumul_private_value = this->cumul_private_value + 0
this->cumul_public_value = this->cumul_public_value - 2*this->env.stage_value_dist[ValueGroup.PUBLIC][Stage.ASYMPTOMATIC]

if not(this->tested or this->tested_traced) and bernoulli.rvs(this->test_chance):
this->stage = Stage.ASYMPDETECTED
this->tested = true
else:
if this->curr_recovery >= this->recovery_time:
this->stage = Stage.RECOVERED

if not(this->isolated):
this->displace()

elif this->stage == Stage.SYMPDETECTED:
// Once a symptomatic patient has been detected, it does not move and starts
// the road to severity, recovery or death. We assume that, by reaching a health
// unit, they are tested as positive.
this->isolated = true
this->tested = true

// Contact tracing logic: use a negative number to indicate trace exhaustion
if this->env.tracing_now and this->tracing_counter >= 0:
// Test only when the count down has been reached
if this->tracing_counter == this->tracing_delay:
for t in this->contacts:
t.test_contact_trace()

this->tracing_counter = -1
else:
this->tracing_counter = this->tracing_counter + 1

this->cumul_private_value = this->cumul_private_value + \
                this->env.stage_value_dist[ValueGroup.PRIVATE][Stage.SYMPDETECTED]
this->cumul_public_value = this->cumul_public_value + \
                this->env.stage_value_dist[ValueGroup.PUBLIC][Stage.SYMPDETECTED]

if this->curr_incubation + this->curr_recovery < this->incubation_time + this->recovery_time:
this->curr_recovery = this->curr_recovery + 1

if bernoulli.rvs(this->severity_value):
this->stage = Stage.SEVERE
else:
this->stage = Stage.RECOVERED
elif this->stage == Stage.ASYMPDETECTED:
this->isolated = true

// Contact tracing logic: use a negative number to indicate trace exhaustion
if this->env.tracing_now and this->tracing_counter >= 0:
// Test only when the count down has been reached
if this->tracing_counter == this->tracing_delay:
for t in this->contacts:
t.test_contact_trace()

this->tracing_counter = -1
else:
this->tracing_counter = this->tracing_counter + 1

this->cumul_private_value = this->cumul_private_value + \
                this->env.stage_value_dist[ValueGroup.PRIVATE][Stage.ASYMPDETECTED]
this->cumul_public_value = this->cumul_public_value + \
                this->env.stage_value_dist[ValueGroup.PUBLIC][Stage.ASYMPDETECTED]

// The road of an asymptomatic patients is similar without the prospect of death
if this->curr_incubation + this->curr_recovery < this->incubation_time + this->recovery_time:
this->curr_recovery = this->curr_recovery + 1
else:
this->stage = Stage.RECOVERED
elif this->stage == Stage.SEVERE:
this->cumul_private_value = this->cumul_private_value + \
                this->env.stage_value_dist[ValueGroup.PRIVATE][Stage.SEVERE]
this->cumul_public_value = this->cumul_public_value + \
                this->env.stage_value_dist[ValueGroup.PUBLIC][Stage.SEVERE]

// Severe patients are in ICU facilities
if this->curr_recovery < this->recovery_time:
// Not recovered yet, may pass away depending on prob.
if bernoulli.rvs(this->mortality_value):
this->stage = Stage.DECEASED
// If hospital beds are saturated, mortality jumps by a factor of 5x
//elif this->env.max_beds_available < compute_severe_n(this->env):
//    if bernoulli.rvs(1.2*this->mortality_value):
//        this->stage = Stage.DECEASED
                else:
                    this->curr_recovery = this->curr_recovery + 1
            else:
                this->stage = Stage.RECOVERED
        elif this->stage == Stage.RECOVERED:
            cellmates = this->get_patch().get_agents()
            
            if this->employed:
                isolation_private_divider = 1
                isolation_public_divider = 1

                if this->isolated:
                    isolation_private_divider = 0.3
                    isolation_public_divider = 0.01

                this->cumul_private_value = this->cumul_private_value + \
                    ((len(cellmates) - 1) * this->env.stage_value_dist[ValueGroup.PRIVATE][Stage.RECOVERED])*isolation_private_divider
                this->cumul_public_value = this->cumul_public_value + \
                    ((len(cellmates) - 1) * this->env.stage_value_dist[ValueGroup.PUBLIC][Stage.RECOVERED])*isolation_public_divider
            else:
                this->cumul_private_value = this->cumul_private_value + 0
                this->cumul_public_value = this->cumul_public_value - 2*this->env.stage_value_dist[ValueGroup.PUBLIC][Stage.RECOVERED]

            // A recovered agent can now move freely within the grid again
            this->curr_recovery = 0
            this->isolated = false
            this->isolated_but_inefficient = false
            this->displace()
        elif this->stage == Stage.DECEASED:
            this->cumul_private_value = this->cumul_private_value + \
                this->env.stage_value_dist[ValueGroup.PRIVATE][Stage.DECEASED]
            this->cumul_public_value = this->cumul_public_value + \
                this->env.stage_value_dist[ValueGroup.PUBLIC][Stage.DECEASED]
        else:
            // If we are here, there is a problem
            sys.exit("Unknown stage: aborting.")

        this->astep = this->astep + 1

    def displace(self):
        // If dwelling has not been exhausted, do not move
        if this->curr_dwelling > 0:
            this->curr_dwelling = this->curr_dwelling - 1

        // If dwelling has been exhausted, move and replenish the dwell
        else:
            possible_steps = this->get_patch().neighbors
            new_position = random.choice(possible_steps)

            this->move(new_position,true)
            this->curr_dwelling = poisson(this->env.avg_dwell)

};
""" An agent representing a potential covid case"""



def compute_susceptible(model):
    return count_type(model, Stage.SUSCEPTIBLE)/model.num_agents

def compute_incubating(model):
    return count_type(model, Stage.EXPOSED)/model.num_agents

def compute_asymptomatic(model):
    return count_type(model, Stage.ASYMPTOMATIC)/model.num_agents

def compute_symptdetected(model):
    return count_type(model, Stage.SYMPDETECTED)/model.num_agents

def compute_severe_n(model):
    return count_type(model, Stage.SEVERE)

def compute_asymptdetected(model):
    return count_type(model, Stage.ASYMPDETECTED)/model.num_agents

def compute_severe(model):
    return count_type(model, Stage.SEVERE)/model.num_agents

def compute_recovered(model):
    return count_type(model, Stage.RECOVERED)/model.num_agents

def compute_deceased(model):
    return count_type(model, Stage.DECEASED)/model.num_agents

def count_type(model, stage):
    count = 0

    for agent in model.agents:
        if agent.stage == stage:
            count = count + 1

    return count

def compute_isolated(model):
    count = 0

    for agent in model.agents:
        if agent.isolated:
            count = count + 1

    return count/model.num_agents

def compute_employed(model):
    count = 0

    for agent in model.agents:
        if agent.employed:
            count = count + 1

    return count/model.num_agents

def compute_unemployed(model):
    count = 0

    for agent in model.agents:
        if not(agent.employed):
            count = count + 1

    return count/model.num_agents

def compute_contacts(model):
    count = 0

    for agent in model.agents:
        count = count + agent.interactants()

    return count/len(model.agents)

def compute_stepno(model):
    return model.stepno

def compute_cumul_private_value(model):
    value = 0

    for agent in model.agents:
        value = value + agent.cumul_private_value

    return np.sign(value)*np.power(np.abs(value), model.alpha_private)/model.num_agents

def compute_cumul_public_value(model):
    value = 0

    for agent in model.agents:
        value = value + agent.cumul_public_value

    return np.sign(value)*np.power(np.abs(value), model.alpha_public)/model.num_agents

def compute_cumul_testing_cost(model):
    tested = 0

    for agent in model.agents:
        if agent.tested:
            tested = tested + 1

    return tested * model.test_cost/model.num_agents

def compute_tested(model):
    tested = 0

    for agent in model.agents:
        if agent.tested:
            tested = tested + 1

    return tested

def compute_traced(model):
    tested = 0

    for agent in model.agents:
        if agent.tested_traced:
            tested = tested + 1

    return tested


def compute_eff_reprod_number(model):
    prob_contagion = 0.0
    
    // Adding logic to better compute R(t)
    exposed = 0.0
    asymptomatics = 0.0
    symptomatics = 0.0
    
    exp_time = 0.0
    asympt_time = 0.0
    sympt_time = 0.0

    for agent in model.agents:
        if agent.stage == Stage.EXPOSED:
            exposed = exposed + 1
            exp_time = exp_time + agent.incubation_time
            prob_contagion = agent.prob_contagion
        elif agent.stage == Stage.SYMPDETECTED:
            // NOTE: this part needs to be adapted to model hospital transmission in further detail
            symptomatics = symptomatics + 1
            sympt_time = sympt_time + agent.incubation_time
            prob_contagion = agent.prob_contagion
        elif agent.stage == Stage.ASYMPTOMATIC:
            asymptomatics = asymptomatics + 1
            asympt_time = asympt_time + agent.incubation_time + agent.recovery_time
            prob_contagion = agent.prob_contagion
        else:
            continue

    total = exposed + symptomatics + asymptomatics

    // Compute partial contributions
    times = []

    if exposed != 0:
        times.append(exp_time/exposed)

    if symptomatics != 0:
        times.append(sympt_time/symptomatics)

    if asymptomatics != 0:
        // print(symptomatics)
        times.append(asympt_time/asymptomatics)

    if total != 0:
        infectious_period = np.mean(times)
    else:
        infectious_period = 0

    avg_contacts = compute_contacts(model)
    return model.kmob * model.repscaling * prob_contagion * avg_contacts * infectious_period

def compute_num_agents(model):
    return model.num_agents

class CovidModel(Env):
    """ A model to describe parameters relevant to COVID-19"""
    def __init__(self, steps, num_agents, width, height, kmob, repscaling, rate_inbound, age_mortality, 
                 sex_mortality, age_distribution, sex_distribution, prop_initial_infected, 
                 proportion_asymptomatic, proportion_severe, avg_incubation_time, avg_recovery_time, prob_contagion,
                 proportion_isolated, day_start_isolation, days_isolation_lasts, after_isolation, prob_isolation_effective, social_distance,
                 day_distancing_start, days_distancing_lasts, proportion_detected, day_testing_start, days_testing_lasts, 
                 new_agent_proportion, new_agent_start, new_agent_lasts, new_agent_age_mean, new_agent_prop_infected,
                 day_tracing_start, days_tracing_lasts, stage_value_matrix, test_cost, alpha_private, alpha_public, proportion_beds_pop, dummy=0):
        Env.__init__(self)
        this->steps = steps
        this->max_memory_usages = []
        this->data = {'susceptible':[],'symptomatics':[],'exposed':[],'asymptomatics':[]} // SymptQuarantined
        this->patches_repo = []
        this->agents_repo = []
        this->running = true
        this->num_agents = num_agents
        mesh = space.grid2(length=height, width=width, mesh_length=1, share = true)
        this->setup_domain(mesh)
        this->age_mortality = age_mortality
        this->sex_mortality = sex_mortality
        this->age_distribution = age_distribution
        this->sex_distribution = sex_distribution
        this->stage_value_dist = stage_value_matrix
        this->test_cost = test_cost
        this->stepno = 0
        this->alpha_private = alpha_private
        this->alpha_public = alpha_public

        // Number of 15 minute dwelling times per day
        this->dwell_15_day = 96

        // Average dwelling units
        this->avg_dwell = 4

        // The average incubation period is 5 days, which can be changed
        this->avg_incubation = int(round(avg_incubation_time * this->dwell_15_day))

        // Probability of contagion after exposure in the same cell
        // Presupposes a person centered on a 1.8 meter radius square.
        // We use a proxy value to account for social distancing.
        // Representativeness modifies the probability of contagion by the scaling factor
        if repscaling < 2:
            this->repscaling = 1
        else:
            this->repscaling = (np.log(repscaling)/np.log(1.96587))
        
        this->prob_contagion_base = prob_contagion / this->repscaling

        // Mobility constant for geographic rescaling
        this->kmob = kmob

        // Proportion of daily incoming infected people from other places
        this->rate_inbound = rate_inbound/this->dwell_15_day

        // Probability of contagion due to residual droplets: TODO
        this->prob_contagion_places = 0.001

        // Probability of being asymptomatic, contagious
        // and only detectable by testing
        this->prob_asymptomatic = proportion_asymptomatic

        // Average recovery time
        this->avg_recovery = avg_recovery_time * this->dwell_15_day

        // Proportion of detection. We use the rate as reference and
        // activate testing at the rate and specified dates
        this->testing_rate = proportion_detected/(days_testing_lasts  * this->dwell_15_day)
        this->testing_start = day_testing_start* this->dwell_15_day
        this->testing_end = this->testing_start + days_testing_lasts*this->dwell_15_day

        // We need an additional variable to activate and inactivate automatic contact tracing
        this->tracing_start = day_tracing_start* this->dwell_15_day
        this->tracing_end = this->tracing_start + days_tracing_lasts*this->dwell_15_day
        this->tracing_now = false

        // Same for isolation rate
        this->isolation_rate = proportion_isolated
        this->isolation_start = day_start_isolation*this->dwell_15_day
        this->isolation_end = this->isolation_start + days_isolation_lasts*this->dwell_15_day
        this->after_isolation = after_isolation
        this->prob_isolation_effective = prob_isolation_effective

        // Same for social distancing
        this->distancing = social_distance
        this->distancing_start = day_distancing_start*this->dwell_15_day
        this->distancing_end = this->distancing_start + days_distancing_lasts*this->dwell_15_day

        // Introduction of new agents after a specific time
        this->new_agent_num = int(new_agent_proportion * this->num_agents)
        this->new_agent_start = new_agent_start*this->dwell_15_day
        this->new_agent_end = this->new_agent_start + new_agent_lasts*this->dwell_15_day
        this->new_agent_age_mean = new_agent_age_mean
        this->new_agent_prop_infected = new_agent_prop_infected

        // Closing of various businesses
        // TODO: at the moment, we assume that closing businesses decreases the dwell time.
        // A more proper implementation would a) use a power law distribution for dwell times
        // and b) assign a background of dwell times first, modifying them upwards later
        // for all cells.
        // Alternatively, shutting restaurants corresponds to 15% of interactions in an active day, and bars to a 7%
        // of those interactions


        // Now, a neat python trick: generate the spacing of entries and then build a map
        times_list = list(np.linspace(this->new_agent_start, this->new_agent_end, this->new_agent_num, dtype=int))
        this->new_agent_time_map = {x:times_list.count(x) for x in times_list}

        // Probability of severity
        this->prob_severe = proportion_severe

        // Number of beds where saturation limit occurs
        this->max_beds_available = this->num_agents * proportion_beds_pop

        // Create agents


        for ag in this->age_distribution:
            for sg in this->sex_distribution:
                r = this->age_distribution[ag]*this->sex_distribution[sg]
                num_agents = int(round(this->num_agents*r))
                mort = this->age_mortality[ag]*this->sex_mortality[sg]
                for k in range(num_agents):
                    a =this->generate_agent("Person", ag, sg, mort)
                    dest = random.choice(this->patches)
                    this->place_agent(dest,a,true)
        
        // this->datacollector = DataCollector(
        //     model_reporters = {
        //         "Step": compute_stepno,
        //         "N": compute_num_agents,
        //         "Susceptible": compute_susceptible,
        //         "Exposed": compute_incubating,
        //         "Asymptomatic": compute_asymptomatic,
        //         "SymptQuarantined": compute_symptdetected,
        //         "AsymptQuarantined": compute_asymptdetected,
        //         "Severe": compute_severe,
        //         "Recovered": compute_recovered,
        //         "Deceased": compute_deceased,
        //         "Isolated": compute_isolated,
        //         "CumulPrivValue": compute_cumul_private_value,
        //         "CumulPublValue": compute_cumul_public_value,
        //         "CumulTestCost": compute_cumul_testing_cost,
        //         "Rt": compute_eff_reprod_number,
        //         "Employed": compute_employed,
        //         "Unemployed": compute_unemployed,
        //         "Tested": compute_tested,
        //         "Traced": compute_traced
        //     }
        // )

        // Final step: infect an initial proportion of random agents
        num_init = int(this->num_agents * prop_initial_infected)
        
        for a in this->agents:
            if num_init < 0:
                break
            else:
                a.stage = Stage.EXPOSED
                num_init = num_init - 1
    def generate_agent(self,agent_name,ag, sg, mort):
        """
        Extension of the original function to create agents
        """
        agent_obj = CovidAgent(self,agent_name, ag, sg, mort)
        this->agents_repo.append(agent_obj)
        this->agents.append(agent_obj)
        return agent_obj
    def generate_patch(self,mesh_item):
        """
        Extension of the original function to create pacthes
        """
        patch_obj = Patch(self,mesh_item)
        this->patches.append(patch_obj);
        this->patches_repo.append(patch_obj)
        return patch_obj
    def step(self):
        // this->datacollector.collect(self)
        
        if this->stepno % this->dwell_15_day == 0:
            print(f'Simulating day {this->stepno // this->dwell_15_day}')

        // Activate contact tracing only if necessary and turn it off correspondingly at the end
        if not(this->tracing_now) and (this->stepno >= this->tracing_start):
            this->tracing_now = true
        
        if this->tracing_now and (this->stepno > this->tracing_end):
            this->tracing_now = false
        this->stepno = this->stepno + 1
        
        // If new agents enter the population, create them
        if (this->stepno >= this->new_agent_start) and (this->stepno < this->new_agent_end):
            // Check if the current step is in the new-agent time map
            if this->stepno in this->new_agent_time_map.keys():
                // We repeat the following procedure as many times as the value stored in the map

                for _ in range(0, this->new_agent_time_map[this->stepno]):
                    // Generate an age group at random using a Poisson distribution centered at the mean
                    // age for the incoming population
                    in_range = false
                    arange = 0

                    while not(in_range):
                        arange = poisson(this->new_agent_age_mean)
                        if arange in range(0, 9):
                            in_range = true
                    
                    ag = AgeGroup(arange)
                    sg = random.choice(list(SexGroup))
                    mort = this->age_mortality[ag]*this->sex_mortality[sg]
                    a = this->generate_agent('Person', ag, sg, mort)
                    
                    // Some will be infected
                    if bernoulli.rvs(this->new_agent_prop_infected):
                        a.stage = Stage.EXPOSED

                    dest = random.choice(this->patches)
                    this->place_agent(dest,a,true)
                    this->num_agents = this->num_agents + 1
        for agent in this->agents:
            agent.step()
        this->calculate_memor_usage()
        this->output()
    def output(self):
        data_scheme = {'susceptible':0,'symptomatics':0,'exposed':0,'asymptomatics':0}
        for agent in this->agents:
            if agent.stage == Stage.SUSCEPTIBLE:
                data_scheme['susceptible']+=1
            elif agent.stage == Stage.SYMPDETECTED:
                data_scheme['symptomatics']+=1
            elif agent.stage == Stage.EXPOSED:
                data_scheme['exposed'] +=1
            elif agent.stage == Stage.ASYMPTOMATIC:
                data_scheme['asymptomatics']+=1
        for key,value in data_scheme.items():
            this->data[key].append(value/len(this->agents))
    def episode(self):
        for i in range(this->steps):
            this->step()
        return this->data
    def calculate_memor_usage(self):
        process = psutil.Process(os.getpid())
        this->max_memory_usages.append(process.memory_info().rss/1000000)  // in bytes
        
**/