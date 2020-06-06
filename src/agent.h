
// #pragma once
// #include "bases.h"
// struct MSC:public Agent{
//     Agent(string);                                        
//     explicit Agent(std::shared_ptr<Agent<dim>> &cellPtr); //!< Agent constructor based on agent type
//     virtual ~Agent();                                     //!< Agent constructor based on a reference agent. 
//     static shared_ptr<Agent<dim>> hatch_a_cell(shared_ptr<Patch<dim>>& host_patch, std::shared_ptr<Agent<dim>> & ref_cell);
//     static shared_ptr<Agent<dim>> hatch_a_cell(shared_ptr<Patch<dim>>& host_patch, string cell_type);

//     void initialize_attributes();
    


//              //!< Sets _patchPtr
//     void function();                        //!< Agent function. This is the main function that calls all other cell activity functions such as #_mitosisFunc and #_diffFunc.

    

    
//     settings_t configs(){
//         return getFramePointer()->settings["configs"];
        
//     };
//     static weak_ptr<Frame<dim>>& _mPtr(){     //!< Weak pointer to the model
//         static weak_ptr<Frame<dim>> var{};
//         return var;
//     };
    
//     py::object get_agent_model(string &type){
//         return getFramePointer()->agent_models[type];
//     }

//     /*** common methods ***/
//     void update_agent_inputs();
//     void receive_agent_outputs();


// };
// template<unsigned dim>
// inline shared_ptr<Agent<dim>> Agent<dim>::hatch_a_cell(shared_ptr<Patch<dim>>& host_patch, std::shared_ptr<Agent<dim>> & ref_cell){
//     shared_ptr<Agent<dim>> cPtr(new Agent<dim>(ref_cell));
//     host_patch->setAgent(cPtr);   // cross-association
//     cPtr->setPatch(host_patch);  // cross-association
//     return cPtr;
// }
// template<unsigned dim>
// inline shared_ptr<Agent<dim>> Agent<dim>::hatch_a_cell(shared_ptr<Patch<dim>>& host_patch, string cell_type ){
//     shared_ptr<Agent<dim>> cPtr(new Agent<dim>(cell_type));
//     host_patch->setAgent(cPtr);   // cross-association
//     cPtr->setPatch(host_patch);  // cross-association
//     cPtr->initialize_attributes();
//     return cPtr;
// }


// template<unsigned dim>
// inline void Agent<dim>::initialize_attributes(){
//     py::str tag = py::cast(c_type);
//     // cout<<"start"<<endl;
//     for (auto attr_key:configs()["agents"][tag]["attrs"].attr("keys")()){
//         string attr = py::cast<string>(attr_key);
//         float value = py::cast<float>(configs()["agents"][tag]["attrs"][attr_key]);
//         this->data.insert(std::pair<string,float>(attr,value));
//     }
//     // cout<<"end"<<endl;
// }






// template<unsigned dim>
// inline Agent<dim>::Agent(string cell_type):
//         c_type(cell_type)
// {

// }
// template<unsigned dim>
// inline Agent<dim>::Agent(shared_ptr<Agent<dim>> &cellPtr):Agent(cellPtr->c_type){
//    this->c_type = cellPtr->c_type;
//    this->data = cellPtr->data;
// }

// template<unsigned dim>
// inline Agent<dim>::~Agent(){

// }








// template <unsigned dim>
// inline void Agent<dim>::update_agent_inputs(){
//     auto main_tags = get_agent_model(this->c_type).attr("inputs").attr("keys")();
//     // for self inputs
//     auto EXTRACT_SELF_INPUTS = [&](){
//         for (auto tag:get_agent_model(this->c_type).attr("inputs")["self"].attr("keys")()){ // collecting inputs from self
//             // py::print(tag);
//             float value;
// //            cout<<"update_agent_inputs "<<tag<<" "<<this->data.at(py::cast<string>(tag))<<endl;
//             try {value = this->data.at(py::cast<string>(tag));}
//             catch (out_of_range & er) {
//                 cerr<<"The input '"<<tag<<"' for agent [self] is not defined in '"<<this->c_type<<"' attributes but requested as input"<<endl;
//                 std::terminate();
//             }
//             get_agent_model(this->c_type).attr("inputs")["self"][tag] = value;
//         }
//     };
//     // for patch inputs
//     auto EXTRACT_PATCH_INPUTS = [&](){
//         for (auto &tag:get_agent_model(this->c_type).attr("inputs")["patch"].attr("keys")()){ // collecting inputs from patch
//             float value;
//             try {value = this->getPatch()->data.at(py::cast<string>(tag));}
//             catch (out_of_range & er) {
//                 cerr<<"The input key '"<<tag<<"' for agent [patch] is not defined in patch attributes "<<endl;
//                 std::terminate();
//             }
//             vector<float> values {value};
//             get_agent_model(this->c_type).attr("inputs")["patch"][tag] = value ;
//         }
//     };
//     for (auto &tag_:main_tags){
//         string tag = py::cast<string>(tag_);
//         if (tag == "self") EXTRACT_SELF_INPUTS();
//         else if (tag == "patch") EXTRACT_PATCH_INPUTS();
//         else {
//             cerr<<"Error: output key '"<<tag<<"' is neither self or patch. Correct the input formating. "<<endl;
//             std::terminate();
//         }
//     }

