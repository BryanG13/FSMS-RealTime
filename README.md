# FSMS-RealTime: Flexible Semi-Fixed Route Bus System with Real-Time Demand

A C++ optimization solver for the **dynamic Feeder Service with Mandatory Stops (FSMS)** that adapts semi-fixed bus routes in real-time to accommodate passenger requests while maintaining service frequency and capacity constraints. This is the code related to this (academic paper)[https://doi.org/10.1080/23249935.2023.2227738].

---

## Overview

This project implements an **online optimization algorithm** for bus route planning that:
- Dynamically adjusts bus routes based on real-time passenger demand
- Maintains mandatory bus stops while flexibly adding optional stops
- Balances competing objectives: travel time, walking distance, and schedule adherence
- Handles both arrival-based (destination-focused) and departure-based (origin-focused) passenger requests
- Uses parallel search with simulated annealing and large neighborhood search (LNS) to find high-quality solutions

---

## Problem Description

### System Components

**Bus Network:**
- **Mandatory stops** (N): Fixed stops that all buses must visit in sequence
- **Optional stops** (M per cluster): Additional stops between mandatory stops that can be dynamically added based on demand
- **Fleet** (B buses): Each bus operates multiple trips throughout the planning horizon

**Passenger Requests:**
- **Arrival requests (R1)**: Passengers specify desired arrival time at destination
- **Departure requests (R2)**: Passengers specify desired departure time from origin
- Each request includes a timestamp indicating when the request was made

**Constraints:**
- **Capacity:** Maximum passengers per bus (C)
- **Walking distance:** Maximum walking time from passenger location to pickup/dropoff stop (dw)
- **Frequency:** Minimum time between consecutive buses at mandatory stops (xt)
- **Time windows:** Tolerances for early/late arrivals and departures (d_ae, d_al, d_de, d_dl)
- **Service time:** Maximum deviation from promised pickup time (d_t)
- **Idle time:** Maximum wait time buses can add to accommodate requests (max_wait)

### Objective Function

Minimize a weighted sum of:
1. **c1**: Total bus travel time
2. **c2**: Total passenger walking time  
3. **c3**: Total deviation from desired arrival/departure times

Default weights: c1 = 0.33, c2 = 0.33, c3 = 0.34

---

## Algorithm

### Solution Approach

1. **Initial Solution Generation (Parallel)**
   - Generate multiple initial feasible solutions using randomized heuristics
   - Construct bus routes by iteratively:
     - Selecting the next available bus
     - Greedily assigning passengers based on arrival/departure times
     - Dynamically inserting optimal bus stops using `bestStop()` function
     - Adjusting timetables to satisfy frequency and time window constraints

2. **Solution Improvement (Large Neighborhood Search)**
   - Iteratively destroy and repair solutions:
     - **Destroy:** Randomly select parameters (PM weights, frequency tolerance, capacity) to perturb
     - **Repair:** Rebuild affected portions of the solution
   - Use parallel exploration with OpenMP
   - Accept worse solutions probabilistically (simulated annealing-style)

3. **Key Functions:**
   - `bestStop()`: Finds the optimal bus stop to assign to a passenger considering route feasibility
   - `insertStop()`: Inserts a new stop into an existing route while maintaining constraints
   - `insertstop()`: Real-time insertion for dynamic passenger requests during operation

---

## Input Data

### Required Files

Located in `data/input/` or `data/input/Antwerp/`:

**Coordinate-based mode (inst_gen = 0):**
- `passengers.txt`: Passenger origin/destination coordinates (x, y)
- `mandatory.txt`: Mandatory bus stop coordinates
- `optional.txt`: Optional bus stop coordinates
- `arrivals.txt`: Desired arrival times for arrival-based passengers
- `departures.txt`: Desired departure times for departure-based passengers
- `timestamps.txt`: Request timestamp for each passenger

**Antwerp dataset mode (inst_gen = 1):**
- `walking_data.csv`: Pre-computed walking times from passengers to stops
- `travel_time.csv`: Pre-computed travel times between all bus stops
- Plus the same .txt files with suffixes (e.g., `passengers100.txt`, `optional8.txt`)

### Default Parameters (Configurable in code)

- **B = 18**: Number of buses
- **N = 8**: Number of mandatory stops
- **M = 8**: Optional stops per cluster
- **OG_R = 100**: Total passenger requests
- **C_OG = 20**: Bus capacity
- **TS = 15,120s** (4.2 hours): Planning horizon
- **OGxt = 1,200s** (20 min): Minimum headway between buses
- **dw = 1,200s** (20 min): Maximum walking time
- **pspeed = 1.0 m/s**: Pedestrian walking speed
- **bspeed = 11.11 m/s** (40 km/h): Bus driving speed

---

## Build & Run

### Prerequisites

- CMake 3.22+
- C++17 compatible compiler (Clang, GCC)
- OpenMP (for parallel execution)

### Build

```bash
cmake -B build -S .
cmake --build build
```

### Run

```bash
./build/DFSMS
```

The program will:
1. Run 5 independent instances (iruns = 0 to 4)
2. Generate instance configuration files: `data/input/Instance_ANT_1.txt` through `Instance_ANT_5.txt`
3. Output solution quality and routing details to console

---

## Output

### Instance Files

Generated at `data/input/Instance_ANT_<i>.txt` containing:
- Objective function weights
- System parameters (buses, stops, capacity, time limits)
- Passenger data (walking times, desired arrival/departure times)
- Travel time matrices between bus stops

### Console Output

- Best routes for each bus (sequence of stops)
- Passenger assignments (bus, trip, stop)
- Travel times and objective function values
- Feasibility checks and constraint violations

---

## Code Structure

### Main Components

- **CSV Parsers** (`read_walking_matrix`, `read_travel_matrix`): Load Antwerp dataset
- **Preprocessing**: Compute nearest stops, sort passengers by time
- **Route Optimization**: Find best stop sequences using local search
- **Initial Solution**: Greedy construction with frequency maintenance
- **LNS Improvement**: Iterative destroy-and-repair with parallel search
- **Insertion Functions**: Dynamic stop insertion with feasibility checks

### Key Data Structures

- `xsol[B][T]`: Bus routes (vector of stop IDs per trip)
- `Dsol[B][T]`: Departure times at each stop
- `ysol[R]`: Passenger assignments [bus, trip, stop]
- `freqN[N]`: Last departure time at each mandatory stop (for frequency constraint)

---

## Real-Time Operation

The system supports real-time passenger insertion through:
- `insertstop()`: Attempts to insert a new request into existing planned routes
- Checks feasibility against all constraints (capacity, frequency, time windows)
- Minimizes disruption to existing passengers
- Returns insertion cost or -1 if infeasible

---

## Performance

- **Parallelization**: Uses OpenMP to explore multiple solution neighborhoods simultaneously
- **Iterations**: Default 30,000 iterations per instance with early stopping on convergence
- **Memory**: AddressSanitizer enabled in Debug builds for memory safety validation

---

## Notes

- The system is designed for **semi-fixed route buses** (not fully flexible ride-sharing)
- Maintains predictable service at mandatory stops with adaptive coverage between them
- Trade-offs can be tuned via objective weights (c1, c2, c3) and parameter ranges (PM, fpm)
- Real-world deployment would require integration with:
  - Live passenger request systems (mobile apps, call centers)
  - Vehicle tracking/dispatching systems
  - Real-time traffic data for travel time updates

---

## Authors & References

This implementation is based on research in demand-responsive transit optimization. For academic context and methodology details, please refer to related publications on flexible transit systems.

---
