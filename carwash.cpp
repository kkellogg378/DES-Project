/// Please add your name below
// Name: Kyler Kellogg

// carwash.cpp implementation of the car wash simulator

#include <random>  // needs c++11
#include <queue>
#include <list>
#include "customer.h"
#include "washevent.h"

bool read_debug_option();

int main()
{
    // average time interval between two consecutive arrivals
    double ave_interarrival_time;
    
    // average service time for each customer
    double ave_service_time;
    
    // total simulation time
    double total_simulation_time;
    
    // debug options
    bool debug_opt1, debug_opt2;
    
    //----------------------------------------------------------------------
    //get user inputs
    cout << "Input the average time interval of two consecutive intervals: ";
    cin >> ave_interarrival_time;
    
    cout << "Input the average service time: ";
    cin >> ave_service_time;
    
    cout << "Input the simulation time: ";
    cin >> total_simulation_time;
    
    // The following can be comment out during debugging
    // total simulation time should be at least 10,000 * ave_interarrival_time
    if (total_simulation_time < 10000 * ave_interarrival_time)
    {
        cout << "Error: total simulation time should be greater than " 
            << ave_interarrival_time * 10000 << endl;
        return 1;
    }
    
    cout << "Debugging option:" << "\n";
    cout << "- Print items in queue whenever there is a change in queue? (y/n): ";
    debug_opt1 = read_debug_option();
    cout << "- Print events when there is an arrival or departure? (y/n): ";
    debug_opt2 = read_debug_option();
    
    //----------------------------------------------------------------
    // create a queue of customers coming to the carwash
    queue<customer*> line;
    
    //create a list of events
    list<washevent*> event_list;
    
    //initialization randon number generator and distributions
    random_device rd;
    default_random_engine gen(rd());
    uniform_real_distribution<double> arrival_dis(0.0, 2.0*ave_interarrival_time);
    uniform_real_distribution<double> service_dis(0.0, 2.0*ave_service_time);
    
    //-----------------------------------------------------------
    // TODO: 
    // 1. generate the first arrival, push into event_list
    // 2. generate an "end_of_simulation" event, push into event_list
    
    washevent* firstArrival = new washevent(arrival, 0, 1);
    washevent* simulationEnd = new washevent(end_of_simulation, total_simulation_time, 0);
    
    event_list.push_back(firstArrival);
    event_list.push_back(simulationEnd);
    
    washevent* next_event;
    
    double num_of_customers = 0;
    double total_waiting_time = 0;
    
    double current_queue_size = 0;
    double time_last_queue_size_change = 0;
    double average_queue_size = 0;
    
    do
    {
        // retrieve the next event from the event list
        next_event = event_list.front();
        
        event_list.pop_front();
        
        // print events
        if (debug_opt2) {
            cout << "[debug] ";
            next_event->print_info();
        }
        
        switch ( next_event->get_type() )
        {
            case arrival: {
                //-----------------------------------------------------------
                // TODO: 
                // 1. create a new customer
                // 2. if the line is empty, update departure time,
                //       create a departure event and push into event_list
                // 3. push the customer into the customer line
                // 4. create a new arrival event and push into event_list
                
                // New customer has ID of next_event, arrival time of next_event, and departure time of -1 temporarily
                customer* newCustomer = new customer(next_event->get_customerid(), next_event->get_time(), -1);
                
                // Activates if line is empty. The customer will get service right away, so 
                // we can just add the randomly generated service time to the arrival time
                if (line.empty()) { 
                    newCustomer->set_departure(next_event->get_time() + service_dis(gen));
                    // Create an appropriate washevent for this customer
                    washevent* newDeparture = new washevent(departure, newCustomer->get_departure(), newCustomer->get_id());
                    event_list.push_back(newDeparture);
                }
                
                // Pushes new customer into customer line
                line.push(newCustomer); 
                
                // New washevent has type arrival, time of previous arrival + randomly generated interarrival time, and ID of previous arrival + 1
                washevent* newArrival = new washevent(arrival, next_event->get_time() + arrival_dis(gen), next_event->get_customerid() + 1);
                
                event_list.push_back(newArrival);
                
                // Handling the average number of customers in the queue: 
                if (current_queue_size != line.size()) {
                    average_queue_size += current_queue_size * (next_event->get_time() - time_last_queue_size_change);
                    current_queue_size = line.size();
                    time_last_queue_size_change = next_event->get_time();
                }
                
                // sort the event list in chronological order
                event_list.sort(comp_time);
                break;
            }
            
            case departure: {
                //-----------------------------------------------------------
                // TODO: 
                // 1. pop the front customer from the line
                // 2. keep track of the average waiting time and average queue length
                // 3. if the line is not empty, update the departure time 
                //    of the next customer; create a new departure event 
                //    and push into event_list
                
                customer* current_customer = line.front();
                
                line.pop();
                
                customer* nextCustomer = line.front();
                
                //if (nextCustomer->get_departure() == -1) {
                if (!line.empty()) {
                    nextCustomer->set_departure(next_event->get_time() + service_dis(gen));
                    // Create an appropriate washevent for this customer
                    washevent* newDeparture = new washevent(departure, nextCustomer->get_departure(), nextCustomer->get_id());
                    event_list.push_back(newDeparture);
                }
                
                // Handling the average waiting time data:
                total_waiting_time += current_customer->get_departure() - current_customer->get_arrival();
                num_of_customers++;
                
                // Handling the average number of customers in the queue:
                if (current_queue_size != line.size()) {
                    average_queue_size += current_queue_size * (next_event->get_time() - time_last_queue_size_change);
                    current_queue_size = line.size();
                    time_last_queue_size_change = next_event->get_time();
                }
                
                // sort the event list in chronological order
                event_list.sort(comp_time);
                break;
            }
            
            case end_of_simulation:    
                //-----------------------------------------------------------
                // TODO: print statistics
                
                // Handling the queue size between last change and end of simulation
                average_queue_size += current_queue_size * (next_event->get_time() - time_last_queue_size_change);
                
                // Outputs
                cout << "The average number of customers in the queue is " << (average_queue_size / total_simulation_time) << endl;
                cout << "The average waiting time for a customer is " << (total_waiting_time / num_of_customers) << endl;
                
                event_list.clear(); // to kill the simulation
                
                debug_opt1 = false; // to disable the final debug output
                
                break;
            
            default:
                cout << "Error: incorrect event type." << endl;
        }
        
        // print items in queue
        if (debug_opt1) {
            cout << "[debug] Queue at time " << next_event->get_time() << " (from front to rear): ";
            queue<customer*> readline = line;
            while (!readline.empty()) {
                cout << "C" << readline.front()->get_id() << " ";
                readline.pop();
            }
            cout << endl;
        }
    
    } while ( !event_list.empty() );
    
    // clean up
    while (!line.empty())
    {
        customer* c = line.front();
        delete c;
        line.pop();
    }
    
    event_list.clear();
    
    return 0;
}

bool read_debug_option() 
{
    char input;
    cin >> input;
    if ( input == 'Y' || input == 'y') return true;
    if ( input == 'N' || input == 'n') return false;
    
    cout << "Error: incorrect input option!" << endl;
    exit(1);
}