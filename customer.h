// FILE: customer.h 
// CLASS PROVIDED: customer
//     A customer has the following features: 
//     id: a positive integer identification number
//     arrival_time: a real value showing the arrival time
//     departure_time: a real value showing the departure time
//
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <iostream>
using namespace std;

class customer{
private:
	unsigned int id;
	double arrival_time;
	double departure_time;
	
public:
	//constructors
	customer() {}
	customer(unsigned int i) { id = i; }
	customer(unsigned int i, double t1, double t2)
    {
        id = i;
        arrival_time = t1;
        departure_time = t2;
    }

    // MODIFICATION MEMBER FUNCTIONS
	void set_id(unsigned int i) {id = i; }
	void set_arrival(double t) { arrival_time = t; }
	void set_departure(double t) { departure_time = t; }
	
    // CONSTANT MEMBER FUNCTIONS
	unsigned int get_id() const { return id; }
	double get_arrival() const { return arrival_time; }
	double get_departure() const { return departure_time; }
	void print_info() const { cout << "C" << id; }
	double get_wait_time() const { return departure_time - arrival_time; }
};

#endif 
