
#ifndef ABM_MODEL_H
#define ABM_MODEL_H
#include <chrono>
#include <thread>
#include <pybind11/stl.h>
#include "patch.h"
#include "agent.h"
#include "settings.h"
#include "mesh.h"
namespace py = pybind11;
template <unsigned dim>
class Agent;
template <unsigned dim>
class Patch;
using namespace std;
//!  Frame class
/*!
  Takes care of generating mesh, creating patch, creating cells, etc. as well as executing them on an iterative manner.
*/
template<unsigned dim>
class Frame:public enable_shared_from_this<Frame<dim>>{
    friend class Agent<dim>;
    friend class Patch<dim>;
public:
    Frame(py::dict agent_modelObjs,py::object patch_model, settings_t settings);
    ~Frame(){};
    bool setup();        
    py::dict run();
    void  visualize() ; //!< To log requrested outputs for visualization
    void visualize_agents(const unsigned &iCount,const settings_t &specs) ;
    void visualize_patches(const unsigned &iCount,const settings_t &specs);
    std::map<std::string,std::vector<unsigned>> agent_counts;
    domain_data_t data;
    settings_t  settings;
    
    std::map<string,py::object>  agent_models;
    py::object patch_model;
    void setup_agents();
    void setup_patches();
    shared_ptr<Patch<dim>>& find_an_empty_patch();
    shared_ptr<Patch<dim>> find_an_empty_patch(shared_ptr<Patch<dim>>& refPatch);
    vector<shared_ptr<Agent<dim>>> agents_container; //!< Bank of Agent.
    vector<shared_ptr<Patch<dim>>> patches_container;
    vector<shared_ptr<Mesh<dim>>> meshes_container;

protected:
    bool append_flag=false;
    void _update();
    void update_agents();
    void update_patches();
    unsigned iCount;            //!< Iteration index of the simulation incremented in #run.
    unsigned duration_ticks;    //!< experiment duration
    
    domain_measurements_scheme_t measurements_scheme; //! the scheme for obtaining #data

};

template<unsigned dim>
inline Frame<dim>::Frame(py::dict agent_modelObjs, py::object patch_model_,settings_t settings_):
        iCount(0), settings(settings_)
{     
    srand(time(0));
   // initialize agent models 
   auto keys = agent_modelObjs.attr("keys")();
   for (auto &key:keys) {
       py::object agent_model = agent_modelObjs[key];
       std::string agent_type = key.cast<string>();
       agent_models.insert(std::pair<std::string,py::object>(agent_type,agent_model));
   }

   // initialize patch model
    patch_model = patch_model_;


    
}

template<unsigned dim>
inline bool Frame<dim>::setup(){
    tools::create_directory(main_output_folder);
    auto INITIALIZE_VARIABLES = [&](){
        this->duration_ticks = py::cast<unsigned>(settings["configs"]["exp_duration"]);

        auto PROCESS_DOMAIN_MEASUREMENTS_SCHEME = [&](){
            auto measurements = settings["configs"]["domain"]["measurements"];
            map<string,variant<string,vector<string>>> m_item;
            for (auto &m_item_:measurements){
                m_item.clear();
                for (auto &tag_:m_item_.attr("keys")()){
                    string tag = py::cast<string>(tag_);
                    if (tag == "on" or tag == "mode") m_item[tag] = py::cast<string>(m_item_[tag_]);
                    else if (tag == "tags") m_item[tag] = py::cast<vector<string>>(m_item_[tag_]);
                    else {
                        std::cerr<<RED<<"Error: "<<RESET<<" input name '"<<RED<<tag<<RESET<<"' is not acceptable for domain measurements inputs."<<endl;
                        std::terminate();
                    }
                }
                this->measurements_scheme.push_back(m_item);
            }
        };
        PROCESS_DOMAIN_MEASUREMENTS_SCHEME();   
    };
    INITIALIZE_VARIABLES();
    Patch<dim>::_modelPtr() = this->shared_from_this();
    setup_patches();
    Agent<dim>::_mPtr() = this->shared_from_this();
    setup_agents();
    this->append_flag = true;
    this->_update();
    return true;
}

