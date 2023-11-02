// FILE: washevent.h 
// CLASS PROVIDED: washevent
//     A washevent has the following features: 
//     type: the event type, should be one of the following
//           { arrival, departure, end_of_simulation }
//     time: a real value showing the time of occurence
//
#ifndef WASHEVENT_H
#define WASHEVENT_H

#include <iostream>
using namespace std;

enum events_t { arrival, departure, end_of_simulation };

class washevent{
private:
	events_t type;
	double time;
    unsigned int customer_id;
	
public:
	//constructors
	washevent() {}
	
	washevent(events_t type_t, double time_t, unsigned int id)
    {
        type = type_t;
        time = time_t;
        customer_id = id;
	}

    // MODIFICATION MEMBER FUNCTIONS
	void set_type(events_t t) { type = t; }
	void set_time(double t) { time = t; }
	void set_customerid(unsigned int id) { customer_id = id; }

    // CONSTANT MEMBER FUNCTIONS
	events_t get_type() const { return type; }
	double get_time() const { return time; }
    unsigned int get_customerid() const { return customer_id; }
	void print_info() const 
	{
         switch ( type )
         {
             case arrival:
                 cout << "C" << customer_id << " arrives at time " << time << endl;
                 break;
                 
             case departure:
                 cout << "C" << customer_id << " departs at time " << time << endl;
                 break;
                 
             case end_of_simulation:
                 cout << "End of simulation at " << time << endl;
                 break;
                 
             default: 
                 cout << "Error: incorrect event type." << endl;
         }
     }
             
};

// NON-MEMBER FUNCTION
// Define a function comparing the time of occurence
// purpose: to sort the event list in chronological order
bool comp_time(washevent* first, washevent* second){
	if ( first->get_time() < second->get_time() )
        return true;
	
    return false;
}
#endif

