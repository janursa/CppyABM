

#from covidmodel import Stage
#from covidmodel import AgeGroup
#from covidmodel import SexGroup
#from covidmodel import LockGroup
import pandas as pd
import sys
import os
import pathlib
current_dir = os.getcwd()
tool = sys.argv[2]
# tool = 'Mesa'
# tool = 'CppyABM'
if tool == 'Mesa':
    from covidmodel_original import *
elif tool == 'CppyABM':
    from covidmodel import *
else:
    raise ValueError()
import json
import time

file_params = sys.argv[1]

# Read JSON file
with open(file_params) as f:
  data = json.load(f)

print(f"Location: { data['location'] }")
print(f"Description: { data['description'] }")
print(f"Prepared by: { data['prepared-by'] }")
print(f"Date: { data['date'] }")
print("")
print("Attempting to configure model from file...")

# Observed distribution of mortality rate per age
age_mortality = {
    AgeGroup.C80toXX: data["model"]["mortalities"]["age"]["80+"],
    AgeGroup.C70to79: data["model"]["mortalities"]["age"]["70-79"],
    AgeGroup.C60to69: data["model"]["mortalities"]["age"]["60-69"],
    AgeGroup.C50to59: data["model"]["mortalities"]["age"]["50-59"],
    AgeGroup.C40to49: data["model"]["mortalities"]["age"]["40-49"],
    AgeGroup.C30to39: data["model"]["mortalities"]["age"]["30-39"],
    AgeGroup.C20to29: data["model"]["mortalities"]["age"]["20-29"],
    AgeGroup.C10to19: data["model"]["mortalities"]["age"]["10-19"],
    AgeGroup.C00to09: data["model"]["mortalities"]["age"]["00-09"],
}

# Observed distribution of mortality rage per sex
sex_mortality = {
    SexGroup.MALE: data["model"]["mortalities"]["sex"]["male"],
    SexGroup.FEMALE: data["model"]["mortalities"]["sex"]["female"],
}

age_distribution = {
    AgeGroup.C80toXX: data["model"]["distributions"]["age"]["80+"],
    AgeGroup.C70to79: data["model"]["distributions"]["age"]["70-79"],
    AgeGroup.C60to69: data["model"]["distributions"]["age"]["60-69"],
    AgeGroup.C50to59: data["model"]["distributions"]["age"]["50-59"],
    AgeGroup.C40to49: data["model"]["distributions"]["age"]["40-49"],
    AgeGroup.C30to39: data["model"]["distributions"]["age"]["30-39"],
    AgeGroup.C20to29: data["model"]["distributions"]["age"]["20-29"],
    AgeGroup.C10to19: data["model"]["distributions"]["age"]["10-19"],
    AgeGroup.C00to09: data["model"]["distributions"]["age"]["00-09"],
}

# Observed distribution of mortality rage per sex
sex_distribution = {
    SexGroup.MALE: data["model"]["distributions"]["sex"]["male"],
    SexGroup.FEMALE: data["model"]["distributions"]["sex"]["female"],
}
# Value distribution per stage per interaction (micro vs macroeconomics)
value_distibution = {
    ValueGroup.PRIVATE: {
        Stage.SUSCEPTIBLE: data["model"]["value"]["private"]["susceptible"],
        Stage.EXPOSED: data["model"]["value"]["private"]["exposed"],
        Stage.SYMPDETECTED: data["model"]["value"]["private"]["sympdetected"],
        Stage.ASYMPTOMATIC: data["model"]["value"]["private"]["asymptomatic"],
        Stage.ASYMPDETECTED: data["model"]["value"]["private"]["asympdetected"],
        Stage.SEVERE: data["model"]["value"]["private"]["severe"],
        Stage.RECOVERED: data["model"]["value"]["private"]["recovered"],
        Stage.DECEASED: data["model"]["value"]["private"]["deceased"]
    },
    ValueGroup.PUBLIC: {
        Stage.SUSCEPTIBLE: data["model"]["value"]["public"]["susceptible"],
        Stage.EXPOSED: data["model"]["value"]["public"]["exposed"],
        Stage.SYMPDETECTED: data["model"]["value"]["public"]["sympdetected"],
        Stage.ASYMPTOMATIC: data["model"]["value"]["public"]["asymptomatic"],
        Stage.ASYMPDETECTED: data["model"]["value"]["public"]["asympdetected"],
        Stage.SEVERE: data["model"]["value"]["public"]["severe"],
        Stage.RECOVERED: data["model"]["value"]["public"]["recovered"],
        Stage.DECEASED: data["model"]["value"]["public"]["deceased"]
    }
}