template<unsigned dim>
inline void Frame<dim>::setup_agents(){
    // create the agents
    for (auto key:settings["configs"]["agents"].attr("keys")()){
        unsigned agent_count = py::cast<unsigned>(settings["configs"]["agents"][key]["initial_n"]);

        string agent_name = py::cast<string>(key);
        // create initial agents and assign attributes
        for(int iter=0; iter<agent_count; iter++) {
            try{
                auto host_patch = find_an_empty_patch();
                auto agent = Agent<dim>::hatch_a_cell(host_patch,agent_name);
                agents_container.push_back(agent);
            }catch(tools::no_available_patch&nap){
                nap.what();
                std::terminate();
            }
        }

    }
    string message = std::to_string(agents_container.size()) + " cells successfully created";
    LOG(message);
}

template<unsigned dim>
inline py::dict Frame<dim>::run(){
    auto EXECUTE = [&](){
        this->append_flag = true;
        visualize();
        for (auto &patch:patches_container) {
            patch->function();
        }
        for (auto &agent:agents_container) {
                agent->function();
        };
        update_agents();
        update_patches();
        _update();
    };
    auto TIME_CONTROL = [&](){
        
        // first passage the model before starting the actual run
        // while (pIter < inputs::params()["end_passaging_n"]){
        //     iCount = 1;
        //     // while (results_repos.back()._quantities_float[] < inputs::params()["confluence_t"]){
        //     //     try{
        //     //        EXECUTE(); 
        //     //    }catch(...){
        //     //         cerr<<"got a problem in execute"<<endl;
        //     //         exit(2);
        //     //    }
                
        //     //     iCount++;
        //     //     if (iCount >=tools::day_to_tick_convertor(inputs::params()["convergence_max_days"])) throw convergence_error("ABM cannot converge");
        //     // }
        //     _passaging();
        //     pIter++;
        // }
        // this is the actual run
        iCount = 1;
        while (iCount < duration_ticks) {
            int barWidth = 70;
            LOG("[");
            int pos = barWidth * (float)iCount/duration_ticks;
            for (int i = 0; i < barWidth; ++i) {
                if (i < pos) LOG("=");
                else if (i == pos) LOG(">");
                else LOG(" ");
            }
            string message = "] " + to_string(iCount) + " Iterations\r";
            LOG(message);
            std::cout.flush();

            EXECUTE();
            iCount++;
        }
        LOG("\n");

    };
    tools::_clock::start();
    TIME_CONTROL();

    py::dict returns;
    for (auto &key_:settings["returns"].attr("keys")()){
        string key = py::cast<string>(key_);
        if (key == "agents_count"){
            vector<unsigned> time_points = py::cast<vector<unsigned>>(settings["returns"][key_]);
            auto sliced_data = tools::slice_map(agent_counts,time_points);
            py::dict returns1 = py::cast(sliced_data);
            returns[key_] = returns1;
        }
    };
    
    tools::_clock::end();
    LOG("Run completed");
    return returns;
}




