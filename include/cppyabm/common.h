#pragma once
#include <memory>

using namespace std;

//! Configuration of hatch order
template<class ENV, class AGENT, class PATCH>
struct HATCH_CONFIG{
	HATCH_CONFIG (bool flag = false, 
		    shared_ptr<PATCH> _patch = nullptr, 
		    bool inherit = false,
		    bool quiet = false,
		    bool reset = false
		    ):
			_flag(flag),_patch(_patch),_inherit(inherit),
			_quiet(quiet),_reset(reset)
	{
		
	};
	bool _flag; //!< if false, the entire hatching process is skipped.
	shared_ptr<PATCH> _patch; //!< the patch that the new agent will be set. If null, a random patch will be selected.
	bool _inherit; //!< Inherit from current patch. `Agent::inherit` should be defined for this purpose.
	bool _quiet; //!< Silent exceptions if hatch fails
	bool _reset; //!< Resets upon failure in hatching. If false, the agent will try in the next steps again
};
//! Configuration of move order
template<class ENV, class AGENT, class PATCH>
struct MOVE_CONFIG{
	MOVE_CONFIG (bool flag = false, 
		    shared_ptr<PATCH> _patch = nullptr, 
		    bool quiet = false,
		    bool reset = false
		    ):
			_flag(flag),_patch(_patch), _quiet(quiet), _reset(reset)
	{};
	bool _flag; //!< if false, the entire process is skipped.
	shared_ptr<PATCH> _patch;//!< the patch that the new agent will be set. If null, a random patch will be selected.
	bool _quiet; //!< Silent exceptions if hatch fails
	bool _reset; //!< Resets upon failure in hatching. If false, the agent will try in the next steps again
};
//! Configuration of switch order from one agent type to another
struct SWITCH_CONFIG{
	SWITCH_CONFIG (bool flag = false, 
		    string to = ""
		    ):
			_flag(flag),_to(to)
	{};
	bool _flag;
	string _to;//!< the target agent class name
};
//! Base exception class
struct base_exception_class{
    base_exception_class(std::string msg):message(msg){}
    std::string message;
    const char *what() const throw() {
        return message.c_str();
    }
};
struct invalid_directory: public base_exception_class{
    using base_exception_class::base_exception_class;
};
struct convergence_error: public base_exception_class{
    using base_exception_class::base_exception_class;
};
struct patch_availibility: public base_exception_class{
    using base_exception_class::base_exception_class;
};
struct undefined_member: public base_exception_class{
    using base_exception_class::base_exception_class;
};
struct invalid_pointer: public base_exception_class{
    using base_exception_class::base_exception_class;
};
struct illegal_action: public base_exception_class{
    using base_exception_class::base_exception_class;
};


// const std::string main_output_folder = "outputs/";