model_params = {
        "num_agents": data["model"]["epidemiology"]["num_agents"],
        "width": data["model"]["epidemiology"]["width"],
        "height": data["model"]["epidemiology"]["height"],
        "repscaling": data["model"]["epidemiology"]["repscaling"],
        "kmob": data["model"]["epidemiology"]["kmob"],
        "age_mortality": age_mortality,
        "sex_mortality": sex_mortality,
        "age_distribution": age_distribution,
        "sex_distribution": sex_distribution,
        "prop_initial_infected": data["model"]["epidemiology"]["prop_initial_infected"],
        "rate_inbound": data["model"]["epidemiology"]["rate_inbound"],
        "avg_incubation_time": data["model"]["epidemiology"]["avg_incubation_time"],
        "avg_recovery_time": data["model"]["epidemiology"]["avg_recovery_time"],
        "proportion_asymptomatic": data["model"]["epidemiology"]["proportion_asymptomatic"],
        "proportion_severe": data["model"]["epidemiology"]["proportion_asymptomatic"],
        "prob_contagion": data["model"]["epidemiology"]["proportion_asymptomatic"],
        "proportion_beds_pop": data["model"]["epidemiology"]["proportion_beds_pop"],
        "proportion_isolated": data["model"]["policies"]["isolation"]["proportion_isolated"],
        "day_start_isolation": data["model"]["policies"]["isolation"]["day_start_isolation"],
        "days_isolation_lasts": data["model"]["policies"]["isolation"]["days_isolation_lasts"],
        "after_isolation": data["model"]["policies"]["isolation"]["after_isolation"],
        "prob_isolation_effective": data["model"]["policies"]["isolation"]["prob_isolation_effective"],
        "social_distance": data["model"]["policies"]["distancing"]["social_distance"],
        "day_distancing_start": data["model"]["policies"]["distancing"]["day_distancing_start"],
        "days_distancing_lasts": data["model"]["policies"]["distancing"]["days_distancing_lasts"],
        "proportion_detected": data["model"]["policies"]["testing"]["proportion_detected"],
        "day_testing_start": data["model"]["policies"]["testing"]["day_testing_start"],
        "days_testing_lasts": data["model"]["policies"]["testing"]["days_testing_lasts"],
        "day_tracing_start": data["model"]["policies"]["tracing"]["day_tracing_start"],
        "days_tracing_lasts": data["model"]["policies"]["tracing"]["days_tracing_lasts"],
        "new_agent_proportion": data["model"]["policies"]["massingress"]["new_agent_proportion"],
        "new_agent_start": data["model"]["policies"]["massingress"]["new_agent_start"],
        "new_agent_lasts": data["model"]["policies"]["massingress"]["new_agent_lasts"],
        "new_agent_age_mean": data["model"]["policies"]["massingress"]["new_agent_age_mean"],
        "new_agent_prop_infected": data["model"]["policies"]["massingress"]["new_agent_prop_infected"],
        "stage_value_matrix": value_distibution,
        "test_cost": data["model"]["value"]["test_cost"],
        "alpha_private": data["model"]["value"]["alpha_private"],
        "alpha_public": data["model"]["value"]["alpha_public"],
        "steps":data['ensemble']['steps']
    }

print('Running on the platform {}'.format(tool))
try:
    print(os.path.join(current_dir,tool))
    os.mkdir(os.path.join(current_dir,tool))
except:
    pass
memory_averages = []
outputs_sum = {}
for iter_n in range(data['ensemble']['runs']):
    begin = time.time()
    model =CovidModel(**model_params)
    output = model.episode()
    end = time.time()
    output = pd.DataFrame(output)
    output.to_csv('{}/outputs_{}.csv'.format(tool,iter_n))
    print("Simulation took {} seconds".format(end-begin))
    memory_averages.append(np.mean(model.max_memory_usages))



memory_averages = pd.DataFrame({'memory':memory_averages})
memory_averages.to_csv(tool+'/memory_averages.csv')
# plt.plot(model.max_memory_usages)
# plt.savefig('{}_memory_{}.svg'.format(tool,iter_n))