template<unsigned dim>
inline void Frame<dim>::_update(){
    auto UPDATE_AGENT_COUNT = [&](){
        map<string,unsigned> agent_counts_single;
        for (unsigned i = 0; i < agents_container.size(); i++) {
            auto cell =  agents_container[i];
            agent_counts_single[cell->c_type]++;
        }
        for (auto &agent_model:agent_models){
            auto agent_tag = agent_model.first;
            if (agent_counts_single.find(agent_tag) == agent_counts_single.end()){ // this agent doesnt have any record in container
                agent_counts_single[agent_tag] = 0;
            }
        }
        for (auto &item:agent_counts_single){
            auto tag = item.first;
            auto value = item.second;
            if (this->agent_counts.find(tag) == this->agent_counts.end()){
                this->agent_counts[tag] = vector<unsigned>();
            }
            if (this->append_flag == true) this->agent_counts[tag].push_back(value);
            else this->agent_counts[tag].back() = value;
        }
    };
    UPDATE_AGENT_COUNT();

    auto UPDATE_DOMAIN_DATA = [&](){
        auto UPDATE_DOMAIN_PATCH_DATA = [&](auto &m_item){
            auto tags = std::get<vector<string>>(m_item["tags"]);
            auto mode = std::get<string>(m_item["mode"]);
            for (const auto &tag:tags){
                float sum = 0;
                for (auto &patch:patches_container){
                    sum+=patch->data[tag];
                }
                float result = 0;
                if (mode == "sum") result = sum;
                else if (mode == "mean") result = sum/patches_container.size();
                else {
                    std::cerr<<RED<<"Error: "<<RESET<<" mode entered as '"<<RED<<mode<<RESET<<"' is not acceptable."<<endl;
                    std::terminate();
                }
                if (this->append_flag == true) this->data["patch"][tag].push_back(result);
                else this->data["patch"][tag].back() = result;
            }      
        };
        auto UPDATE_DOMAIN_AGENT_DATA = [&](auto &m_item){
            auto tags = std::get<vector<string>>(m_item["tags"]);
            auto mode = std::get<string>(m_item["mode"]);
            for (const auto &tag:tags){
                float sum = 0;
                for (auto &agent:agents_container){
                    sum+=agent->data[tag];
                }
                float result = 0;
                if (mode == "sum") result = sum;
                else if (mode == "mean") result = sum/agents_container.size();
                else {
                    std::cerr<<RED<<"Error: "<<RESET<<" mode entered as '"<<RED<<mode<<RESET<<"' is not acceptable."<<endl;
                    std::terminate();
                }
                if (this->append_flag == true) this->data["agent"][tag].push_back(result);
                else this->data["agent"][tag].back() = result;
            }      
        };
        for ( auto& m_item: this->measurements_scheme){
            auto on = std::get<string>(m_item["on"]);
            if (on == "patch") UPDATE_DOMAIN_PATCH_DATA(m_item);
            else if (on == "agent") UPDATE_DOMAIN_AGENT_DATA(m_item);
            else {
                std::cerr<<RED<<"Error: "<<RESET<<" the entered '"<<RED<<on<<RESET<<"' as domain measurements on is neither patch or agent."<<endl;
                std::terminate();
            }
        }        
    };
    UPDATE_DOMAIN_DATA();
    this->append_flag = false;

}



template<unsigned dim>
inline void Frame<dim>::visualize()  {
    bool logs_flag = py::cast<bool>(settings["logs"]["flag"]);
    if (!logs_flag) return; // no visualization
    // /***  cell distriburion visualization ***/
    // std::chrono::seconds dura( 2);
    // std::this_thread::sleep_for( dura );
    visualize_agents(iCount,settings["logs"]);
    visualize_patches(iCount,settings["logs"]);
}

