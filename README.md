# C++ Agent-Based Innovation Diffusion Model

Agent-based simulation of innovation diffusion (e.g. green practices) in social network

---

## Table of Contents

- [C++ Agent-Based Innovation Diffusion Model](#c-agent-based-innovation-diffusion-model)
  - [Table of Contents](#table-of-contents)
  - [Project Overview](#project-overview)
  - [Model Description](#model-description)
  - [Algorithm Overview](#algorithm-overview)
  - [References](#references)
  - [Project Structure](#project-structure)
  - [Installation](#installation)
    - [Requirements](#requirements)
    - [Steps to build and run](#steps-to-build-and-run)
  - [Usage](#usage)
  - [Configuration](#configuration)
  - [Output](#output)
    - [Visualizing results with Python](#visualizing-results-with-python)
  - [Extensions](#extensions)
  - [License and Terms of Use](#license-and-terms-of-use)

---

## Project Overview

This project implements an **agent-based model (ABM)** for simulating the diffusion of innovations [`[1]`](#1-e-m-rogers-diffusion-of-innovations-5th-edition-the-free-press-2003) — such as green practices — through a social network. The model is based on a previously published ABM of green product diffusion [`[2]`](#2-k-byrka-a-jedrzejewski-k-sznajd-weron-and-r-weron-difficulty-is-critical-the-importance-of-social-factors-in-modeling-diffusion-of-green-products-and-practices-renewable-and-sustainable-energy-reviews-vol-62-pp-723-735-09-2016). It incorporates two key mechanisms: (1) **social interactions** between agents and (2) the probability of participating in a given behavior. Unlike the original version, which assumed a fixed functional form for engagement, the new model treats the **engagement probability as an independent parameter**. This change makes the model a more general framework for simulating innovation diffusion processes that go beyond pairwise interactions.

The updated implementation also enables **systematic sensitivity analysis** and captures **temporal adoption dynamics**, allowing researchers to compare simulated patterns with empirical observations (e.g., S-shaped diffusion curves).

Written in C++, the model allows researchers and students to explore how **individual decision-making**, **social influence**, and **network structure** affect the spread of behavior over time. It is intended as a tool for researchers, students, and educators interested in computational modeling, innovation diffusion, and social dynamics.

---

## Model Description

The ***q*-voter model with independence** [`[3]`](#3-p-nyczka-k-sznajd-weron-j-cislo-phase-transitions-in-the-q-voter-model-with-two-types-of-stochastic-driving-physical-review-e-07-2012) is a framework for modeling how binary opinions — such as yes/no, for/against, or adopter/non-adopter (in the case of innovation diffusion) — spread in a population. The model captures two responses to social influence: **conformity** and **independence**. In the case of conformity, an agent follows the unanimous opinion of a selected group of neighbors — an idea inspired by classic social psychology experiments (e.g., Asch's conformity studies [`[4]`](#4-s-e-asch-opinions-and-social-pressure-scientific-american-1935-31-35-1955)). Independence, in contrast, reflects situations where individuals ignore social pressure and rely on their own internal motivations. The model originates from statistical physics and resembles the Ising model [`[5]`](#5-c-castellano-m-a-munoz-r-pastor-satorras-nonlinear-q-voter-model-physical-review-e-10-2009).

In this version, independent agents adopt the behavior with a specific engagement probability, meaning they are more or less likely to choose adoption over rejection.

- **Agents**: *N* individuals who decide whether or not to adopt an innovation (binary decision).
- **Network**: A graph representing social connections (e.g. complete graph, random network, Watts-Strogatz, or real-world structure), represented as an adjacency list.
- **Update Rule**:  At each elementary time step, a randomly chosen target agent chooses one of two behaviors:
    - **independence** with probability $p^{\text{ind}}$: the agent adopts the behavior according to the engagement probability $p^{\text{eng}}$.
    - **conformity** with probability $(1 - p^{\text{ind}})$: the agent adopts the unanimous opinion of a randomly selected group of $q$ neighbors, if such unanimity exists.
- **Simulation**: The model uses **asynchronous updates**. Each Monte Carlo step (MCS) consists of *N* elementary steps, where in each step one agent is selected **at random (with replacement)** from the population and updated one by one according to the rules. This means some agents may be updated multiple times, while others may not be selected at all within a given MCS. Such an approach better reflects real-time processes, where people make decisions at different moments and behaviors spread through individual interactions, like conversations or spontaneous actions.

---

## Algorithm Overview

The simulation models opinion dynamics on a network according to the following algorithm:

1. Initialize the network and model parameters, and set time counter to zero.

2. Randomly select one agent (called the *target*) among all $N$ agents to reconsider its opinion. Increment time by $1/N$.

3. With probability $p^{\text{ind}}$, the target behaves **independently**:
   - With probability $p^{\text{eng}}$, it adopts opinion $+1$ (adopt).
   - With complementary probability $1 - p^{\text{eng}}$, it adopts opinion $-1$ (withdraw adoption).

4. With complementary probability $1-p^{\text{ind}}$, the target is a **conformist**:
   - The target randomly selects $q$ neighbors.
   - If all chosen neighbors share the same opinion, the target conforms and adopts that opinion.

5. Repeat from step 2.

---

## References

##### [1] E. M. Rogers, "Diffusion of Innovations", 5th Edition. The Free Press, 2003.

##### [2] K. Byrka, A. Jedrzejewski, K. Sznajd-Weron, and R. Weron, "Difficulty is critical: The importance of social factors in modeling diffusion of green products and practices", Renewable and Sustainable Energy Reviews, vol. 62, pp. 723-735, 09 2016.

##### [3] P. Nyczka, K. Sznajd-Weron, J. Cislo, "Phase transitions in the *q*-voter model with two types of stochastic driving", Physical Review E, 07 2012.

##### [4] S. E. Asch, "Opinions and social pressure", Scientific American, 193(5), 31-35, 1955.

##### [5] C. Castellano, M. A. Munoz, R. Pastor-Satorras, "Nonlinear *q*-voter model", Physical review E, 10 2009.

---

## Project Structure

The project contains the following source and header files:

- `Agent.h`  
  Definitions of the agent class and related functions.

- `AutomaticallyGeneratedNetworkTopologies.h`  
  Contains definitions and implementations of network topologies represented as adjacency lists.

- `AutomaticallyGeneratedNetworkTopology.h`  
  Contains definitions and implementations of a network topology represented as an adjacency list.

- `CompleteGraph.h`  
  Implementations of a complete graph (where every node is connected to every other node).

- `ComputeTrajectory.h`  
  Functions for computing trajectories of model runs.

- `Executionflow.h`  
  Manages execution flow and the order of operations.

- `Graph.h`  
  Defines the graph structure used in simulations.

- `Main.cpp`  
  Main program file containing the `main()` function.

- `MathFunctions.h`  
  A collection of helper mathematical functions.

- `ModelParameter.h`  
  Definitions of model parameters used in simulations.

- `RandomNumberHandler.h`  
  Module for managing the random number generator.

- `README.md`  
  This file — project description and usage instructions.

- `ResultFolderHandler.h`  
  Handles the results directory and data saving.

- `ResultHandler.h`  
  Functions responsible for processing and saving simulation results.

- `SocialNetwork.h`  
  Implementations of real-world social networks used in the analysis.

- `TheoreticalGraph.h`  
  Implementations of the Watts-Strogatz theoretical graph model used in analysis.

- `Build.bat`  
  Script for automated project compilation.

---

## Installation

### Requirements

- A C++ compiler (best would be **g++**)
 - Windows: use the `Build.bat` batch script
 - Linux: use the `Build.sh` bash script
- Optional: an IDE or editor of your choice (e.g., Visual Studio Code)

### Steps to build and run

1. Clone or download this repository:

   ```
   git clone <repository_url>
   ```

2. Open your terminal and navigate to the project directory.

3. Run the build script to compile the project:

    ```powershell
   Build.bat
   ```
   or
   ```bash
   Build.sh
   ```

4. After a successful build, an executable file (e.g., InnovationDiffusion.exe) will be created in the project directory.

5. Run the executable to start the program:

    ```powershell
   build//InnovationDiffusion.exe
   ```
   or
   ```bash
   build/InnovationDiffusion
   ```

---

## Usage

To run the simulation, follow these steps:

1. **Configure parameters**
Adjust the simulation parameters in `ModelParameter.h` to suit your needs, such as network topology, model parameters, and simulation settings.

2. **Build the project**
Run the provided `Build.bat` or `Build.sh` script to compile the program.

3. **Run the executable**
Execute the compiled program (e.g., `InnovationDiffusion.exe`). The simulation will run with the chosen settings.

4. **View results**
After completion, check the `Results` directory for .csv output files containing adoption dynamics data.

---

## Configuration

The `ModelParameter.h` file defines key parameters and settings used throughout the simulation, including the choice of network topology, model parameters, and simulation control variables.

- **Network topology selection:**  
  Choose between real-world networks (`PREDEFINED_TOPOLOGY`), a complete graph (`COMPLETE_GRAPH`), or theoretical models such as Watts-Strogatz (`THEORETICAL_GRAPH`).

- **Model parameters:**  
  Define network size (`N`), influence group sizes (`q_min` - initial value, `q_steps` - number of different values, `dq` - increment), and probabilities related to agent behavior (`p_eng_min`, `p_eng_steps`, `dp_eng`, `p_ind_min`, `p_ind_steps`, `dp_ind`).

- **Simulation settings:**  
  Initial fraction of adopters (`c0`), number of simulation time steps (`T`), and number of independent runs (`LS`).

- **Watts-Strogatz network parameters:**  
  Minimum average number of neighbors (`k_min`) and rewiring probability (`b_min`).

- **Execution:**  
  Parallel execution is enabled via `EXECUTION_POLICY` (`std::execution::par`).

This centralized configuration allows easy adjustment of simulation parameters before compilation.

---

## Output

The simulation generates a `.csv` file with the cumulative adoption rates (from 0 to 1) over time, saved automatically to the `Results` directory. Each file contains a single row with adoption values at consecutive time steps, separated by commas.

Check the `Results` folder for the generated data files and reports after running simulations.

### Visualizing results with Python

You can visualize the adoption dynamics using example visualization from `Scripts` folder

This code reads the single-row CSV data, converts it to a series of adoption rates over time, and plots the diffusion curve.

Feel free to customize the analysis and visualization according to your needs.

---

## Extensions

Heterogeneous agents (e.g. individual engagement probability)

Learning, memory, different goals or strategies

Different response to social influence, e.g. anticonformity

Dynamic networks (changing connections)

External influence (e.g. media campaigns)

---

## License and Terms of Use

This project is Licensed under the MIT licence
