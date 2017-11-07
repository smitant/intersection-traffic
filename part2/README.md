# CS441/541
## Project 4 Part 2 Documentation

## Author(s):

Anthony Smith, Russell Westeren

## Date:

11/2/2017

## Before Coding
1.a) 
The old way that Smallville handled this intersection was that one and only one car could be in the intersection at any given time.
This can be seen as a semaphore that has been initalized to 1, and surronds the protected variable "intersection". By initalizing the 
semaphore to 1, that means when we call wait(semaphore) that our semaphore's count gets decremented and will force the other cars (aka threads)
to wait until the sempahore has been signaled, increasing the count by one, and then allowing another car to pass through the protected area of
the intersection.

This is suboptimal because depending on traffic, there is room for two or more cars to be in the intersection at once. allowing for cars (threads)
to do their work.

1.b)
4 things must occur for deadlock to be possible:
Mutual exclusion
Hold and Wait
No Preemption
Circular Wait

For each quadrant of the intersection there will be mutual exclusion. Only one car can be inside of any given quadrant at any given time.
Otherwise that would be considered a crash.

When a car is inside a quadrant, that means that specific car has a hold on that resource (quadrant), and is waiting to
be able to move to the next quadrant. 

When a car acquires a quadrant, the quadrant is not released until the car is through the intersection; there is no preemption
because other cars cannot demand resources from other cars

In the situation that 4 cars arrive at the intersection at the same time from every direction and all desire to turn left, 
circular wait will occur. Each car will be requesting some resources held by another and be waiting for those resources to be
released, which can't happen until the other cars get the held resources, creating circular wait.

2.a)

Suppose for one intersection you have a car in each quadrant. Each car is attempting to make a left turn.
this will cause deadlock as each car holds a resource (quadrant) and is looking for a resource that another
car holds (hold and wait). The cars will be stuck and traffic will come to a halt.

Suppose the car in SE1 and NE2 are making right turns. The rest of the quadrants are filled with cars attempting to
go straight on their second move. Deadlock will occur in this situation.

Suppose we have cars all going straight from south 1, south 2, east, west, north1, north2 and that
the bridge is filled with N cars. If all cars
enter the intersection, taking up one quadrant on their first move, then there will be deadlock. Each car
Has a quadrant and is waiting on the next one to be available.




######################################################## Our pseduo-code before we started coding ################################################################
make a queue init to null!
make 2 additional semaphors to protect Queue and isOpen array (init to true)

make CheckinLocks methods. Overridding depending on params


Car{
  init car
  make sure arri_dir != dest_dir // get random direction takes an argument for an excluded direction, not needed anymore
  while{
    mutex
      add to queue
      print_state
    
      if (car.id == queue.peak){
      
        mutex
          if(isOpen[car.arriv_dir] == true){
            queue.pop;
            isOpen[car.arriv_dir] = false;
            print_state
            }
        mutex
    mutex
      MAKE BEFORE LOGIC.
      }
      ##########################
      INTERSECTION LOGIC
      switch arri_dir
      case east
        switch dest_dir
          case north
            checkinLocks(1);
            update car_state_t = enum;
            print_state
      ###########################      
        
    
    update car_state_t = leave
    print_state
    wait for period of random time?
    
    update cars variables so its basically a new car.
  }
  
  
  
  
######################################################## Problems we ran into while coding ################################################################
          //Only allow one thread to access the queue at a time
        semaphore_wait(&queueMutex);
            enqueue(&car_q, &this_car);
            car_t * endCarPtr = NULL;
            queuePeek(&car_q, endCarPtr);
        semaphore_post(&queueMutex);
        
        
        //if the current car's id is the one at the end of the queue.
        if(this_car.car_id == endCarPtr->car_id){
            semaphore_wait(&isOpenMutex);
                //and if the nextInLine spot is open...
                if(isOpen[this_car.appr_dir] == 1){
                    //pop the car off the queue and set the isOpen spot to false.
                    semaphore_wait(&queueMutex);
                        dequeue(&car_q, NULL);
                    semaphore_post(&queueMutex);
                    isOpen[this_car.appr_dir] = 0;
                    this_car.state = STATE_APPROACH_I1;
                    print_state(this_car,NULL);
                }
            semaphore_post(&isOpenMutex);
        }
  
}

So we ran into the problem on what to do for the else on this if check.
We want to keep the other cars doing nothing if they are not at the start of the queue.

Our two options both have disadvtanges, one busy waits, while the other takes into
account randomness because of the semaphores.
Option 1: busy waiting
do{
  pter to the start of queue
  wait for some time
  
}while(this_car.id != pt.id)

Only Pro to this situation ^^ is that once we check to see if that thread is not the
one at the start of the queue, then we are guanteed that it wont be checked again.
However this option makes each thread take up CPU time, thus taking away all the
gains that we make by not having to check the same thread multiple times.

Option 2: randomness
while(1){
  wait(someMutex);
  if(this_car.id != pt.id){
    break;
  }else{
    signal another thread to go for the if check.
  }
}