template<unsigned dim>
inline void  Frame<dim>::visualize_agents(const unsigned &iCount,const settings_t &specs){

    auto SCATTER_LOG = [&](auto &item){
        vector<string> tags = py::cast<vector<string>>(item["tags"]);
        string dir =py::cast<string>(item["dir"]);
        ofstream O(dir);
        O<<"x,y,type";
        for (auto &tag:tags){
        O<<","<<tag;
        }
        O<<"\n";
        unsigned ii =0;
        for (auto &agent:agents_container){
        float x = agent->getPatch()->xyzcoords[0];
        float y = agent->getPatch()->xyzcoords[1];
        // if (ii == 0) cout<<agent->getPatch()->patchIndex<<endl;

        string type = agent->c_type;
        O<<x<<","<<y<<","<<type;
        for (auto &tag:tags){
            float value;
            try {value = agent->data.at(tag);}
            catch(out_of_range &er){
                cerr<<"Error: '"<<tag<<"' requested for visualization is not defined in agent attributes"<<endl;
                std::terminate();
            }
            O<<","<<value;
          }
        O<<"\n";
        ii++;
        }
        O.close();
    };
    auto AGENT_COUNT_LOG = [&](auto &item){
        string dir =py::cast<string>(item["dir"]);
        ofstream O(dir);
        unsigned ii =0;
        for (auto &agent_count_item: agent_counts){
            auto agent_tag = agent_count_item.first;
            if (ii == 0) {
                O << agent_tag;
            }
            else O<<","<<agent_tag;
            ii++;
        }
        O<<"\n";
        for (unsigned iter =0; iter != iCount; iter++){
            unsigned ii =0;
            for (auto &agent_count_item: agent_counts){
                auto value = agent_count_item.second[iter];
                if (ii == 0) O << value;
                else O<<","<<value;
                ii++;
            }
            O<<"\n";
        }

        O.close();
    };
    auto TRAJ_LOG = [&](auto &item){
        string dir = py::cast<string>(item["dir"]);
        ofstream O(dir);
        vector<string> tags = py::cast<vector<string>>(item["tags"]);
        //header
        unsigned ii = 0;
        for (auto &tag:tags){
            if (ii == 0) O<<tag;
            else O<<","<<tag;
            ii ++;
        }
        O<<"\n";

        for (unsigned iter =0; iter != iCount; iter++){
            unsigned ii =0;
            for (auto &tag: tags){
                if (data["agent"].find(tag) == data["agent"].end()){
                    std::cerr<<RED<<"Error: "<<RESET<<" the tag '"<<RED<<tag<<RESET<<"' requested for agent log but missing in the domain variables."<<endl;
                    std::terminate();
                }
                else{
                    auto value = data["agent"][tag][iter];
                    if (ii == 0) O << value;
                    else O<<","<<value;
                    ii++;
                }

            }
            O<<"\n";
        }
        O.close();
    };
    for (auto &item:specs["agent"]){
        bool item_logs_flag = py::cast<bool>(item["flag"]);
        if (item_logs_flag == false) continue;
        unsigned interval = py::cast<unsigned>(item["interval"]);
        if (iCount != 0) {
            if ( iCount % interval != 0) continue;  // interval is not met
        }
        string type = py::cast<string>(item["type"]);
        if (type == "scatter") SCATTER_LOG(item);
        else if (type == "agents_count_traj") AGENT_COUNT_LOG(item);
        else if (type == "traj") TRAJ_LOG(item);
        else {
            std::cerr<<RED<<"Error: "<<RESET<<" type of log entered as '"<<RED<<type<<RESET<<"' is not acceptable."<<endl;
            std::terminate();
        }

    }


}

