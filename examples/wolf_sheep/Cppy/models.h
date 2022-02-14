
#include "cppyabm/bases.h"
#include "cppyabm/mesh.h"
#include <random>
#include <vector>

struct PARAMS{
	static unsigned steps;
	static float height;
	static float width;
	static unsigned initial_sheep;
	static unsigned initial_wolves;
	// sheep_reproduce=0.04,
	static float sheep_reproduce;
	static float wolf_reproduce;
	static unsigned wolf_gain_from_food;
	static unsigned grass_regrowth_time;
	static unsigned sheep_gain_from_food;
};
unsigned PARAMS::steps = 1000;
float PARAMS::height=100;
float PARAMS::width=100;
unsigned PARAMS::initial_sheep=2000;
unsigned PARAMS::initial_wolves=500;

float PARAMS::sheep_reproduce=0.04;
// float PARAMS::sheep_reproduce=0.08;
unsigned PARAMS::grass_regrowth_time=30;

unsigned PARAMS::sheep_gain_from_food=4;

float PARAMS::wolf_reproduce=0.001;
unsigned PARAMS::wolf_gain_from_food=4;
	

struct WolfSheep;
struct Animal;
struct GrassPatch;
using DataType = std::map<std::string,std::vector<float>>;

struct random{
	static double randrange(double min, double max){
	    std::random_device rd;
	    std::mt19937 gen(rd());
	    std::uniform_real_distribution<> dis(min,max);
	    return dis(gen);
	};
	static double randint(int min, int max){
	    return rand()%(max-min) + min;;
	};
	static int choice(vector<int> list){
	    std::random_device rd;
	    std::mt19937 gen(rd());
	    std::discrete_distribution<> dis(list.begin(),list.end());
	    return list[dis(gen)];
	};
};


struct GrassPatch: public Patch<WolfSheep,Animal,GrassPatch>{

	using basePatch = Patch<WolfSheep,Animal,GrassPatch>;
	using basePatch::basePatch;
	GrassPatch(shared_ptr<WolfSheep> env,MESH_ITEM mesh_item, bool fully_grown,int countdown):basePatch(env,mesh_item){
		this->fully_grown = fully_grown;
		this->countdown = countdown;
	}
	void step();
	bool fully_grown;
	int countdown;
};

struct Animal: public Agent<WolfSheep,Animal,GrassPatch>{

	using baseAgent = Agent<WolfSheep,Animal,GrassPatch>;
	using baseAgent::baseAgent;
	Animal(shared_ptr<WolfSheep> env, string class_name, int energy):baseAgent(env,class_name){
		this->energy = energy;
		this->living = true;
	}
	
	void random_move();
	void sheep_step();
	void wolf_step();
	void step();
	int energy;
	bool living;
		
};


struct WolfSheep: public Env<WolfSheep,Animal,GrassPatch>{
	virtual shared_ptr<Animal> generate_agent(string agent_name,int wolf_energy, int sheep_energy)=0;
};

inline void GrassPatch::step(){
	if (! this->fully_grown){
		if (this->countdown <= 0){
			this->fully_grown = true;
			this->countdown = PARAMS::grass_regrowth_time;
		}				
		else this->countdown --;
	}
}

inline void Animal::step(){
	this->random_move();
	this->energy --;
	if (this->class_name == "Sheep") this->sheep_step();
	else if (this->class_name == "Wolf") {
		this->wolf_step();
	}
	else throw ;
	if (this->energy < 0){
		this->disappear = true;
		this->living = false;
	}	
}

inline void Animal::wolf_step(){
	auto local_agents = this->get_patch()->get_agents();
	vector<shared_ptr<Animal>> sheep;
	for (auto &obj:local_agents){	
		if (obj->class_name == "Sheep") sheep.push_back(obj);
	}
	if (sheep.size() > 0){
		auto chosen_index = random::randint(0,sheep.size());
		auto sheep_to_eat = sheep[chosen_index];
		this->energy += PARAMS::wolf_gain_from_food;
		sheep_to_eat->disappear = true;
	}
	
	if (this->living and random::randrange(0,1) < PARAMS::wolf_reproduce){
		this->energy /= 2;
		auto cub = this->env->generate_agent("Wolf",this->energy,0);
		auto dest = this->get_patch();
		this->env->place_agent(dest, cub,true);
	}
}

inline void Animal::sheep_step(){
		auto grass_patch = this->get_patch();
		if (grass_patch->fully_grown){
			this->energy += PARAMS::sheep_gain_from_food;
			grass_patch->fully_grown = false;
		}

		if (this->living and random::randrange(0,1) < PARAMS::sheep_reproduce){
			this->energy /= 2;
			auto lamb = this->env->generate_agent("Sheep",0,this->energy);
			auto dest = this->get_patch();
			this->env->place_agent(dest,lamb,true);
		}
			
}

inline void Animal::random_move(){
	auto neighbors = this->get_patch()->neighbors;	
	auto chosen_index=random::randint(0,neighbors.size());
	auto dest = neighbors[chosen_index];
	this->move(dest,true);
}