We chose to go with option 2 because semaphores guantee no starvation
having the threads wait guarentees that no CPU time will be used up as each thread
waits for its turn to check to see if it is as the start of the line.
Also realistically we are only going to have less than 100 threads to "randomly" check
We are assuming that semaphore's have some sort of built in notion of which one
to send next. So far in class we cant guantee that the thread will be picked,
But im guessing that there is some sort of fairness involved with how semaphores
are signaled.




For future implementations:
use sem_timedwait in order to have beter fairness. This is not included in 
the library that was provided to us.



isLocked array to release resources



## After Coding
1.
Cars are first initalized.

Cars are added to a queue. car_q to be more specific to our solution. Each car 
constantly checks to see if they are the first in the car_q. This makes a pool of
cars that are waiting to be the next in line. This can be seen 
by our while(1) loop. Normaly this would be busy waiting. However we used mutexes
to solve this issue. Only one car is allowed to check to see if it is the first
in the queue at any given time, while the rest must wait for a mutex to be signaled.
@@SEE "Problems we ran into while coding" SECTION ABOVE for a more detailed description about how we handled this issue.@@

Once a car knows that it can slot into the waiting in line position,
(another check that we make alongside being the first in the queue), then the car
can move into position to be the next one to go.

Once that is done, we used bakers algorithm to solve the deadlock issue.
Each car that is next in line constantly checks to see if it can lock up one
of the intersections. If it cannot then it must back out of all of the other
insections (aka mutexes) that is locked up previously. Essentially we are saying
that if a car cannot lock all of its intersections based on its turn, then
do not go and repeat those checks once again.

Once the car successfully obtains all of its resources, we let it pass with 
the time to cross and release the lock for each resource (mutex) it has based on
its turn. Finally we set the state to leaving, update some numbers and then have
this on repeat. 





2.a) AVOIDING DEADLOCK
  -
    break circular wait by forceing each car to obtain all of its resources
    before moving through the intersection.
  b) PREVENT ACCIDENTS
  -
    A car must be able to move into all the quadrants of the intersection it needs
    before it starts to go. If it cannot grab all of these sections, it will try
    again later. This ensures that no two cars are in the same position at the same
    time.
  c) IMPROVE TRAFFIC FLOW
  -
    Multiple cars can be within the intersection assuming they do not collide 
    when trying to lock all of their resources. at worst case only one car
    will be in the intersection under a rare circunstance. At best case there
    can be up to 4 fours in the intersection at a given time.
  d) PRESERVE CAR ORDER
  -
    We used a Queue in order to accomplish this. There was a big decision in 
    deciding if we wanted to implement 4 queues for each possibly direction 
    of arrival or having one large queue that all of the cars waited in.
    We went with one large queue because of fairness. if you have 4 different 
    queues and you have the situation where say N and S queues have a bunch
    of straight turns. Then you will run into an issue where the W and E directions
    will be starved of going through the intersection. Where as if you only 
    have one large queue you might not be as effecient, as it can be as you might
    have open slots for the Next in Line while a car isnt at the first of the 
    queue, BUT you guarentee that no direction will ever be starved from letting
    cars into the intersection because of a baked up queue.
  e) FAIRNESS
  -
    Fairness was ensured through the use of a queue and our isOpen array. The queue
    preserves order, which means that arrival order is preserved there. There is an
    assumption that semaphores have some aspect of fairness in their implementation.
    While we don't know exactly how semaphores decide who goes next, the material in
    class and the documentation online suggests a light level of fairness, even if
    it is small.  Randomness over a large data set also helps keep fairness, mostly
    in regards to starvation, which is covered below.
  f) PREVENT STARVATION
  -
    Starvation is prevented through a queue and our isOpen array. This array keeps
    track of whether or not a car is ready to go from a certain direction. This 
    array allows us to allow every direction to go at some point, even if it isn't 
    right away. The queue only feeds into this array when there is space available.
    Through randomness over a large enough data set, we can be certain that there
    will be a point when every car can go.





## Description:
This program runs a simulation of an intersection with emphasis on the use of threads
and semaphores.  The program will take in user input to determine how long to run
and how many cars to simulate, and then create a unique thread for each car.

As these cars run through various stages of an intersection in an attempt to either
turn right, go straight, or turn left, the status of each car is printed out in 
order to monitor correct activity.

At the end of the simulation the minimum, maximum, average, and total time spent 
at the intersection is printed out.


## How to build the software
This software is built through the use of the included make file


## How to use the software
This software is run via the command line and takes in two required arguments:
  -arg 1: time to live in seconds
          How long the intersection simulation will run for
  -arg 2: number of cars
          The number of cars to create and run through the intersection
          

## How the software was tested
This implementation was ran with the same arguments given in the example output.
The output from this implementation is repeatedly within the same order of magnitude
for all collected statistcs, including total time and number of times each car goes
through the intersection. Due to the differences in machines, order of magnitude 
was deemed an acceptable measure of success, as exactly  matching output is impossible.

The program was also tested using very large and very small inputs. For instance,
this implementation can correctly handle only one car repeatedly moving through the
intersection, and this is very fast becasue this car never has to wait on any other.
Very large inputs are very tedious to verify by hand, so the lack of any sort of error
and the immense scaling up of the statistics were all that was used to determine
success.

## Known bugs and problem areas

There are no known bugs.