template<unsigned dim>
inline void Frame<dim>::update_agents() {
    /*** Update age-related members ***/

    for (auto &cell:agents_container){
        if (cell->switch_info.first == true){ // switch
            // cout<<"switching"<<endl;
            shared_ptr<Patch<dim>> host_patch = cell->getPatch();
            string new_type = cell->switch_info.second;
            host_patch->removeAgent();
            auto new_cell = Agent<dim>::hatch_a_cell(host_patch,new_type);
            auto pos = distance(agents_container.begin(), find(agents_container.begin(), agents_container.end(), cell));
            agents_container[pos] = new_cell;
        }
    }

    /* in case of disappear, Agent<dim>::agents_container needs to be updated */

    int jj = 0;
    while (true) {
        if (jj >= agents_container.size()) break;
        for (int ii = jj; ii < agents_container.size(); ii++) {
            if (agents_container[ii]->disappear_flag == true) {
                agents_container.erase(agents_container.begin() + ii);
                break;
            }
            jj++;
        };
    }

    /*** Check for mitosis ***/
    for (unsigned iter = 0 ; iter < agents_container.size(); iter++){
        auto cell = agents_container[iter];
        if (cell->hatch_flag == true) {
            // cout<<"hatching"<<endl;
            shared_ptr<Agent<dim>> new_agent;
            try {
                shared_ptr<Patch<dim>> ref_patch = cell->getPatch();
                shared_ptr<Patch<dim>> host_patch = find_an_empty_patch(ref_patch);
                shared_ptr<Agent<dim>> ref_cell = cell->getPtr();
                new_agent = Agent<dim>::hatch_a_cell(host_patch,ref_cell);
                agents_container.push_back(new_agent);
            } catch (tools::no_available_patch & er) {
                // cerr<<"Warning: hatch cannot occur due to high agent density"<<endl;
                continue;
            }
            cell->hatch_flag = false;
            new_agent->hatch_flag = false;
            cell->data["hatch_cycle_c"]++;
            new_agent->data["hatch_cycle_c"]++;
        }
    }



    for (unsigned iter = 0; iter < agents_container.size(); iter++){
        // cout<<iter<<" out of "<<agents_container.size()<<endl;
        auto cell =  agents_container[iter];
        if (cell->walk_flag == false) continue;
        // cout<<"M 1"<<endl;
        shared_ptr<Patch<dim>> destination;
        shared_ptr<Patch<dim>> ref_patch = cell->getPatch();
        try {destination = find_an_empty_patch(ref_patch);}
        catch (tools::no_available_patch & err) {
            continue;
        }
        auto current_patch = cell->getPatch();
        current_patch->removeAgent();
        destination->setAgent(cell);
        cell->setPatch(destination);
    }


}
template <unsigned dim>
inline void Frame<dim>::setup_patches(){
    float x_l = py::cast<float>(settings["configs"]["domain"]["x_l"]);
    float y_l = py::cast<float>(settings["configs"]["domain"]["y_l"]); 
    Mesh<dim>::_modelPtr() = this->shared_from_this();
    Mesh<dim>::setup_meshes(x_l,y_l, py::cast<float>(settings["configs"]["domain"]["patch_size"]));
    /*** define patches on grids ***/
    for (int iter=0; iter<meshes_container.size(); iter++)
    {
        auto mesh = Mesh<dim>::at(iter);
        auto pPtr = make_shared<Patch<dim>>();
        pPtr->initialize();
        pPtr->patchIndex = mesh->get_meshindex(); // relating mesh index to associated patch index

        auto coords = mesh->get_globalxyzcoordinates();
        pPtr->xyzcoords= coords;
        patches_container.push_back(pPtr);

    }

    for (int iter=0; iter<meshes_container.size(); iter++){
        auto mesh = Mesh<dim>::at(iter);
        auto neighbors = mesh->neighbor_indices; // relating mesh neighbors to associated patch neighbors
        auto patch = patches_container[iter];
        for (const auto &neighbor:neighbors){
            auto neighborPatch = patches_container[neighbor];
            patch->neighborPatches.push_back(neighborPatch);
        }
    }
}