//     // // for neighbors  inputs
//     // auto EXTRACT_NEIGHBORS_INPUTS = [&](){
//     //     vector<string> from_neighbors_inputs = configs()[this->c_type]["inputs"]["neighbors"];
//     //     for (auto &tag:from_neighbors_inputs){ // collecting inputs from patch
//     //         vector<float> values;
//     //         for (auto &patch:this->getPatch()->neighborPatches){
//     //             float value;
//     //             try {value = patch->data.at(tag);}
//     //             catch (out_of_range & er) {
//     //                 cerr<<"The input key '"<<tag<<"' for agent [neighbors] is not defined in patch attributes "<<endl;
//     //                 std::terminate();
//     //             }
//     //             values.push_back(value);
//     //         }
//     //         inputs["neighbors"].insert(std::pair<string,vector<float>>(tag,values));
//     //     }
//     // };
//     // if (configs()[this->c_type]["inputs"]["neighbors"] != nullptr) EXTRACT_NEIGHBORS_INPUTS();
//     // return inputs;
// }
// template <unsigned dim>
// inline void Agent<dim>::receive_agent_outputs(){
//     get_agent_model(c_type).attr("forward")();  // updates the outputs in python file
//     auto main_tags = get_agent_model(c_type).attr("outputs").attr("keys")();
//     auto RECEIVE_SELF_OUTPUTS = [&](py::dict sub_output){
//         for (const auto event_key_:sub_output.attr("keys")()){
//                 auto event_key = py::cast<string>(event_key_);
//                 // auto it = this->data.find(item.first);
//                 if (event_key == "hatch"){
//                     this->hatch_flag = py::cast<bool>(sub_output[event_key_]);
//                 }
//                 else if (event_key == "walk"){
//                     this->walk_flag = py::cast<bool>(sub_output[event_key_]);
//                 }
//                 else if (event_key == "switch"){
//                     auto flag = py::str(sub_output[event_key_]).is(py::str(Py_False));
//                     if (!flag) {
//                         this->switch_info.first = true;
//                         this->switch_info.second = py::cast<string>(sub_output[event_key_]);
//                     }
//                 }
//                 else if (event_key == "disappear"){
//                     this->disappear_flag = py::cast<bool>(sub_output[event_key_]);
//                 }
//                 else{
//                     this->data[event_key] = py::cast<float>(sub_output[event_key_]);
// //                    cout<<"receive outputs "<<event_key<<this->data.at(event_key)<<endl;
//                 }
//             }
//     };
//     auto RECEIVE_PATCH_OUTPUTS = [&](py::dict sub_output){
//         for (const auto event_key_:sub_output.attr("keys")()){
//                 auto event_key = py::cast<string>(event_key_);
//                 this->getPatch()->data[event_key] = py::cast<float>(sub_output[event_key_]);
//             }
//         };
//     for (auto &tag_:main_tags){
//         string tag = py::cast<string>(tag_);
//         auto sub_output = get_agent_model(c_type).attr("outputs")[tag_];
//         if (tag == "self") RECEIVE_SELF_OUTPUTS(sub_output);
//         else if (tag == "patch") RECEIVE_PATCH_OUTPUTS(sub_output);
//         else {
//             cerr<<"Error: output key '"<<tag<<"' is neither self or patch. Correct the output formating. "<<endl;
//             std::terminate();
//         }
//     }
// }
// template <unsigned dim>
// inline void Agent<dim>::function(){
//     this->update_agent_inputs();
//     this->receive_agent_outputs();
// }