template<unsigned dim>
inline void Frame<dim>::visualize_patches(const unsigned &iCount,const settings_t &specs){
    auto DENSITYMAP_LOG = [&](auto &item){
        string dir = py::cast<string>(item["dir"]);
        ofstream O(dir);
        // header
        vector<string> tags = py::cast<vector<string>>(item["tags"]);
        O<<"x,y";
        for (auto &tag:tags){
            O<<","<<tag;
        }
        O<<"\n";        
        for (auto &patch:patches_container){ 
            float x = patch->xyzcoords[0];
            float y = patch->xyzcoords[1];
            O<<x<<","<<y;
            for (auto &tag:tags){
                float value;
                try {value = patch->data.at(tag);}
                catch(out_of_range &er){
                    cerr<<"Error: '"<<tag<<"' requested for visualization is not defined in patch attributes"<<endl;
                    std::terminate();
                }
                O<<","<<value;
              }
            O<<"\n";
        }
        O.close();
    };
    auto TRAJ_LOG = [&](auto &item){
        string dir = py::cast<string>(item["dir"]);
        ofstream O(dir);
        vector<string> tags = py::cast<vector<string>>(item["tags"]);
        //header
        unsigned ii = 0;
        for (auto &tag:tags){
            if (ii == 0) O<<tag;
            else O<<","<<tag;
            ii ++;
        }
        O<<"\n";

        for (unsigned iter =0; iter != iCount; iter++){
            unsigned ii =0;
            for (auto &tag: tags){
                if (data["patch"].find(tag) == data["patch"].end()){
                    std::cerr<<RED<<"Error: "<<RESET<<" requested tag '"<<RED<<tag<<RESET<<"' for patch log is not defined in the domain variables."<<endl;
                    std::terminate();
                }
                else{
                    auto value = data["patch"][tag][iter];
                    if (ii == 0) O << value;
                    else O<<","<<value;
                    ii++;
                }
                
            }
            O<<"\n"; 
        }
        

        O.close();
    };

    for (auto &item:specs["patch"]){
        bool item_logs_flag = py::cast<bool>(item["flag"]);
        if (item_logs_flag == false) continue;
        unsigned interval = py::cast<unsigned>(item["interval"]);
        if (iCount != 0) {
            if ( iCount % interval != 0) continue;  // interval is not met
        }
        string type = py::cast<string>(item["type"]);
        if (type == "densitymap"){
            DENSITYMAP_LOG(item);
        }
        else if (type == "traj"){
            TRAJ_LOG(item);
        }
        else {
            std::cerr<<RED<<"Error: "<<RESET<<" type of log entered as '"<<RED<<type<<RESET<<"' is not acceptable."<<endl;
            std::terminate();
        }
    }
      

}

template<unsigned dim>
inline shared_ptr<Patch<dim>>& Frame<dim>::find_an_empty_patch(){
    int randIndex;
    for(int iter=0; iter<patches_container.size(); iter++){
        randIndex =rand() %patches_container.size();
        if (patches_container[randIndex]->hasAgent()) {
            continue;
        }
        else return patches_container[randIndex];

    }
    throw tools::no_available_patch("There is no available patch for agent allocation"); //in case there is no empty patch available
}
template<unsigned dim>
inline shared_ptr<Patch<dim>> Frame<dim>::find_an_empty_patch(shared_ptr<Patch<dim>>& refPatch){
    auto neighborPatches = refPatch->neighborPatches;
    // refPatch.reset(); //pointer to refPatch is no longer needed
    vector<shared_ptr<Patch<dim>>> free_patch_container; //available neighbor patches will be stored here
    for (auto neighborPatch:neighborPatches){
        if (!neighborPatch->hasAgent()) free_patch_container.push_back(neighborPatch);
    }

    int freeNeighborsCount = free_patch_container.size();
    for (int iter = 0; iter<freeNeighborsCount; iter++){
        int randn = rand()%freeNeighborsCount;
        auto freeAvailablePatch = free_patch_container[randn];
        return freeAvailablePatch;
    }
    throw tools::no_available_patch("There is no available patch for agent allocation");
}


template<unsigned dim>
inline void Frame<dim>::update_patches(){
    auto CALCULATE_AGENTDENSITY = [&](){
        for (auto &patch:patches_container){
            int cellCount=0;
            for (auto neighborPatch:patch->neighborPatches){
                if (neighborPatch->_hasAgent) {
                    cellCount ++;
                }
            }
            if (patch->_hasAgent) {
                cellCount ++;
            }
            patch->data["agent_density"] = float(cellCount)/(patch->neighborPatches.size()+1);
        }
    };
    CALCULATE_AGENTDENSITY();
}
#endif //ABM_MODEL_H
